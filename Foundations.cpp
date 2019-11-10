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

template <class State, class State2>
class myPair {
public:
	State first;
	State2 second;

	//myPair<State, State2>(State mp1, State2 mp2) : first(mp1), second(mp2) {}

	myPair<State, State2>() {}
	myPair<State, State2>(State first, State2 second) {
		this->first = first;
		this->second = second;
	}

	friend ostream& operator<<(ostream& os, const myPair<State, State2>& mp)
	{
		os << mp.first << mp.second << endl;
		return os;
	}

	friend bool operator==(const myPair<State, State2>& lhs, const myPair<State, State2>& rhs)
	{
		return(lhs.first == rhs.first && lhs.second == rhs.second);
	}
};

template<class State, class State2>
myPair<State, State2> my_make_pair(State first, State2 second) {
	return (myPair<State, State2>(first, second));
}

class Char {
public:
	char c;
	Char() { this->c = 'E'; }
	Char(char x) {this->c = x;}
	bool isEmpty() { return false; }
};

class mtChar : public Char {
	char c;
public:
	mtChar() {
		this->c = NULL;
	}
	mtChar(char x) {
		this->c = NULL;
	}
	bool isEmpty() { return true; }
};

class String {
public:
	String() {}
	virtual bool isEmpty() { return true; }
	virtual Char fChar() { return (Char('E')); }
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
	void print() {}
};

class OneString : public String{
public:
	Char c;
	String* s;
	OneString() {
		this->c = Char('E');
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

	/*
	1. organize sigma* into 'columns' of the different length strings
	2. find out which 'column your string belongs to
	3. find out the index of your string in that column
	4. calculate the string based on the alphabet and index in that column
	5. construct and return the string
	*/

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

//***************************DFA START***************************//

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
	String* acceptedString() {
		vector<Char> charVec;
		vector<State> visitedStates;
		State qi = q0;
		
		OneString* ret1 = new OneString();
		OneString* ret = ret1;
		if (this->pAccept(qi, visitedStates, charVec)) {
			while ( true ) {
				if (charVec.empty())break;
				ret->c = charVec.back();
				charVec.pop_back();
				if (charVec.empty())break;
				OneString* temp = new OneString();
				ret->s = temp;
				ret = temp;
			}
			return ret1;
		}
		else return new epsilon();
	}
	auto pAccept(State qi, vector<State> &visitedStates, vector<Char> &charVec) {
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
			if (pAccept(Delta(qi, v[i]), visitedStates, charVec)) {
				charVec.push_back(v[i].c);
				//cout << v[i].c;
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

		while (temp->isEmpty() == false) {
			
			//cout << "should be a repeat here";
			cout << qi;	// need to generalize for pairs
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

// function for returning the complement of a given DFA
template <class State>
DFA<State>* complementDFA(DFA<State>* inputDFA) {

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
template <class State, class State2>
DFA<myPair<State, State2>>* unionDFA(DFA<State>* dfa1, DFA<State2>* dfa2)
{
	return new DFA<myPair<State, State2>>(
		[=](myPair<State, State2> a) -> bool { // function for possible states
		return (dfa1->Q(a.first) && dfa2->Q(a.second));// possible states
	},
		dfa1->v,// alphabet
		my_make_pair(dfa1->q0, dfa2->q0),// start state
		[=](myPair<State, State2> a, Char b) -> myPair<State, State2> {
		return (my_make_pair(dfa1->Delta(a.first, b), dfa2->Delta(a.second, b)));
	},
		[=](myPair<State, State2> a) { // accept states
		return ((dfa1->F(a.first)) || (dfa2->F(a.second)));
	});
}

// creates a DFA that is the intersection of dfa1 and dfa2
template <class State, class State2>
DFA<myPair<State, State2>>* intersectionDFA(DFA<State>* dfa1, DFA<State2>* dfa2)
{
	return new DFA<myPair<State, State2>>(
		[=](myPair<State, State2> a) -> bool { 
		return (dfa1->Q(a.first) && dfa2->Q(a.second));
	},
		dfa1->v,// alphabet
		my_make_pair(dfa1->q0, dfa2->q0),
		[=](myPair<State, State2> a, Char b) -> myPair<State, State2> {
		return (my_make_pair(dfa1->Delta(a.first, b), dfa2->Delta(a.second, b)));
	},
		[=](myPair<State, State2> a) {                    // accept states
		return ((dfa1->F(a.first)) && (dfa2->F(a.second))); // only difference from unionDFA function
	});
}

// returns true/false to indicate 
// the calling DFA is a subuset of dfa2
template <class State, class State2>
bool subsetDFA(DFA<State>* dfa1, DFA<State2>* dfa2)
{
	DFA<myPair<State, State2>>* dfa3 = intersectionDFA(dfa1, complementDFA(dfa2));
	String* sub = dfa3->acceptedString();
	if (sub->isEmpty()) {
		return true;
	} else return false;
	//return sub;
}

// returns true/false to indicate 
// the calling DFA is a equal to dfa2
template <class State, class State2>
bool equalityDFA(DFA<State>* dfa1, DFA<State2>* dfa2)
{
	DFA<myPair<myPair<State, State2>, myPair<State, State2>>>* dfa3 = unionDFA(intersectionDFA(dfa1, complementDFA(dfa2)), intersectionDFA(complementDFA(dfa1), dfa2));
	String* sub = dfa3->acceptedString();
	if (sub->isEmpty()) {
		return true;
	}
	else return false;
}

// function for testing the input of a DFA
template <class State>
void testDFA(DFA<State>* inputDFA, String* inputString, bool valid) {
	
	cout << "testing with: ";
	inputString->print();
	cout << endl;
	cout << "DFA gives: " << inputDFA->accepts(inputString) << "\tshould give: " << valid << endl;
}

//****************************DFA END****************************//


//***************************NFA START**************************//

// NFA Class
template <class templ>
class NFA {
public:
	function<bool(templ)> Q;			// States
	vector<Char> v;						// Alphabet
	templ q0;							// Start state
	function<vector<templ>(templ, Char)> Delta;	// Delta function
	function<vector<templ>(templ)> EDelta;
	function<bool(templ)> F;			// Accepting states

	// standard constructor for a DFA
	NFA(function<bool(templ)> Q,					// DFA
		vector<Char> v,								// DFA
		templ q0,									// DFA
		function<vector<templ>(templ, Char)> Delta,	// Q x epsilon -> P(Q)
		function<vector<templ>(templ)> EDelta,
		function<bool(templ)> F) {					// DFA
		this->Q = Q;
		this->v = v;
		this->q0 = q0;
		this->Delta = Delta;
		this->EDelta = EDelta;
		this->F = F;
	}

	// DFA to NFA trivial function in the form of a cunstructor for NFA
	NFA(DFA<templ> in) {
		this->Q = in.Q;
		this->v = in.v;
		this->q0 = in.q0;
		this->Delta = ([=](int qi, Char c) {
			vector<templ> cState;
			cState[0] = in.Delta(qi, c);
			return cState;
			});
		this->F = in.F;
	}

	// Oracle function (task 27) doesnt work yet
	bool valid(OneString inputString, OneString traceString) {
		OneString* in = &inputString;
		OneString* tr = &traceString;

		templ state = q0;
		vector<templ> possibleStates;
		vector<templ> tempVec;

		if (tr->c.c != this->q0) { return false; }
		tr = (OneString*) tr->next();

		while (!tr->isEmpty() && !in->isEmpty()) {
			tempVec = this->EDelta(state);
			possibleStates.insert(possibleStates.end(), tempVec.begin(), tempVec.end());
			tempVec = this->Delta(state, in->c);
			possibleStates.insert(possibleStates.end(), tempVec.begin(), tempVec.end());

			// if traceString.c isnt in possibleStates then return fasle
			bool track = 0;
			for (templ x : possibleStates) {
				if (tr->c.c == x) { track = 1; }
			}
			if (track == 0) { return false; }
			tempVec.clear();
			possibleStates.clear();

			in = (OneString*) in->next();
			tr = (OneString*) tr->next();
		}
		// if all letters are used up from input and trace
		// without already returning false, then its true
		if (inputString.isEmpty() && traceString.isEmpty()) { return true; }
		return false; 
	}

	// accepts function for NFA 
	bool accepts(OneString inputString)
	{
		vector<templ> currentStates{ this->q0 };
		vector<templ> tempVector;
		vector<templ> newStates;
		vector<templ> epsilonStates;
		OneString* temp = &inputString;

		if (temp->isEmpty()) 
		{
			tempVector = this->EDelta(this->q0);
			currentStates.insert(currentStates.begin(), tempVector.begin(), tempVector.end());
		}

		// step through NFA with the input string
		while (temp->isEmpty() != true)
		{
			newStates.clear(); 
			epsilonStates.clear();

			for (templ x : currentStates)
			{
				tempVector = this->EDelta(x); 
				epsilonStates.insert(epsilonStates.end(), tempVector.begin(), tempVector.end());
			}
			currentStates.insert(currentStates.end(), epsilonStates.begin(), epsilonStates.end());

			for (templ x : currentStates)
			{
				tempVector = this->Delta(x, temp->fChar()); 
				newStates.insert(newStates.end(), tempVector.begin(), tempVector.end());
			}

			currentStates.clear();
			currentStates = newStates;
			temp = (OneString*) temp->next();
		}

		for (templ x : currentStates)
		{
			if (F(x))
				return true;
		}
		return false;
	}
};

//***************************NFA END****************************//

int main()
{
	mtChar test = mtChar();
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
	int r = 58;
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

	// DFA for odd length string
	auto oddL =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			binary,
			0,
			[](int qi, Char c) {
		if (qi == 0) { return 1; }
		else { return 0; }},
			[](int qi) { return qi == 1; });

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

	// DFA for even Number (Decimal)
	auto evenNDec =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			decimal,
			0,
			[](int qi, Char c) {
		if (qi == 0) {
			if (c.c == '0' || c.c == '2' || c.c == '4' || c.c == '6' || c.c == '8') { return 0; }
			else return 1;
		}
		else if (qi == 1) {
			if (c.c == '0' || c.c == '2' || c.c == '4' || c.c == '6' || c.c == '8') { return 0; }
			else return 1;
		}
	},
			[](int qi) { return qi == 0; });

	// DFA for odd number (binary)
	auto oddN =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			binary,
			1,
			[](int qi, Char c) {
		if (qi == 0) {
			if (c.c == '1') { return 0; }
			else return 1;
		}
		else if (c.c == '1') { return 0; }
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

	// accepts any number of 0's meant to be a subset of evenN
	auto zeroDFA =
		new DFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			binary,
			0,
			[](int qi, Char c) {
		if (qi == 0) {
			if (c.c == '0') {
				return 0;
			}
			else return 1;
		} if (qi == 1) return 1; },
			[](int qi) { return qi == 0; });

	// DFA's created using special functions

	// DFA that only accepts the given character
	auto onlyOne = new DFA<int>('A');

	// DFA for odd numbers (complement of even numbers)
	auto oddNC = complementDFA(evenN);

	// DFA for odd length strings
	auto oddLC = complementDFA(evenL);
	
	// DFA that should accept even number OR even lengthed strings
	auto unionTest = unionDFA(evenN, evenL);

	// DFA that should accept strings that are even length AND even numbers
	auto intersectionTest = intersectionDFA(evenN, evenL);

	//************************End of DFA section********************//

	//***************************NFA section************************//

	// accepts a string of zeros with length divisable by 2 or 3
	auto zero32 =
		new NFA<int>
		([](int qi) { return qi == 0 || qi == 1 || qi == 2 || qi == 3 || qi == 4 || qi == 5; },
			binary,
			0,
			[](int qi, Char c) {
				vector<int> vec{};
				vector<int> vec0{ 0 };
				vector<int> vec1{ 1 };
				vector<int> vec2{ 2 };
				vector<int> vec3{ 3 };
				vector<int> vec4{ 4 };
				vector<int> vec5{ 5 };
				if (qi == 0) {
					return vec;
				}
				else if (qi == 1) {
					if (c.c == '0') {
						return vec2;
					}
					else return vec;
				}
				else if (qi == 2) {
					if (c.c == '0') {
						return vec1;
					}
					else return vec;
				}
				else if (qi == 3) {
					if (c.c == '0') {
						return vec4;
					}
					else return vec;
				}
				else if (qi == 4) {
					if (c.c == '0') {
						return vec5;
					}
					else return vec;
				}
				else if (qi == 5) {
					if (c.c == '0') {
						return vec3;
					}
					else return vec;
				}
			},
			[](int qi) {
				vector<int> vec{};
				vector<int> vec13{ 1,3 };
				if (qi == 0) {
					return vec13;
				}
				else return vec;
			},
			[](int qi) { return qi == 1 || qi == 3; });

	// ends in a 0
	auto endInZeroNFA =
		new NFA<int>
		([](int qi) { return qi == 0 || qi == 1; },
			binary,
			0,
			[](int qi, Char c) {
				vector<int> vec{};
				vector<int> vec1{ 1 };
				vector<int> vec0{ 0 };
				vector<int> vec01{0,1};
				if (qi == 0) {
					if (c.c == '0') {
						return vec01;
					}
					else if (c.c == '1') {
						return vec0;
					}
				}
				else return vec;
			},
			[](int qi) {
				vector<int> vec{};
				return vec;
			},
			[](int qi) { return qi == 1; });

	//************************End of NFA section********************//
	//	0
	OneString* test1 = new OneString(Char('0'), new epsilon());
	//	
	epsilon* test2 = new epsilon();
	//	AA
	OneString* test3 = new OneString(Char('A'), new OneString(Char('A'), new epsilon()));
	//	scott
	OneString* nameString = new OneString(Char('s'), new OneString(Char('c'), new OneString(Char('o'), new OneString(Char('t'), new OneString(Char('t'), new epsilon())))));
	//	10
	OneString* test4 = new OneString(Char('1'), new OneString(Char('0'), new epsilon()));
	//	01
	OneString* test5 = new OneString(Char('0'), new OneString(Char('1'), new epsilon()));
	//	011
	OneString* test6 = new OneString(Char('0'), new OneString(Char('1'), new OneString(Char('1'), new epsilon())));
	//	010
	OneString* test7 = new OneString(Char('0'), new OneString(Char('1'), new OneString(Char('0'), new epsilon())));
	//	0000
	String* zeroDfaTest = nString(16, binary); // String of 4 zeros 
	//	370
	OneString* test8 = new OneString(Char('3'), new OneString(Char('7'), new OneString(Char('0'), new epsilon())));
	//	523
	OneString* test9 = new OneString(Char('5'), new OneString(Char('2'), new OneString(Char('3'), new epsilon())));
	//	00
	OneString* test01 = new OneString(Char('0'), new OneString(Char('0'), new epsilon()));
	//	11
	OneString* test02 = new OneString(Char('1'), new OneString(Char('1'), new epsilon()));


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
	testDFA(oddLC, test1, true);
	testDFA(oddLC, test2, false);
	testDFA(oddLC, test3, false);
	testDFA(oddLC, test4, false);
	testDFA(oddLC, test5, false);
	testDFA(oddLC, test6, true);
	testDFA(oddLC, test7, true);
	cout << endl;

	cout << "\t\ttesting with oddN:\n";
	testDFA(oddNC, test1, false);
	testDFA(oddNC, test2, false);
	testDFA(oddNC, test3, true);	// any non number input is 'odd'
	testDFA(oddNC, test4, false);
	testDFA(oddNC, test5, true);
	testDFA(oddNC, test6, true);
	testDFA(oddNC, test7, false);
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

	cout << "\t\ttesting with zeroDFA:\n";
	testDFA(zeroDFA, test1, true);
	testDFA(zeroDFA, test4, false);
	testDFA(zeroDFA, test5, false);
	testDFA(zeroDFA, test6, false);
	testDFA(zeroDFA, test7, false);
	testDFA(zeroDFA, zeroDfaTest, true);
	cout << endl;

	cout << "\t\ttesting with evenNDec:\n";
	testDFA(evenNDec, test1, true);
	testDFA(evenNDec, test4, true);
	testDFA(evenNDec, test5, false);
	testDFA(evenNDec, test8, true);
	testDFA(evenNDec, test9, false);
	testDFA(evenNDec, zeroDfaTest, true);
	cout << endl;

	// Trivial tests for subset and equality
	bool subTest = subsetDFA(zeroDFA, evenN);
	bool subTest2 = subsetDFA(oddNC, evenN);
	cout << subTest << subTest2 << endl;

	bool equalTest = equalityDFA(evenN, evenN);
	bool equalTest2 = equalityDFA(evenN, evenL);
	cout << equalTest << equalTest2 << endl;

	// Better test for equality
	bool betterEqual1 = equalityDFA(oddN, oddNC);
	bool betterEqual2 = equalityDFA(oddL, oddLC);
	cout << betterEqual1 << betterEqual2 << endl;

	cout << endl;
	String* boop = nameDFA->acceptedString();
	boop->print();
	cout << endl;

	nameDFA->trace(nameString); // States 0-1-2-3-4
	cout << endl;
	evenN->trace(test7);		// States 0-0-1
	cout << endl;
	evenL->trace(test7);		// States 0-1-0
	cout << endl;
	/*trace of a union shows each trace
	individually, but also vertically next to eachother
	in this case:
	00
	01
	10*/
	unionTest->trace(test7);	// trace the union of two DFA's
	cout << endl;

	// END OF DFA TESTING



	// NFA TESTING
	
	// traces of nfa behavior
	// zero32 with string '0000'
	// accepted
	OneString* z32trace1 = new OneString(Char('0'), new OneString(Char('1'), new OneString(Char('2'), new OneString(Char('1'), new OneString(Char('2'), new OneString(Char('1'), new epsilon()))))));
	// not accepted
	OneString* z32trace2 = new OneString(Char('0'), new OneString(Char('3'), new OneString(Char('4'), new OneString(Char('5'), new OneString(Char('3'), new OneString(Char('4'), new epsilon()))))));


	// endInZeroNFA with string '0000'
	// accepted
	OneString* ztrace1 = new OneString(Char('0'), new OneString(Char('0'), new OneString(Char('0'), new OneString(Char('0'), new OneString(Char('1'), new epsilon())))));
	// not accepted or done yet
	OneString* ztrace = new OneString(Char('0'), new OneString(Char('1'), new epsilon()));


	// testing oracle function
	cout << "testing values for oracle: ";
	cout << endInZeroNFA->valid((OneString&)*zeroDfaTest, *ztrace1) << endl;

	cout << zero32->accepts(*test01) << endl;
	cout << zero32->accepts(*test3) << endl;
	cout << zero32->accepts(*test02) << endl;
	cout << zero32->accepts((OneString&)*zeroDfaTest) << endl << endl;

	cout << endInZeroNFA->accepts(*test01) << endl;
	cout << endInZeroNFA->accepts(*test02) << endl;
	cout << endInZeroNFA->accepts((OneString&)*zeroDfaTest) << endl;
	
}