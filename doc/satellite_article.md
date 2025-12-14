# From Singletons to Satellites

*Calum Grant, originally written November 2012*

> Singleton is a much-maligned design pattern, and is usually too blunt to be used effectively. With a few modifications, a much more disciplined pattern emerges, which has the performance and convenience of Singleton, without its fundamental flaws.

## Introduction

Singleton is a design pattern which provides global access to a single instance of a class. Whilst this sounds simple and unproblematic, this is one of the most controversial patterns in object-oriented design, and there is a good case for avoiding Singleton entirely.

However with some simple tweaks, Singleton can be tamed into something much more usable, which I’ll call “Satellite”. Satellite is a simple solution to the problem of

> *How does function X access object Y, without passing/storing object Y everywhere throughout the code?*

This is actually a very real problem, since “doing the right thing” of removing a singleton can involve a lot of extra work, and lead to a slower program which consumes more memory. 

To see why this is so, imagine the following C++ code:

```c++
// Singleton version:

SysLogger logger;

int compute(int a, int b)  
{  
    logger.log(“Now computing”);  
    return a+b;  
}

struct Compute  
{  
    int compute(int a, int b)   
    { logger.log(“Now computing”); return a+b; }  
};
```

The global variable *logger* is effectively a singleton, though we can wrap it up and present it in more sophisticated ways. In order to “improve” the design, we can remove the singleton, add an interface class (*Logger*), and pass the *Logger* in as an argument to each class or function which uses it.  

```c++
// Non-singleton version:

int compute(int a, int b, Logger & logger)  
{  
    logger.log(“Now computing”);  
    return a+b;  
}

class Compute  
{  
public:  
    Compute(Logger & l) : m\_logger(l) { }  
    int compute(int a, int b)   
    { m_logger.log(“Now computing”); return a+b; }  
private:  
    Logger & m_logger;  
};
```

The new code is much more flexible because it is no longer tied to the singleton, but it has come at a high cost of carrying an extra variable throughout the code. If we had a million `Compute` objects, that would be a lot of extra baggage!

## So why is Singleton so bad?

1. Singletons are effectively global variables, and this means that different unrelated parts of the program can suddenly interact, and you have action at a distance. This makes debugging harder.  
2. Unit testing is much more difficult because you can’t change or replace the singleton, and your unit tests might end up writing to the real database or logging to the system log. You can’t test the singleton itself.  
3. Code reuse is more difficult because you must remember to copy the singleton into the new application, where it might conflict with something else.  
4. Threading is more challenging because the singleton might be accessed by multiple threads, which is perhaps inefficient and could introduce race conditions. Lazy initialization has its own challenges.  
5. The decision that there should only be one instance of something can turn out to be premature.  
6. Encapsulation is broken, because a module can no longer operate in isolation.  
7. Static initialization is messy, because you don’t have good control over what order objects are initialized. Multiple libraries can pull in different versions of the singleton, and shared libraries can create multiple instances of the singleton. Destruction is generally out of the programmer’s control.  
8. Singletons can only be default-initialized or initialized from other singletons.  
9. You cannot control what uses the singleton.  
10. The object model is broken because true singletons are rare. The number pi is a singleton, but configurations, log files, memory allocators or data stores are not truly unique at all.  
11. The intent of the programmer using a singleton is usually to conveniently access a facility, not to enforce the uniqueness of that facility.  
12. It is harder to recover from exceptions and other initialization failures of singletons.  
13. Singletons fail to document dependencies in class/function declarations.

## Enter Satellite

*Satellite* is a proposed design pattern which allows parameters to be communicated via a side-channel without the need to pass them by reference. Each parameter is accessed via a type, so for example, `satellite<Logger>` refers to the `Logger` satellite, `satellite<Allocator>` refers to the `Allocator` satellite and so on.

Unlike singletons, satellites must be explicitly defined, and only exist for a limited scope. For example, to define a Satellite:

```c++
int main()  
{  
	SysLogger syslog;  
	satellite<Logger>::set logger(syslog);  
	compute(1,2);  
}
```

Unlike a singleton, the satellite is defined in the *main()* function itself, or at whatever scope is appropriate in your application. The implementation can be polymorphic, for example `SysLogger` implements `Logger`. There are no longer any issues of static initialization. In this example, the scope of the satellite is the variable `logger`.

Code which uses the satellite:

```c++
int compute(int a, int b)  
{  
	satellite<Logger>()->log(“Now computing”);  
	return a+b;  
}
```

Unlike singletons, satellites can be nested and only affect the current thread, so they are conceptually much more like named parameters than global variables. Satellites are by default thread-local, which means that different threads will not interfere. The thread function must initialize the appropriate satellites in each thread.

We can have multiple satellites of the same type, distinguished by “type tags”. For example

```c++
// Set up the satellites:  
satellite<Logger,Database>::set s1(testdb);  
satellite<Logger,System>::set s2(dummydb);

// In a function call far far away...  
satellite<Logger,Database>()->log(“Accessing a locked account”);  
satellite<Logger,System>()->log(“Internal error 789”);
```

The implementation of Satellite is straightforward, and is shown in [Listing 1](../include/cutty/satellite.hpp). So far there is only a C++ implementation, since C++ has templates and object scope. At the heart of the implementation is the function

```c++
template<typename T, typename Tag>  
static T *& pointer() { thread_local T *t=0; return t; }
```

This function stores the current value of the satellite in a thread local variable, and returns a unique pointer for each combination of `T` and `Tag`.

### Nesting Satellites

The `satellite<>::set` object can be nested, which means that a function can temporarily replace a satellite. The destructor of `satellite<>::set` restores the previous satellite, so that a function does not interfere with the caller.

In the following example, we have chosen to implement a counter using a `satellite<int>`, as opposed to the more conventional approach of passing a reference or returning a value. The function installs a new satellite `s2` which temporarily replaces `s1`. (Exercise: what number is output, and what would be the output for each permutation of lines 1,2 and 3?)

```c++
struct TotalTag;  
typedef satellite<int,TotalTag> Counter;

void nested(int count)  
{  
	if( count>0 )  
	{  
		Counter() \+= count; // 1  
		Counter::value<> s2; // 2  
		nested(count-1);     // 3  
	}  
}

int main()  
{  
	Counter::value<> s1;  
	nested(10);  
	std::cout << "Total is " << *Counter();  
}
```

Note that `satellite<>::value<>` is a utility for instantiating and setting a satellite. The type tag `TotalTag` is not necessary in this example, but would be necessary to differentiate different counters.

## Why is Satellite better than Singleton?

Addressing the previous list of deficiencies,

1. Unlike Singleton, the caller has full control over the objects passed as satellites. Satellites have a well defined scope and cannot “leak” out of the current scope. It is possible to override a satellite locally or on a per-thread basis. Satellites are not required to be global.  
2. Satellites can be unit tested, and used in unit tests, by simply defining the satellite in each unit test as opposed to the `main()` function or as a global variable.  
3. Satellites are defined by the caller, rather than at global scope. There is no requirement to define the satellite at global scope.  
4. Satellites are by default thread-local so are completely thread-safe. The initialization is done at a defined point in the code so does not have the issues associated with lazy initialization and double-locking.  
5. There is no requirement for satellites to be unique. Satellites can be redefined and nested, and different modules can assign their own satellites. Type-tags can further isolate different instances.  
6. Encapsulation is fully restored, because the caller/user of a module has full control over the satellite variables.  
7. Satellite variables are generally not static. The initialization is performed by the application, which can ensure that instances are created and destroyed in a well defined sequence. Objects are instantiated by the caller just like normal pass-by-reference.  
8. Satellite variables are initialized like normal variables, so do not have any constraints on how they are initialized.  
9. Satellites can be set locally or protected using a (private) type tag, so that no other part of the program can access it.  
10. Satellites are only locally unique, not globally unique.  
11. Satellites offer convenient access to facilities without constraining their uniqueness.  
12. Initialization failures can be handled correctly.  
13. Satellites do not document dependencies in class/function declarations.

## Typical applications

In general, any solution which uses Singleton can use Satellite. Satellite is applicable for most background facility objects, which tend to persist for a larger scope, which are often implemented as singletons. For example, loggers, settings, resource monitors, schedulers, work queues, databases connections, memory allocators, storage facilities, `boost::io_service` etc, can all be passed by Satellite, to avoid endless parameter passing.

Another application is efficiency. For example if you allocate millions of objects, all of which need to store pointers, then there is a speed and space saving by using satellites. A typical example could be C++ allocators, which are quite fiddly to initialize, and may all point to the same thing.

Another application could be a component registry. Often you end up with a “master object” representing all of the state and components. A satellite could replace the need for such a context object, by providing direct access to facilities as needed.

Satellites can pass values through third-party code. A defaulted parameter could read its value from a satellite. Satellites can be used for collecting debugging information and statistics without rewriting the code under test. Satellites can be used for optional or rarely used parameters.

## Conclusion

Satellite is a proposed design pattern to enable access to objects without parameter passing. This achieves the convenience and efficiency of the Singleton pattern without its drawbacks. It can be used as a replacement for Singleton in most cases, but does not mandate the global uniqueness of a class.

The primary motivator for using Satellite is flexibility and convenience, not performance. If you suddenly find yourself needing access to an object, and the only way to get it is to change about 20 function signatures, then Satellite could be the answer. With great power comes great responsibility, and using function signatures to document dependencies is no bad thing.

Why “Satellite”? Well, like real satellites, satellite variables are remote and orbit somewhere “up” the call stack.  

