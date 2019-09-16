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
	virtual void print() {};
};

class epsilon : public String {
public:
	epsilon() {}
	bool isEmpty() { return true; }
	Char fChar() { throw new _exception; }
	String* next() { throw new _exception; }
	void print() { std::cout << 'E'; }
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
		std::cout << this->c.c;
		this->s->print();
	}
};

int main()
{
	Char test = Char('t');
	Char test2 = Char('e');
	Char test3 = Char('s');

	epsilon e = epsilon();
	OneString testString = OneString(test, &OneString(test2,  &OneString(test3, &OneString(test, &e))));

	testString.print();
}
