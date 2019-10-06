#include <iostream>
#include <list>
#include <string>
#include <cmath>
#include <functional>
#include <utility>
#include <vector>

using namespace std;

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
	Char ep = Char('E');
	epsilon() {}
	bool isEmpty() { return true; }
	Char fChar() { return ep; }
	String* next() { throw new _exception; }
	void print() {  }
};

class OneString : public String{
public:
	Char c;
	String* s;
	OneString() {
		this->c = Char('n');
		this->s = new epsilon();
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

	/*bool equals(OneString a) {
		OneString* temp = this;

		if (this->c.c == a.c.c) {
			temp = temp->next();
			a = a.next();

		}
	}*/

	void print() {
		std::string a;
		std::cout << this->c.c;
		this->s->print();
	}
};

/*
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
public:
	Char c;
	Alphabet* a;
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
		// deal with the trivial cases
		if (in == 1) {
			return new epsilon();
		}
		else if (in == 2) {
			return new OneString(Char(this->index(0)), new epsilon());
		}

		double z = in; // index of the desired string
		double x = this->size(); // number of characters in the alphabet
		double p = 0;	// max power of x so that (x^p < z)
						// also the length of the desired string
		double a = 0; // index of the desired string in the list of Strings of the correct length

		// setting p to the correct power
		int tracker = 1;
		for (double i = 0; i < 10; i++) {
			if (tracker > z) {
				p = (i - 1);
				break;
			}
			tracker += std::pow(x, i);
		}

		// getting the correct index of the set of strings of correct length
		int temp = pow(x, p);
		int sum = 0;
		for (int i = p; i >= 0; i--) {
			sum += pow(x, i);
		}
		a = temp - (sum - z);

		// creating the returning string and a pointer to it
		// in order to change the values.
		OneString* ret1 = new OneString();
		OneString* ret = ret1;
		// vars i'll need for convenience
		int append = 0;
		int tester = (pow(x, p));

		// loop for determining and creating the correct string
		for (int i = 0; i <= p - 1; i++) {
			for (double j = 1; j <= x+1; j++) {
				if (a <= (j * (tester / x))) {
					append = (j - 1);
					a = a - (((j - 1) * tester) / x);
					tester = ((tester) / x);
					break;
				}
			}
			ret->c = this->index(append);

			// moving the pointer forward in the string 
			// and 'saving' the information
			if (i != p - 1) {
				OneString* temp = new OneString();
				ret->s = temp;
				ret = temp;
			}
		}
		// return the constructed string
		return ret1;
	}
};
*/

String* nString(int in, vector<Char> myVector) {
	// deal with the trivial cases
	if (in == 1) {
		return new epsilon();
	}
	else if (in == 2) {
		return new OneString(Char(myVector[0]), new epsilon());
	}

	double z = in; // index of the desired string
	double x = myVector.size(); // number of characters in the alphabet
	double p = 0;	// max power of x so that (x^p < z)
					// also the length of the desired string
	double a = 0; // index of the desired string in the list of Strings of the correct length

	// setting p to the correct power
	int tracker = 1;
	for (double i = 0; i < 10; i++) {
		if (tracker > z) {
			p = (i - 1);
			break;
		}
		tracker += std::pow(x, i);
	}

	// getting the correct index of the set of strings of correct length
	int temp = pow(x, p);
	int sum = 0;
	for (int i = p; i >= 0; i--) {
		sum += pow(x, i);
	}
	a = temp - (sum - z);

	// creating the returning string and a pointer to it
	// in order to change the values.
	OneString* ret1 = new OneString();
	OneString* ret = ret1;
	// vars i'll need for convenience
	int append = 0;
	int tester = (pow(x, p));

	// loop for determining and creating the correct string
	for (int i = 0; i <= p - 1; i++) {
		for (double j = 1; j <= x + 1; j++) {
			if (a <= (j * (tester / x))) {
				append = (j - 1);
				a = a - (((j - 1) * tester) / x);
				tester = ((tester) / x);
				break;
			}
		}
		ret->c = myVector[append];

		// moving the pointer forward in the string 
		// and 'saving' the information
		if (i != p - 1) {
			OneString* temp = new OneString();
			ret->s = temp;
			ret = temp;
		}
	}
	// return the constructed string
	return ret1;
}

// DFA Class
template <class State>
class DFA{
public:
	function<bool(State)> Q;
	vector<Char> v;
	State q0;
	function<State(State, Char)> Delta;
	function<bool(State)> F;

	DFA(function<bool(State)> Q, vector<Char> v, State q0, function<State(State, Char)> Delta, function<bool(State)> F){
		this->Q = Q;
		this->v = v;
		this->q0 = q0;
		this->Delta = Delta;
		this->F = F;
	};

	// constructor for DFA that only takes given char
	DFA(char tChar) {
		this->Q = ([=](int qi) { return qi == 0 || qi == 1; });
		this->q0 = 0;
		this->Delta = ([=](int qi, Char c) {
			if (qi == 0) {
				if (c.c == tChar) {
					return 0;
				}
				else return 1;
			}});
		this->F = (	[=](int qi) { return qi == 0; });
	}

	/*
	// constructor for DFA union
	DFA<pair<State, State>>  unionDFA(DFA<State> dfa1, DFA<State> dfa2){

		// combine alphabets

		// return dfa
		return 
		this->Q = ([=](int qi) { return qi == 0 || qi == 1; });
		this->q0 = 0;
		this->Delta = ([=](int qi, Char c) {
			if (qi == 0) {
				if (c.c == tChar) {
					return 0;
				}
				else return 1;
			}});
		this->F = ([=](int qi) { return qi == 0; });
	}*/

	
	// return pair of stack and bool

	// stack will be a

	// find an accepted string within the DFA
	auto acceptedString() {
		vector<State> visitedStates;
		State qi = q0;
		//if (F(qi)) return new epsilon();

		return(this->pAccept(qi, visitedStates));
	}
	auto pAccept(State qi, vector<State> &visitedStates) {
		if (F(qi)) { 
			return true; 
		}

		for (int i = 0; i < visitedStates.size(); i++) {
			if (qi == visitedStates[i]) {
				return false;
			}
		}
		visitedStates.push_back(qi);

		for (int i = 0; i < v.size(); i++) {
			if (pAccept(Delta(qi, v[i]), visitedStates)) {
				cout << v[i].c;
				return true;
			}
		}
		return false;
	}

	// function to trace the tree given a string
	bool trace(String* inputString) {
		State qi = this->q0;
		String* temp = inputString;
		cout << qi;
		while (temp->fChar().c != 'E') {
			cout << qi;
			qi = Delta(qi, temp->fChar());
			temp = temp->next();
			
		}
		return F(qi);
	}

	// function that is given a char, 
	// returns a dfa that returns strings with that char
	bool accepts(String* inputString) {
		State qi = this->q0;
		String* temp = inputString;

		while (temp->fChar().c != 'E') {
			qi = Delta(qi, temp->fChar());
			temp = temp->next();
		}
		return F(qi);
	}
};

// function for returning the cimpliment of a given DFA
template <class State>
DFA<State> complimentDFA(DFA<State> inputDFA) {

	return DFA<State>(inputDFA.Q,
		inputDFA.v,
		inputDFA.q0,
		inputDFA.Delta,
		[=](State a) {
			if (inputDFA.F(a) == true) {
				return false
			} return true;
		}
		);
}

int main()
{
	/*
	// binary (2 char)
	SingleAlphabet a = SingleAlphabet(Char('0'), new SingleAlphabet(Char('1'), new EmptyAlphabet()));
	// 3 char alphabet
	SingleAlphabet b = SingleAlphabet(Char('0'), new SingleAlphabet(Char('1'), new SingleAlphabet(Char('2'), new EmptyAlphabet())));
	// 4 char alphabet
	SingleAlphabet c = SingleAlphabet(Char('0'), new SingleAlphabet(Char('1'), new SingleAlphabet(Char('2'), new SingleAlphabet(Char('3'), new EmptyAlphabet()))));
	*/

	vector<Char> a{ Char('0'), Char('1') };
	vector<Char> b{ Char('0'), Char('1'), Char('2') };
	vector<Char> c{ Char('0'), Char('1'), Char('2'), Char('3') };
	vector<Char> name{ Char('s'), Char('c'), Char('o'), Char('t') };

	/* test area start */
	int m = 26;
	int n = 22;
	String* tester = nString(m, a);
	String* tester2 = nString(n, a);
	std::cout << "\n(2 char) nstring test with m = " << m << ": ";
	tester->print();
	std::cout << "\n(2 char) nstring test with n = " << n << ": ";
	tester2->print();

	int o = 21;
	int p = 38;
	String* testerb = nString(o, b);
	String* testerb2 = nString(p, b);
	std::cout << "\n(3 char) nstring test with o = " << o << ": ";
	testerb->print();
	std::cout << "\n(3 char) nstring test with p = " << p << ": ";
	testerb2->print();

	int q = 21;
	int r = 51;
	String* testerc = nString(q, c);
	String* testerc2 = nString(r, c);
	std::cout << "\n(4 char) nstring test with q = " << q << ": ";
	testerc->print();
	std::cout << "\n(4 char) nstring test with r = " << r << ": ";
	testerc2->print();
	std::cout << std::endl;
	/* test area end */

	// DFA example that accepts no strings
	auto noAccept =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			a,
			0,
			[](int qi, Char c) { return 0; },
			[](int qi) { return qi == 1; });

	// DFA example that only accepts the empty string
	auto mtAccept =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			a,
			0,
			[](int qi, Char c) {
		if (qi == 0) {
			if (c.c == 'E') {
				return 0;
			}
			else return 1;
		}},
			[](int qi) { return qi == 0; });

	// DFA for even length string
	auto evenL =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			a,
			0,
			[](int qi, Char c) {
		if (qi == 0) { return 1; }
		else { return 0; }},
			[](int qi) { return qi == 0; });

	// DFA for even number (binary)
	auto evenN =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			a,
			0,
			[](int qi, Char c) {
		if (qi == 0) {
			if (c.c == '0') { return 0; }
			else return 1;
		}
		else if (c.c == '0') { return 0; }
		else return 1;
	},
			[](int qi) { return qi == 0; });

	// DFA that accepts my name kinda
	auto nameDFA =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1 || qi == 2 || qi == 3 || qi == 4 || qi == 5; },
			name,
			0,
			[](int qi, Char c) {
		if (qi == 0) {
			if (c.c == 's') {
				return 1;
			}
			else return 0;
		}
		else if (qi == 1) {
			if (c.c == 'c') {
				return 2;
			}
			else return 0;
		}
		else if (qi == 2) {
			if (c.c == 'o') {
				return 3;
			}
			else return 0;
		}
		else if (qi == 3) {
			if (c.c == 't') {
				return 4;
			}
			else return 0;
		}
		else if (qi == 4) {
			if (c.c == 't') {
				return 5;
			}
			else return 0;
		}
		else if (qi == 5) {
			return 5;
		}
		else { return 0; }},
			[](int qi) { return qi == 5; });

	//DFA onlyOne = evenN->charDfa(ctemp);
	auto onlyOne = new DFA<int>('A');


	OneString* test1 = new OneString(Char('0'), new epsilon());
	epsilon* test2 = new epsilon();
	OneString* test3 = new OneString(Char('A'), new OneString(Char('A'), new epsilon()));

	cout << evenL->accepts(test1) << " should be " << false << endl;

	cout << evenN->accepts(test1) << " should be " << true << endl;

	cout << noAccept->accepts(test1) << " should be " << false << endl;
	
	cout << mtAccept->accepts(test1) << " should be " << false << endl;
	cout << mtAccept->accepts(test2) << " should be " << true << endl;

	cout << onlyOne->accepts(test3) << " should be " << true << endl;

	cout << noAccept->acceptedString() << " should be " << false << endl;
	cout << nameDFA->acceptedString() << " should be " << true << endl;

	onlyOne->trace(test3);
}