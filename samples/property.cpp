#include <cutty/property.hpp>
#include <cutty/check.hpp>
#include <string>
#include <stdexcept>

namespace cy = cutty;

// This is a class that contains properties.
class Person
{
public:

	// This is a property named  "Age", in struct "Person", of type "int".
	CY_PROPERTY(Person, Age, int);

	// These are readonly properties.
	CY_GETTER(Person, Adult, bool);
	CY_GETTER(Person, YearsToRetirement, int);

protected:
	// This is a protected property just because.
	CY_PROPERTY(Person, Name, const char *);

private:
	// This is the backing field for the Age property.
	int age = 0;

	// This is a backing field for the Name property.
	std::string name;

	// This is the getter for the Age property.
	// It must have the name get_XXX, where XXX is the name of the property.
	// The getter must be const.
	int get_Age() const { return age; }

	// This is the setter of the Age property.
	// It has the name set_XXX
	// This setter adds validation of the property value.
	void set_Age(int a) { 
		if (a < 0 || a>150) throw std::runtime_error("Invalid age");
		age = a;
	}

	// This is the getter of the Adult property.
	// This shows that the class can use its own properties (i.e. Age).
	bool get_Adult() const { return Age >= 18; }

	// This is the getter of the Name property.
	// The property type can have a different type to its underlying field.
	// In this case, the string is stored in std::string as an implementation detail.
	const char * get_Name() const { return name.c_str(); }

	// This is the setter of the Name property.
	void set_Name(const char * n) { name = n; }

	// This is a virtual getter.
	virtual int get_YearsToRetirement() const { return 65 - Age; }
};


int main()
{
	// Create a person
	Person p;

	// Assign 69 to the age, via the Age property.
	p.Age = 69;

	// Read the Adult property.
	cy::check(p.Adult);

	// Assign 17 to the Age property.
	p.Age = 17;
	cy::check(!p.Adult);
	cy::check(p.YearsToRetirement == (65 - 17));

	// Copying a person copies the underlying field
	// but does not call any getters or setters.
	// In fact the copying and assignment operators of class Person
	// do not affect the properties.
	auto p2 = p;
	cy::check(p2.Age == 17);

	// If you need to assign between properties, use the * operator
	p.Age = *p2.Age;
	return 0;
}