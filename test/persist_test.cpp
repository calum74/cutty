#include <cutty/check.hpp>
#include <cutty/persist.hpp>
#include <string>
#include <vector>

namespace cy = cutty;

class TestPersist
{
  public:
    TestPersist()
    {
        DefaultConstructor();
        EmptyFile();
        Versions();
        TestData();
        TestLimits();
        TestModes();
        TestAllocators();
    }

    void DefaultConstructor()
    {
        persist::map_file file;
        cy::check(!file);
    }

    void EmptyFile()
    {
        persist::map_file file("file.db", 0, 0, 0, 1000, 1000, persist::create_new);
        cy::check(file);
        cy::check(file.data().empty());
        cy::check(file.data().size() == 0);
    }

    void Versions()
    {
        {
            persist::map_file file("file.db", 0, 0, 0, 1000, 1000, persist::create_new);
        }

        {
            // Reopen no problem
            persist::map_file file("file.db", 0, 0, 0);
        }

        cy::check_throws([]() { persist::map_file file("file.db", 1, 0, 0); }, "Version number mismatch");

        cy::check_throws([]() { persist::map_file file("file.db", 0, 1, 0); }, "Version number mismatch");

        cy::check_throws([]() { persist::map_file file("file.db", 0, 0, 1); }, "Version number mismatch");
    }

    typedef std::basic_string<char, std::char_traits<char>, persist::allocator<char>> pstring;
    typedef std::basic_string<char, std::char_traits<char>, persist::fast_allocator<char>> fstring;

    struct Demo
    {
        int value;

        pstring string1;
        fstring string2;

        std::shared_ptr<int> intptr;
        std::vector<pstring, persist::allocator<pstring>> vec;

        Demo(persist::shared_memory &mem) : string1(mem), string2(mem), vec(persist::allocator<pstring>(mem))
        {
            intptr = std::allocate_shared<int, persist::fast_allocator<int>>(mem, 123);
        }
    };

    void TestData()
    {
        persist::map_file file("file.db", 0, 0, 0);
    }

    void TestLimits()
    {
        {
            persist::map_file file("file.db", 0, 0, 0, 16384, 16384, persist::create_new);
            TestHeapLimit(file.data(), 16384);
        }

        {
            persist::map_file file("file.db", 0, 0, 0, 16384, 65536, persist::create_new);
            TestHeapLimit(file.data(), 65536);
        }

        {
            persist::map_file file(nullptr, 0, 0, 0, 16384, 16384, persist::temp_heap);
            TestHeapLimit(file.data(), 16384);
        }

        {
            persist::map_file file(nullptr, 0, 0, 0, 16384, 65536, persist::temp_heap);
            TestHeapLimit(file.data(), 65536);
        }
    }

    void ValidateMemory(void *p, size_t size)
    {
        char *c = (char *)p;
        for (int i = 0; i < size; ++i)
            c[i] = i;

        for (int i = 0; i < size; ++i)
            cy::check((char)i == c[i]);
    }

    void TestHeapLimit(persist::shared_memory &mem, size_t expected_limit)
    {
        auto initial_capacity = mem.capacity();

        auto p = mem.malloc(mem.capacity() / 2);
        cy::check(p);
        ValidateMemory(p, initial_capacity / 2);
        mem.clear();

        p = mem.malloc(expected_limit);
        cy::check(!p);
        mem.clear();
        cy::check(initial_capacity == mem.capacity());

        p = mem.malloc(mem.capacity() / 2);
        cy::check(p);
        ValidateMemory(p, initial_capacity / 2);
        mem.clear();

        p = mem.fast_malloc(mem.capacity());
        cy::check(p);
        ValidateMemory(p, initial_capacity);
        cy::check(p);
        mem.clear();

        for (int i = 0; i < 8; ++i)
        {
            p = mem.fast_malloc((initial_capacity / 8) & ~7);
            cy::check(p);
        }
    }

    void TestModes()
    {
        {
            persist::map_file file(nullptr, 0, 0, 0, 16384, 16384, persist::temp_heap);
            cy::check(file);

            persist::map_data<Demo> data{file.data(), file.data()};
            data->value = 10;

            bool failed = false;
            for (int i = 0; i < 100; ++i)
            {
                auto p = file.data().malloc(1000);
                if (p)
                    ValidateMemory(p, 1000);
                else
                    failed = true;
            }
            cy::check(failed);
        }

        {
            persist::map_file file("file.db", 0, 0, 0, 16384, 10000, persist::create_new);
            cy::check(file);
            persist::map_data<Demo> data{file.data(), file.data()};
            cy::check(0 == data->value);
            data->value = 10;
        }

        {
            persist::map_file file("file.db", 0, 0, 0, 16384, 10000);
            cy::check(file);
            persist::map_data<Demo> data{file.data(), file.data()};
            cy::check(10 == data->value);
        }
    }

    void TestAllocators()
    {
        persist::map_file file(nullptr, 0, 0, 0, 16384, 1000000, persist::temp_heap);
        cy::check(file);

        persist::map_data<Demo> data{file.data(), file.data()};
    }
} tp;

int main()
{
    return 0;
}
