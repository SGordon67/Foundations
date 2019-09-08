#include <iostream>
#include <list>
#include <string>
#include <iterator>

class Object {
public:
	Object() {}
	Object(std::string x) {this->o = x;}
	std::string o;
};
class string {
};
class OneString {
public:
	OneString() {}
	OneString(Object x, OneString* y) {
		this->a = x;
		this->b = y;
	}
	Object a;
	OneString* b;
};
class MTString : public OneString {
public:
	MTString() {}
};

int main()
{
	std::list<Object> Alphabet; // The Alphabet
	MTString mt = MTString();	// Empty String

	Object sigma = Object("sigma");
	Object zero = Object("0");	// characters
	Object one = Object("1");

	Alphabet.push_back(zero);
	Alphabet.push_back(one);

	std::cout << "The Alphabet: \n"; // Outputting the alphabet
	for (auto v : Alphabet) {
		std::cout << v.o << "\n";
	}

	// iterate through the alphabet to grab characters 
	std::list<Object>::iterator it = Alphabet.begin();
	Object temp = *it;
	std::cout << "first element: " << temp.o << "\n";
	// grab a specific element of the list
	std::list<Object>::iterator it1 = std::next(Alphabet.begin(), 1);
	temp = *it1;
	std::cout << "Second element: " << temp.o << "\n";

	// outputting a specific string 
	OneString OIO = OneString(zero, new OneString(one, new OneString(zero, new MTString())));
	std::cout << "OIO string: \n";
	std::cout << OIO.a.o << "\n" <<  OIO.b->a.o << "\n" << OIO.b->b->a.o << "\n";
	
}