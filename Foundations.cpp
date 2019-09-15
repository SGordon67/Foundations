#include <iostream>
#include <list>
#include <string>


class Char {
public:
	char c;
	Char() { this->c = 'n'; };
	Char(char x) {this->c = x;}
};

class Alphabet {
public:
	virtual int size() { return 0; }
	virtual Char index(int i) { return 0; }
};

class EmptyAlphabet : public Alphabet {
public:
	EmptyAlphabet() {}
	int size() { return 0; }
	Char index(int i) { return 0; }
};

class SingleAlphabet : public Alphabet {
	Char c;
	Alphabet a;
public:
	SingleAlphabet(Char c, Alphabet a) {
		this->c = c;
		this->a = a;
	}
	int size() {
		return 1 + a.size();
	}
	Char index(int i) {
		if (i == 0) { return c; }
		else return( a.index(i - 1) );
	}
};

class String {
public:
	String() {}
	virtual bool isEmpty() { return true; }
	virtual Char fChar() { return (Char('n')); }
	virtual String* next() { return (&String()); }
};

class epsilon : public String {
public:
	epsilon() {}
	bool isEmpty() { return true; }
	Char fChar() { throw new _exception; }
	String* next() { throw new _exception; }
};

class OneString : public String{
	Char c;
	String* s;
public:
	OneString(Char c, OneString* s) {
		this->c = c;
		this->s = s;
	}
	OneString(Char c, String* s) {
		this->c = c;
		this->s = s;
	}
	bool isEmpty() {
		return false;
	}
	Char fChar() { return this->c; }
	String* next() { return this->s; }

	void print() {
		std::string a;

		while ( this->isEmpty() == false ) {
			//std::cout << "im here";
			a.push_back(this->c.c);
			a.push_back(this->next()->fChar().c);
			break;
		}
		std::cout << a;
		//std::cout << this->c.c;
	}
};


/*
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
*/


int main()
{
	Char test = Char('t');
	Char test2 = Char('e');
	Char test3 = Char('s');

	epsilon e = epsilon();
	OneString testString = OneString(test, &OneString(test2,  &e));

	testString.print();

	/*
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
	*/
}
