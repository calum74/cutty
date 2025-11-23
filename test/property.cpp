
#include <cutty/property.hpp>
#include <cutty/check.hpp>

#include <string>
#include <iostream>

namespace cy = cutty;

struct PropertySizes
{
	CY_GETTER(PropertySizes, P1, int);
	CY_GETTER(PropertySizes, P2, int);
	CY_GETTER(PropertySizes, P3, int);
	int get_P1() const { return 0; }
	int get_P2() const { return 0; }
	int get_P3() const { return 0; }
};

class PersonWithoutProperties
{
public:
	const std::string & get_Name() const { return name; }
	void set_Name(const std::string&value) { name = value; }
private:
	std::string name;
};

class PersonWithProperties : public PersonWithoutProperties
{
public:
	CY_PROPERTY(PersonWithProperties, Name, const std::string&);
};

class Foo
{
public:
	CY_PROPERTY(Foo, Foo1, int);
	CY_PROPERTY(Foo, Foo2, std::string);
	std::string name;
public:
	int get_Foo1() const { return 1; }
	void set_Foo1(int) { }
	std::string str;
	const std::string & get_Foo2() const { return str; }
	void set_Foo2(const std::string &s) { str = s; }
};

int main()
{
	// Tests the size of the property.
	// This is compiler-specific, however in practice, each property
	// will be allocated 1 byte somewhere in the type.
	{
		// This isn't portable but records what happens in practise.
		cy::check(sizeof(PropertySizes::P1) == 1);
		cy::check(sizeof(PropertySizes) == 3);
	}

	// Tests an integer getter
	{
		struct IntGetter
		{
			CY_GETTER(IntGetter, P, int);
			int get_P() const { return 10; }
		} p;

		cy::check(p.P == 10);
		cy::check((int)p.P == 10);
		cy::check(*p.P == 10);
		cy::check(p.P.Get() == 10);

		// These are compilation errors:
		// auto prop1 = p.P;
		// decltype(p.P) prop2;
	}

	// Tests an integer property
	{
		struct IntProperty
		{
			CY_PROPERTY(IntProperty, P, int);
		private:
			int get_P() const { return p; }
			void set_P(int value) { p = value; }
		private:
			int p;
		} p;

		p.P = 10;
		cy::check(p.P == 10);
		cy::check((int)p.P == 10);
		cy::check(*p.P == 10);
		cy::check(p.P.Get() == 10);

		p.P.Set(30);
		cy::check(p.P == 30);

		// These are compilation errors:
		// auto prop1 = p.P;
		// decltype(p.P) prop2;
	}

	// Tests const std::string & property.
	{
		PersonWithProperties p;
		cy::check(*p.Name == "");
		p.Name = "Fred";
		cy::check(*p.Name == "Fred");

		// Tests the copy constructor
		auto p2 = p;
		cy::check(*p2.Name == "Fred");  // The underlying field was copied.

		p2.Name = "Alf";
		p = p2;
		cy::check(*p.Name == "Alf");  // The underlying field was assigned
	}

	// Tests a setter
	{
		struct Setting
		{
			CY_SETTER(Setting, Stop, bool);
			void set_Stop(bool value) { stopped = value; }
			bool stopped;
		} s;

		s.Stop = true;
		cy::check(s.stopped);
		s.Stop = false;
		cy::check(!s.stopped);
	}

	std::cout << "Tests passed\n";
	return 0;
}
