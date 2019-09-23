#include <iostream>
#include <list>
#include <string>
#include <cmath>

class Char {
public:
	char c;
	Char() { this->c = 'n'; }
	Char(char x) {this->c = x;}
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
public:
	Char c;
	String* s;
	OneString() {
		this->c = Char('n');
		this->s = new epsilon();
	}
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

class Alphabet {
public:
	virtual int size() { return 0; }
	virtual Char index(int i) { return 0; }
	virtual String* nString(int i) { return 0; }
};

class EmptyAlphabet : public Alphabet {
public:
	EmptyAlphabet() {}
	int size() { return 0; }
	Char index(int i) { 
		if (i == 0) { return(Char('E')); }
		else std::cout << "Exception here!\n";
	}
	String* nString(int i) {
		if (i == 0) { return (new epsilon()); }
		else throw new _exception;
	}
};

class SingleAlphabet : public Alphabet {
	Char c;
	Alphabet* a;
public:
	SingleAlphabet(Char c, SingleAlphabet* a) {
		this->c = c;
		this->a = a;
	}
	SingleAlphabet(Char c, Alphabet* a) {
		this->c = c;
		this->a = a;
	}
	int size() {
		return 1 + (a->size());
	}
	Char index(int i) {
		if (i == 0) { return c; }
		else return(a->index(i - 1));
	}
	String* nString(int in) {
		double z = in; // index of the desired string
		double x = this->size(); // number of characters in the alphabet
		double p = 0;	// max power of x so that (x^p < z)
					// also the length of the desired string

		double a = 0; // index of the desired string in the list of Strings of the correct length

		// setting p to the correct power
		for (double i = 0; i < 10; i++) {
			if (std::pow(x, i) > z) {
				p = (i-1);
				break;
			}
		}

		// getting the correct index of the set of strings of correct length
		int temp = pow(x, p);
		int sum = 0;
		for (int i = p; i >= 0; i--) {
			sum += pow(x, i);
		}
		a = temp - (sum - z);

		std::cout << " z = " << z << "\n";
		std::cout << " a = " << a << "\n";
		std::cout << " p = " << p << "\n";
		std::cout << " x = " << x << "\n";

		OneString* ret1 = new OneString();
		OneString* ret = ret1;
		int append = 0;
		int tester = ((pow(x, p)) / x);


		for (int i = 0; i < p; i++) {
			for (double j = 1; j <= x+1; j++) {
				std::cout << "testing condition: \n";
				std::cout << "a: " << a << "\n";
				std::cout << "tester: " << (j * tester) << "\n";
				if (a <= (j * tester)) {
					append = (j - 1);
					tester = ((j * tester) / x);
					std::cout << "append in loop: " << append << "\n";
					break;
				}
				else { 

				}
			}
			std::cout << "append post loop: " << append << "\n";
			ret->c = this->index(append);
			std::cout << "appending to string: " << this->index(append).c << "\n";



			std::cout << "after inner for loop\n\n";
			if (i != p - 1) {
				OneString* temp = new OneString();
				ret->s = temp;
				ret = temp;
			}
		}
		return ret1;
		/*
		// getting the correct OneString object
		switch (x)
		{
		case 1: //1 character in the alphabet
			break;
		case 2: // 2 characters in the alphabet
			if (p == 0) {
				return new epsilon();
			}
			else if (p == 1) {
				OneString* ret = new OneString(this->index(a), new epsilon());
				std::cout << "here and it is: " << this->index(a).c;
				return ret;
			}
			else if (p == 2) {
				if (a < (temp/2)) {

				}
			}
			break;
		case 3:
			break;
		default: // code to be executed if n doesn't match any cases
			return new epsilon();
		}
		*/
		return new epsilon(); // filler so I can compile
	}
};

int main()
{
	// 3 char alphabet
	SingleAlphabet a = SingleAlphabet(Char('0'), new SingleAlphabet(Char('1'),  new SingleAlphabet(Char('2'), new EmptyAlphabet())));

	// binary ( 2 char )
	//SingleAlphabet a = SingleAlphabet(Char('0'), new SingleAlphabet(Char('1'), new EmptyAlphabet()));

	/* test area start */
	String* tester = a.nString(11);
	tester->print();
	/* test area end */

	// Test code for char, string, and print function
	Char test = Char('t');
	Char test2 = Char('e');
	Char test3 = Char('s');
	epsilon e = epsilon();

	OneString testString = OneString(test, new OneString(test2,  new OneString(test3, new OneString(test, &e))));

	testString.print();
}
