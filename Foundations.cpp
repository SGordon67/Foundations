// Scott Gordon
// CS 3040 Foundations of Computer Science

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

	void print() {
		std::string a;
		std::cout << this->c.c;
		this->s->print();
	}
};

// task 3 function
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
	function<bool(State)> Q;			// States
	vector<Char> v;						// Alphabet
	State q0;							// Start state
	function<State(State, Char)> Delta;	// Delta function
	function<bool(State)> F;			// Accepting states

	// standard constructor for a DFA
	DFA(function<bool(State)> Q, 
		vector<Char> v, 
		State q0, 
		function<State(State, Char)> Delta, 
		function<bool(State)> F){
		this->Q = Q;
		this->v = v;
		this->q0 = q0;
		this->Delta = Delta;
		this->F = F;
	};

	// constructor for DFA that only takes given char
	DFA(char tChar) {
		this->Q = ([=](int qi) { return qi == 0 || qi == 1 || qi == 2; });
		this->q0 = 0;
		this->Delta = ([=](int qi, Char c) {
			if (qi == 0) {
				if (c.c == tChar) {
					return 1;
				}
				else return 2;
			}
			else if (qi == 1) {
				if (c.c == tChar) {
					return 1;
				}
				else return 2;
			}
			else return 2; });
		this->F = (	[=](int qi) { return qi == 1; });
	}

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
	// accepts function but with output on every step
	bool trace(String* inputString) {
		State qi = this->q0;
		String* temp = inputString;
		//cout << qi;			// first state
		while (temp->isEmpty() == false) {
			cout << qi;
			qi = Delta(qi, temp->fChar());
			temp = temp->next();
			
		}
		return F(qi);
	}

	// accepts function, returns true if
	// the given string is accepted by the calling DFA
	bool accepts(String* inputString) {
		State qi = this->q0;
		String* temp = inputString;

		while (temp->isEmpty() == false) {
			qi = Delta(qi, temp->fChar());
			temp = temp->next();
		}
		return F(qi);
	}
};

// function for returning the compliment of a given DFA
template <class State>
DFA<State>* complimentDFA(DFA<State>* inputDFA) {

	return new DFA<State>(inputDFA->Q,
		inputDFA->v,
		inputDFA->q0,
		inputDFA->Delta,
		[=](State a) {
			if (inputDFA->F(a) == true) {
				return false;
			} return true;
		}
		);
}

// function that returns the union of two given DFA's
template <class State>
DFA<pair<State, State>>* unionDFA(DFA<State>* dfa1, DFA<State>* dfa2)
{
	/*
	std::list<myChar> a = dfa1.alphabet;
	std::list<myChar> b = dfa2.alphabet;
	a.insert(a.end(), b.begin(), b.end()); // combines the alphabets
	*/
	return new DFA<pair<State, State>>(
		[=](pair<State, State> a) -> bool { // function for possible states
		return (dfa1->Q(a.first) && dfa2->Q(a.second));// possible states
	},
		dfa1->v,// alphabet
		make_pair(dfa1->q0, dfa2->q0),// start state
		[=](pair<State, State> a, Char b) -> pair<State, State> {
		return (make_pair(dfa1->Delta(a.first, b), dfa2->Delta(a.second, b)));
	},
		[=](pair<State, State> a) { // accept states
		return ((dfa1->F(a.first)) || (dfa2->F(a.second)));
	});
}

// creates a DFA that is the intersection of dfa1 and dfa2
template <class State>
DFA<std::pair<State, State>>* intersectionDFA(DFA<State>* dfa1, DFA<State>* dfa2)
{
	/*
	list<myChar> a = dfa1.alphabet;
	list<myChar> b = dfa2.alphabet;
	a.insert(a.end(), b.begin(), b.end()); // combine alphabets
	*/
	return new DFA<std::pair<State, State>>(
		[=](std::pair<State, State> a) -> bool { 
		return (dfa1->Q(a.first) && dfa2->Q(a.second));
	},
		dfa1->v,// alphabet
		make_pair(dfa1->q0, dfa2->q0),
		[=](pair<State, State> a, Char b) -> pair<State, State> {
		return (make_pair(dfa1->Delta(a.first, b), dfa2->Delta(a.second, b)));
	},
		[=](std::pair<State, State> a) {                    // accept states
		return ((dfa1->F(a.first)) && (dfa2->F(a.second))); // only difference from unionDFA function
	});
}

// returns true/false to indicate 
// the calling DFA is a subuset of dfa2
template <class State>
bool subsetDFA(DFA<State> dfa1, DFA<State> dfa2)
{
	DFA<State> dfa3 = intersectionDFA(dfa1, complementDFA(dfa2));
}

// returns true/false to indicate 
// the calling DFA is a equal to dfa2
template <class State>
bool equalityDFA(DFA<State> dfa1, DFA<State> dfa2)
{
	DFA<State> dfa3 = unionDFA(intersectionDFA(dfa1, complementDFA(dfa2)), intersectionDFA(complementDFA(dfa1), dfa2));
}

// function for testing the input of a DFA
template <class State>
void testDFA(DFA<State>* inputDFA, String* inputString, bool valid) {
	
	cout << "testing with: ";
	inputString->print();
	cout << endl;
	cout << "DFA gives: " << inputDFA->accepts(inputString) << "\tshould give: " << valid << endl;
}

int main()
{

	vector<Char> binary{ Char('0'), Char('1') };
	vector<Char> ternary{ Char('0'), Char('1'), Char('2') };
	vector<Char> char4{ Char('0'), Char('1'), Char('2'), Char('3') };
	vector<Char> name{ Char('s'), Char('c'), Char('o'), Char('t') };
	vector<Char> decimal{ Char('0'), Char('1'), Char('2'), Char('3'), Char('4'), Char('5'), Char('6'), Char('7'), Char('8'), Char('9') };

	/* test area for task 3 start */
	int m = 26;
	int n = 22;
	String* tester = nString(m, binary);
	String* tester2 = nString(n, binary);
	std::cout << "\n(2 char) nstring test with m = " << m << ": ";
	tester->print();
	std::cout << "\n(2 char) nstring test with n = " << n << ": ";
	tester2->print();

	int o = 21;
	int p = 38;
	String* testerb = nString(o, ternary);
	String* testerb2 = nString(p, ternary);
	std::cout << "\n(3 char) nstring test with o = " << o << ": ";
	testerb->print();
	std::cout << "\n(3 char) nstring test with p = " << p << ": ";
	testerb2->print();

	int q = 21;
	int r = 51;
	String* testerc = nString(q, char4);
	String* testerc2 = nString(r, char4);
	std::cout << "\n(4 char) nstring test with q = " << q << ": ";
	testerc->print();
	std::cout << "\n(4 char) nstring test with r = " << r << ": ";
	testerc2->print();
	std::cout << std::endl;
	/* test area for task 3 end */


	//***********Area to write down different DFA examples**********//

	// DFA example that accepts no strings
	auto noAccept =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			binary,
			0,
			[](int qi, Char c) { return 0; },
			[](int qi) { return qi == 1; });

	// DFA example that only accepts the empty string
	auto mtAccept =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			binary,
			0,
			[](int qi, Char c) {
		if (qi == 0) {
			return 1;
		}},
			[](int qi) { return qi == 0; });

	// DFA for even length string
	auto evenL =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			binary,
			0,
			[](int qi, Char c) {
		if (qi == 0) { return 1; }
		else { return 0; }},
			[](int qi) { return qi == 0; });

	// DFA for even number (binary)
	auto evenN =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			binary,
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
			name, // {s,c,o,t}
			0,
			[](int qi, Char c) {
		switch (qi)
		{
		case 0:
			if (c.c == 's') {
				return 1;
			}
			else return 0;
			break;
		case 1:
			if (c.c == 'c') {
				return 2;
			}
			else return 0;
			break;
		case 2:
			if (c.c == 'o') {
				return 3;
			}
			else return 0;
			break;
		case 3:
			if (c.c == 't') {
				return 4;
			}
			else return 0;
			break;
		case 4:
			if (c.c == 't') {
				return 5;
			}
			else return 0;
			break;
		case 5:
			return 5;
			break;
		default:
			return 0;
		}},
			[](int qi) { return qi == 5; });


	// DFA that only accepts the given character
	auto onlyOne = new DFA<int>('A');

	// DFA for odd numbers (compliment of even numbers)
	auto oddN = complimentDFA(evenN);

	// DFA for odd length strings
	auto oddL = complimentDFA(evenL);
	
	// DFA that should accept even number OR even lengthed strings
	auto unionTest = unionDFA(evenN, evenL);

	// DFA that should accept strings that are even length AND even numbers
	auto intersectionTest = intersectionDFA(evenN, evenL);

	//************************End of DFA section********************//

	OneString* test1 = new OneString(Char('0'), new epsilon());
	epsilon* test2 = new epsilon();
	OneString* test3 = new OneString(Char('A'), new OneString(Char('A'), new epsilon()));
	OneString* nameString = new OneString(Char('s'), new OneString(Char('c'), new OneString(Char('o'), new OneString(Char('t'), new OneString(Char('t'), new epsilon())))));
	OneString* test4 = new OneString(Char('1'), new OneString(Char('0'), new epsilon()));
	OneString* test5 = new OneString(Char('0'), new OneString(Char('1'), new epsilon()));
	OneString* test6 = new OneString(Char('0'), new OneString(Char('1'), new OneString(Char('1'), new epsilon())));
	OneString* test7 = new OneString(Char('0'), new OneString(Char('1'), new OneString(Char('0'), new epsilon())));
	
	cout << "\n\t\ttesting evenL:\n";
	testDFA(evenL, test1, false);	// Only one char, flase
	testDFA(evenL, test2, true);	// Zero characters, true
	testDFA(evenL, test3, true);	// Two characters, true
	testDFA(evenL, test4, true);
	testDFA(evenL, test5, true);
	testDFA(evenL, test6, false);
	testDFA(evenL, test7, false);
	cout << endl;

	cout << "\t\ttesting with evenN:\n";
	testDFA(evenN, test1, true);	// 0 is even
	testDFA(evenN, test2, true);	// Empty is considered even for this example
	testDFA(evenN, test3, false);	// Non number inputs are not even by my defintion
	testDFA(evenN, test4, true);
	testDFA(evenN, test5, false);
	testDFA(evenN, test6, false);
	testDFA(evenN, test7, true);
	cout << endl;
	
	cout << "\t\ttesting with oddL:\n";
	testDFA(oddL, test1, true);
	testDFA(oddL, test2, false);
	testDFA(oddL, test3, false);
	testDFA(oddL, test4, false);
	testDFA(oddL, test5, false);
	testDFA(oddL, test6, true);
	testDFA(oddL, test7, true);
	cout << endl;

	cout << "\t\ttesting with oddN:\n";
	testDFA(oddN, test1, false);
	testDFA(oddN, test2, false);
	testDFA(oddN, test3, true);		// any non number input is 'odd'
	testDFA(oddN, test4, false);
	testDFA(oddN, test5, true);
	testDFA(oddN, test6, true);
	testDFA(oddN, test7, false);
	cout << endl;

	
	cout << "\t\ttesting with noAccept:\n";
	testDFA(noAccept, test1, false);
	testDFA(noAccept, test2, false);
	testDFA(noAccept, test3, false);
	cout << endl;

	cout << "\t\ttesting with mtAccept:\n";
	testDFA(mtAccept, test1, false);
	testDFA(mtAccept, test2, true);
	testDFA(mtAccept, test3, false);
	cout << endl;

	cout << "\t\ttesting with onlyOne:\n";
	testDFA(onlyOne, test1, false);
	testDFA(onlyOne, test2, false);
	testDFA(onlyOne, test3, true);
	cout << endl;

	cout << "\t\ttesting with nameDFA:\n";
	testDFA(nameDFA, test1, false);
	testDFA(nameDFA, test2, false);
	testDFA(nameDFA, test3, false);
	testDFA(nameDFA, nameString, true);
	cout << endl;
	
	cout << "\t\ttesting with unionTest:\n";
	testDFA(unionTest, test1, true);
	testDFA(unionTest, test4, true);
	testDFA(unionTest, test5, true);
	testDFA(unionTest, test6, false);
	testDFA(unionTest, test7, true);
	cout << endl;

	cout << "\t\ttesting with intersectionTest:\n";
	testDFA(intersectionTest, test1, false);
	testDFA(intersectionTest, test4, true);
	testDFA(intersectionTest, test5, false);
	testDFA(intersectionTest, test6, false);
	testDFA(intersectionTest, test7, false);	
	cout << endl;

	nameDFA->trace(nameString); // States 0-1-2-3-4 
	//unionTest->trace(test7);	// trace the union of two DFA's
}