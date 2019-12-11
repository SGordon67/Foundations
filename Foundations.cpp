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
	Char() { this->c = '_'; }
	Char(char x) {this->c = x;}
	bool isEmpty() { return false; }
};

class mtChar : public Char {
	char c;
public:
	mtChar() {
		this->c = '_';
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
	virtual Char fChar() { return (Char('_')); }
	virtual String* next() { return (&String()); }
	virtual void print() {};
};

class epsilon : public String {
public:
	Char ep = Char('_');
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
		this->c = Char('_');
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
		std::cout << this->c.c;
		this->s->print();
	}
};

class TraceEpsilon : public String {
public:
	Char ep = Char('_');
	TraceEpsilon() {}
	bool isEmpty() { return true; }
	Char fChar() { return ep; }
	String* next() { throw new _exception; }
	void print() {}
};

class TraceString : public String {
public:
	Char c;
	Char state;
	String* s;
	TraceString() {
		this->c = Char('_');
		this->s = new epsilon();
	}
	TraceString(Char c, Char state, String* s) {
		this->c = c;
		this->state = state;
		this->s = s;
	}

	bool isEmpty() {
		return false;
	}
	Char fChar() { return this->c; }
	Char getState() { return this->state; }
	String* next() { return this->s; }

	void print() {
		std::cout << this->c.c << this->state.c;
		if (!this->next()->isEmpty()) {
			cout << "->";
		}
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
	bool trace(OneString inputString) {
		State qi = this->q0;
		OneString* temp = &inputString;

		cout << qi;
		while (temp->isEmpty() == false) {
			qi = Delta(qi, temp->fChar());
			temp = (OneString*) temp->next();
			cout << qi;
		}
		qi = Delta(qi, temp->fChar());

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

	// Oracle function (task 27)
	bool valid(OneString inputString, TraceString traceString) {

		OneString* in = &inputString;
		TraceString* tr = &traceString;
		vector<templ> tempVec;
		templ state = this->q0;
		int track = 0;

		// if trace doesnt start with the start state then return flse
		// remove start state
		if ((tr->state.c) != this->q0) {
			return false;
		}
		tr = (TraceString*) tr->next();

		// loop while your trace isnt empty
		while (!tr->isEmpty()) {
			track = 0;

			// detect epsilon transition
			if (tr->fChar().c == '_') {
				tempVec = this->EDelta(state);
				for (templ x : tempVec) {
					if ((tr->getState().c) == x) { track++; }
				}
				if (track < 1) { return true; }

				// since we got here by epsilon, 
				// we dont have to use up a character.
				// move the state and trace forward
				state = tr->getState().c;
				tr = (TraceString*)tr->next();
			}
			// check if the character of the trace is the same as input
			else if (tr->fChar().c == in->fChar().c) {
				tempVec = this->Delta(state, tr->fChar().c);
				for (templ x : tempVec) {
					if ((tr->getState().c) == x) { track++; }
				}
				if (track < 1) { return false; }

				// move trace, state, and input forward
				state = tr->getState().c;
				tr = (TraceString*)tr->next();
				in = (OneString*)in->next();
			}
			// if charater wasnt the same as input, the trace is invalid
			else return false;
		}
		if (tr->isEmpty() && in->isEmpty()) { return true; }
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

		// if input is empty, add epsilon transition to current states
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

			// add epsilon transitions to current states
			for (templ x : currentStates)
			{
				tempVector = this->EDelta(x); 
				epsilonStates.insert(epsilonStates.end(), tempVector.begin(), tempVector.end());
			}
			currentStates.insert(currentStates.end(), epsilonStates.begin(), epsilonStates.end());

			// add delta transitions to new states
			for (templ x : currentStates)
			{
				tempVector = this->Delta(x, temp->fChar()); 
				newStates.insert(newStates.end(), tempVector.begin(), tempVector.end());
			}

			// clear the current states and then update
			// move the input forward
			currentStates.clear();
			currentStates = newStates;
			temp = (OneString*) temp->next();
		}

		// if none of your current states are accepting
		// then the string isnt accepted
		for (templ x : currentStates)
		{
			if (this->F(x))
				return true;
		}
		return false;
	}

	// trace tree function
	void traceTree(String* inputString) // creates tree of all possible traces
	{
		vector<templ> currentStates{};	// used for delta function
		vector<templ> epsilonStates{};	// used for epsilon transitions
		vector<templ> comboStates{};	// combines delta and epsilon states
		vector<templ> tempVector{};		// used to gather input for each vector
		vector<templ> newStates{ this->q0 }; // keeps track of what happened in the last iteration
		String* temp = inputString;		// character input

		// trace must always start with the start state
		cout << "[ " << this->q0 << " ]" << endl;

		// if the input is empty, print out the start state and epsilon transitions
		if (temp->isEmpty()) // inputString is the emptyString
		{
			cout << "[";
			tempVector = this->EDelta(this->q0); // check for epsilon transitions from start state
			currentStates.insert(currentStates.end(), tempVector.begin(), tempVector.end());
			for (auto x : currentStates) {
				if (this->F(x)) { cout << "("; }
				std::cout << x;
				if (this->F(x)) { cout << ")"; }
				if (x != currentStates.back()) {
					cout << " ";
				}
			}
			cout << "]";
		}
		
		// loop while the input is not empty
		while (!temp->isEmpty()) {
			cout << "[";
			// gather all the epsilon and delta transitions and save resulting states
			for (auto y : newStates) {
				tempVector = this->EDelta(y);
				epsilonStates.insert(epsilonStates.end(), tempVector.begin(), tempVector.end());
				comboStates.insert(comboStates.end(), tempVector.begin(), tempVector.end());
				tempVector = this->Delta(y, temp->fChar().c);
				currentStates.insert(currentStates.end(), tempVector.begin(), tempVector.end());
				comboStates.insert(comboStates.end(), tempVector.begin(), tempVector.end());
			}
			// outputting the resulting states with formatting
			for (auto x : comboStates) {
				if (this->F(x)) { 
					cout << "(" << x << ")"; 
				}
				else {
					cout << " " << x << " ";
				}
				if (x != comboStates.back()) {
					cout << " ";
				}
			}
			cout << "]" << endl;
			// reset all the local variables
			// if you didnt delta transition, dont use up a character
			newStates = comboStates;
			if (!currentStates.empty()) {
				temp = (OneString*)temp->next();
			}
			currentStates.clear();
			epsilonStates.clear();
			comboStates.clear();
		}
		cout << endl;
	}

};

template<class templ, class templ2>
class NFAUnionState {
public:
	bool isStartState;
	bool isAcceptingState;
	templ fromX;	// state when you're in the first NFA
	templ fromY;	// state when you're in the second NFA

	NFAUnionState() {};
	NFAUnionState(bool isStartState, bool isAcceptingState, templ fromX, templ2 fromY) {
		this->isStartState = isStartState;
		this->isAcceptingState = isAcceptingState;
		this->fromX = fromX;
		this->fromY = fromY;
	}
};

template<class templ, class templ2>
NFA<NFAUnionState<templ, templ2>>* UnionNFA(NFA<templ> input1, NFA<templ2> input2) {
	templ state = '_';
		return new NFA<NFAUnionState<templ, templ2>>([=](NFAUnionState<templ, templ2> qi) -> bool {
			return(qi.isStartState || input1.Q(qi.fromX) || input2.Q(qi.fromY));
		},
		// take the alphabet from the first NFA, they should be the same though
		input1.v,
			// start state for the union, epsilon transitions to start states of both
			NFAUnionState<templ, templ2>(1, 0, state, state),// states could be anything
			[=](NFAUnionState<templ, templ2> qi, Char c) {
			vector<NFAUnionState<templ, templ2>> ret{};
			// if you're in the start state, dont delta trans to anything
			if (qi.isStartState) {
				return ret;
			}
			vector<templ> temp = input1.Delta(qi.fromX, c);
			vector<templ2> temp2 = input2.Delta(qi.fromY, c);
			bool acceptTemp = 0;
			//cout << endl << "c.c:" << c.c << " ";
			//cout << "qi.fromX:" << qi.fromX << " ";
			//cout << "qi.fromY:" << qi.fromY << " ";
			if (temp.size() == 0 && temp2.size() == 0) {
				return ret;
			}
			else if (temp.size() == 0) {
				for (templ y : temp2) {
					if (input2.F(y)) {
						acceptTemp = 1;
					}
					else acceptTemp = 0;
					NFAUnionState<templ, templ2> ex(0, acceptTemp, state, y);
					ret.push_back(ex);
				}
			}
			else if (temp2.size() == 0) {
				for (templ x : temp) {
					if (input1.F(x)) {
						acceptTemp = 1;
					}
					else acceptTemp = 0;
					NFAUnionState<templ, templ2> ex(0, acceptTemp, x, state);
					ret.push_back(ex);
				}
			}
			else {
				// take all possible combinations of states from both delta transitions
				for (templ x : temp) {
					//cout << "|";
					for (templ y : temp2) {
						//cout << " :" << x << " ";
						//cout << "" << y << ": ";
						// remember if youre in an accepting state
						if (input1.F(x) || input2.F(y)) {
							acceptTemp = 1;
						}
						else acceptTemp = 0;
						// make a union state out of the pair and add it to the returning vector
						NFAUnionState<templ, templ2> ex(0, acceptTemp, x, y);
						ret.push_back(ex);
					}
					//cout << "|";
				}
			}
			return ret;
			},
			[=](NFAUnionState<templ, templ2> qi) {
			vector<NFAUnionState<templ, templ2>> ret{};
			bool acceptTemp = 0;

			// if you're in the start state, epsilon transition to the start states of both NFAs
			if (qi.isStartState) {
				if (input1.F(input1.q0) || input2.F(input2.q0)) {
					acceptTemp = 1;
				}
				else acceptTemp = 0;
				NFAUnionState<templ, templ2> ex(0, acceptTemp, input1.q0, input2.q0);
				ret.push_back(ex);
			}
			else {
				vector<templ> temp = input1.EDelta(qi.fromX);
				vector<templ> temp2 = input2.EDelta(qi.fromY);
				// take all possible combos of states created with epsilon trans of both NFAs
				for (templ x : temp) {
					for (templ y : temp2) {
						if (input1.F(x) || input2.F(y)) {
							acceptTemp = 1;
						}
						else acceptTemp = 0;
						// make a union state out of the pair and add it to the returning vector
						NFAUnionState<templ, templ2> ex(0, acceptTemp, x, y);
						ret.push_back(ex);
					}
				}
			}
			return ret;
		},
		[=](NFAUnionState<templ, templ2> qi) {
			return (input1.F(qi.fromX) || input2.F(qi.fromY));
		});
}

template<class templ, class templ2>
class NFAConcatState {
public:
	bool isFromX;
	bool isFromY;
	templ fromX;	// state when you're in the first NFA
	templ2 fromY;	// state when you're in the second NFA

	NFAConcatState() {};
	NFAConcatState(bool isFromX, bool isFromY, templ fromX, templ2 fromY) {
		this->isFromX = isFromX;
		this->isFromY = isFromY;
		this->fromX = fromX;
		this->fromY = fromY;
	}
};

template<class templ, class templ2>
NFA<NFAConcatState<templ, templ2>>* ConcatNFA(NFA<templ> input1, NFA<templ2> input2) {
	//templ state = '_';
	templ state = input1.q0;
	templ2 state2 = input2.q0;
	return new NFA<NFAConcatState<templ, templ2>>(
		[=](NFAConcatState<templ, templ2> qi) -> bool {
		return(input1.Q(qi.fromX) || input2.Q(qi.fromY));
		},
		input1.v,
			NFAConcatState<templ, templ2>(1, 0, input1.q0, state2)
			,
			[=](NFAConcatState<templ, templ2> qi, Char c) {
				vector<NFAConcatState<templ, templ2>> ret{};
				vector<templ> temp{};
				vector<templ2> temp2{};
				if (qi.isFromX) {
					temp = input1.Delta(qi.fromX, c);
					for (templ x : temp) {
						NFAConcatState<templ, templ2> ex = NFAConcatState<templ, templ2>(1, 0, x, state2);
						ret.push_back(ex);
					}
				}
				else if (qi.isFromY) {
					temp2 = input2.Delta(qi.fromY, c);
					for (templ2 x : temp2) {
						NFAConcatState<templ, templ2> ex = NFAConcatState<templ, templ2>(0, 1, state, x);
						ret.push_back(ex);
					}
				}
				return ret;
			},
			[=](NFAConcatState<templ, templ2> qi) {
				vector<NFAConcatState<templ, templ2>> ret{};
				vector<templ> temp{};
				vector<templ2> temp2{};
				if (qi.isFromX) {
					if (input1.F(qi.fromX)) {
						NFAConcatState<templ, templ2> swap = NFAConcatState<templ, templ2>(0, 1, state, input2.q0);
						ret.push_back(swap);
					}
					temp = input1.EDelta(qi.fromX);
					for (templ x : temp) {
						NFAConcatState<templ, templ2> ex = NFAConcatState<templ, templ2>(1, 0, x, state2);
						ret.push_back(ex);
					}
				}
				else if (qi.isFromY) {
					temp2 = input2.EDelta(qi.fromY);
					for (templ2 x : temp2) {
						NFAConcatState<templ, templ2> ex = NFAConcatState<templ, templ2>(1, 0, state, x);
						ret.push_back(ex);
					}
				}
				return ret;
			},
			[=](NFAConcatState<templ, templ2> qi) {
				return input2.F(qi.fromY);
			});
}

template<class templ>
NFA<templ>* KleeneStar(NFA<templ> input) {
	templ state = '_';
	return new NFA<templ>(
		[=](templ qi) -> bool {
			return (input.Q(qi) || qi == '_');
		},
		input.v,
			state,
			[=](templ qi, Char c) {
			vector<templ> ret{};
			ret = input.Delta(qi, c);
			return ret;
		},
			[=](templ qi) {
			vector<templ> ret{};
			vector<templ> temp{};
			if (qi == state || input.F(qi)) {
				ret.push_back(input.q0);
			}
			else {
				temp = input.EDelta(qi);
				for (templ x : temp) {
					ret.push_back(x);
				}
			}
			return ret;
		},
			[=](templ qi) {
			return (input.F(qi) || qi == state);
		});
}

// Converts an NFA to a DFA
template <class State>
DFA<vector<State>>* NFAtoDFA(NFA<State> nfa)
{
	vector<State> startStates{ nfa.q0 };
	vector<State> epsiFromStart = nfa.EDelta(nfa.q0);
	startStates.insert(startStates.end(), epsiFromStart.begin(), epsiFromStart.end());

	return new DFA<vector<State>>(
		[=](vector<State> a) -> bool {
			for (State x : a)
			{
				if (!nfa.Q(x))
					return false;
			}
			return true; // all elements of input vector are valid nfa states
		},
		nfa.v, // uses same alphabet as given nfa
			startStates,
			[=](vector<State> a, Char b) -> vector<State> { // transition function
			vector<State> tempVector;
			vector<State> newStates;
			vector<State> currentStates;
			vector<State> epsilonStates;

			for (State x : a) // check if there are epsilon transitions from the current state
			{
				tempVector = nfa.EDelta(x);
				epsilonStates.insert(epsilonStates.end(), tempVector.begin(), tempVector.end());
			}
			currentStates.insert(currentStates.end(), epsilonStates.begin(), epsilonStates.end());
			currentStates.insert(currentStates.end(), a.begin(), a.end());

			for (State x : currentStates) // generate Delta states from input char with each current state
			{
				tempVector = nfa.Delta(x, b);
				newStates.insert(newStates.end(), tempVector.begin(), tempVector.end());
			}
			currentStates.clear();
			currentStates.insert(currentStates.end(), newStates.begin(), newStates.end());

			return currentStates; // return new state generated from the current state
		},
			[=](vector<State> a) -> bool {
			for (State x : a)
			{
				if (nfa.F(x)) // make states in vector are all accepted by the original nfa
					return true;
			}
			return false;
		});
}

//***************************NFA END****************************//

//***************************REG EXP****************************//

class regex {
public:
	bool isMtRegex;
	bool isEpsRegex;
	bool isCharRegex;
	bool isUnionRegex;
	bool isStarRegex;
	bool isConcatRegex;
	regex() {
		this->isMtRegex = false;
		this->isEpsRegex = false;
		this->isCharRegex = false;
		this->isUnionRegex = false;
		this->isStarRegex = false;
		this->isConcatRegex = false;
	};
	virtual void print() {
	}
	virtual String* generate() {
		return new epsilon();
	}
};
class mtRegex : public regex {
public:
	mtRegex(){
		this->isMtRegex = true;
		this->isEpsRegex = false;
		this->isCharRegex = false;
		this->isUnionRegex = false;
		this->isStarRegex = false;
		this->isConcatRegex = false;
	}
	void print() {
		cout << "";
	}
	String* generate() {
		return new epsilon();
	}
};
class epsRegex : public regex {
public:
	epsRegex(){
		this->isMtRegex = false;
		this->isEpsRegex = true;
		this->isCharRegex = false;
		this->isUnionRegex = false;
		this->isStarRegex = false;
		this->isConcatRegex = false;
	}
	void print() {
		cout << "_";
	}
	String* generate() {
		return new epsilon();
	}
};
class charRegex : public regex {
public:
	Char c;
	charRegex(Char c){
		this->c = c;
		this->isMtRegex = false;
		this->isEpsRegex = false;
		this->isCharRegex = true;
		this->isUnionRegex = false;
		this->isStarRegex = false;
		this->isConcatRegex = false;
	}
	void print() {
		cout << this->c.c;
	}
	String* generate() {
		return new OneString(Char(this->c.c), new epsilon());
	}
};
class unionRegex : public regex {
public:
	regex* left;
	regex* right;
	unionRegex(regex* left, regex* right){
		this->left = left;
		this->right = right;
		this->isMtRegex = false;
		this->isEpsRegex = false;
		this->isCharRegex = false;
		this->isUnionRegex = true;
		this->isStarRegex = false;
		this->isConcatRegex = false;
	}
	void print() {
		cout << "(";
		this->left->print();
		cout << " U ";
		this->right->print();
		cout << ")";
	}
	OneString* generate() {
		return ((OneString*)left->generate());
	}
};
class starRegex : public regex {
public:
	regex* r;
	starRegex(regex* r){
		this->r = r;
		this->isMtRegex = false;
		this->isEpsRegex = false;
		this->isCharRegex = false;
		this->isUnionRegex = false;
		this->isStarRegex = true;
		this->isConcatRegex = false;
	}
	void print() {
		cout << "(";
		this->r->print();
		cout << ")*";
	}
	OneString* generate() {
		return ((OneString*) r->generate());
	}
};
class concatRegex : public regex {
public:
	regex* left;
	regex* right;
	concatRegex(regex* left, regex* right) {
		this->left = left;
		this->right = right;
		this->isMtRegex = false;
		this->isEpsRegex = false;
		this->isCharRegex = false;
		this->isUnionRegex = false;
		this->isStarRegex = false;
		this->isConcatRegex = true;
	}
	void print() {
		this->left->print();
		this->right->print();
	}
	OneString* generate() {
		OneString* L = (OneString*) left->generate();
		OneString* R = (OneString*) right->generate();
		OneString* index = L;
		
		while (!index->next()->isEmpty()) {
			index = (OneString*)index->next();
		}
		index->s = R;

		return L;
	}
};

OneString* generator(regex* input) {
	return ((OneString*) input->generate());
}


// functions to convert each type of regex into an NFA

// function header so I can reference it before it's defined
NFA<char>* RegexToNFA(regex* input, char &offset);

// trivial epsilon regex to NFA fucntion, never reaches accept state
NFA<char>* epNFAConv(regex* input, char &offset) {
	offset++;
	char off1 = offset;
	offset++;
	char off2 = offset;
	vector<Char> alph{Char(off1), Char(off2)};
	return new NFA<char>(
		[=](char qi) -> bool {
			return (qi == off1 || qi == off2);
		},
		alph,
		off1,
			[=](char qi, Char c) {
			vector<char> ret{};
			return ret;
			},
			[=](char qi) {
				vector<char> ret{};
				vector<char> retb{ off2 };
				if (qi == off1) {
					return retb;
				}
				else return ret;
			},
			[=](char qi) {
				return (qi == off2);
		});
}

// also trivial empty regex to NFA function, starts at accepting and transitions away
NFA<char>* mtNFAConv(regex* input, char &offset) {
	offset++;
	char off1 = offset;
	offset++;
	char off2 = offset;
	vector<Char> alph{ Char(off1), Char(off2) };
	return new NFA<char>(
		[=](char qi) -> bool {
			return (qi == off1 || qi ==  off2);
		},
		alph,
			off1,
			[=](char qi, Char c) {
			vector<char> ret{off2};
			return ret;
		},
			[=](char qi) {
			vector<char> ret{off2};
			return ret;
		},
			[=](char qi) {
			return (qi == off1);
		});
}

// char to NFA function, accepts a single specific character given
NFA<char>* chNFAConv(regex* input, char &offset) {
	offset++;
	char off1 = offset;
	offset++;
	char off2 = offset;
	offset++;
	char off3 = offset;
	charRegex* actual = (charRegex*)input;
	vector<Char> alph{ Char(off1), Char(off2) };
	return new NFA<char>(
		[=](char qi) -> bool {
			return (qi == off1 || qi == off2 || qi == off3);
		},
		alph
			,
			off1
			,
			[=](char qi, Char c) {
			vector<char> ret{};
			vector<char> reta{ off1 };
			vector<char> retb{ off2 };
			vector<char> retc{ off3 };
			if (qi == off1) {
				if (c.c == actual->c.c) {
					return retb;
				}
				else return retc;
			}
			else return retc;
		},
			[=](char qi) {
			vector<char> ret{};
			return ret;
		},
			[=](char qi) {
			return (qi == off2);
		});
}
// unionregex to NFA function, creates extra start state that epsilon transitions to the start
//  state of both individual NFA's (made from branches of regex tree)
//  accepts if either indivual branch would accept
NFA<char>* unNFAConv(regex* input, char &offset) {
	unionRegex* actual = (unionRegex*)input;
	NFA<char>* leftNFA = RegexToNFA(actual->left, offset);
	NFA<char>* rightNFA = RegexToNFA(actual->right, offset);
	char stState = offset - 32;
	vector<char> ret{};

	return new NFA<char>(
		[=](char qi) -> bool {
			return (qi == stState || leftNFA->Q(qi) || rightNFA->Q(qi));
		},
		leftNFA->v
			,
			stState
			,
			[=](char qi, Char c) {
			if (leftNFA->Q(qi)) {
				return leftNFA->Delta(qi, c);
			}
			else if (rightNFA->Q(qi)) {
				return rightNFA->Delta(qi, c);
			}
			else return ret;
		},
			[=](char qi) {
			vector<char> eRet;
			if (qi == stState) {
				eRet.push_back(leftNFA->q0);
				eRet.push_back(rightNFA->q0);
				return eRet;
			}
			else if (leftNFA->Q(qi)) {
				return leftNFA->EDelta(qi);
			}
			else if (rightNFA->Q(qi)) {
				return rightNFA->EDelta(qi);
			}else return ret;
		},
			[=](char qi) {
			return (leftNFA->F(qi) || rightNFA->F(qi));
		});
}

// epsilon transition from the accepting state of first NFA to start state of second NFA
NFA<char>* coNFAConv(regex* input, char &offset) {
	concatRegex* actual = (concatRegex*)input;
	NFA<char>* leftNFA = RegexToNFA(actual->left, offset);
	NFA<char>* rightNFA = RegexToNFA(actual->right, offset);

	return new NFA<char>(
		[=](char qi) -> bool {
			return (leftNFA->Q(qi) || rightNFA->Q(qi));
		},
		leftNFA->v
			,
			leftNFA->q0
			,
			[=](char qi, Char c) {
			vector<char> ret{};
			if (leftNFA->Q(qi)) {
				return leftNFA->Delta(qi, c);
			}
			else if (rightNFA->Q(qi)) {
				return rightNFA->Delta(qi, c);
			}
			else return ret;
		},
			[=](char qi) {
			vector<char> eRet{};
			if (leftNFA->Q(qi)) {
				eRet = leftNFA->EDelta(qi);
				if (leftNFA->F(qi)) {
					eRet.push_back(rightNFA->q0);
				}
				return eRet;
			}
			else if (rightNFA->Q(qi)) {
				return rightNFA->EDelta(qi);
			}
			else return eRet;
		},
			[=](char qi) {
			return rightNFA->F(qi);
		});
}

// adds a new start state that is accepting and epsilon transitions to the old start state.
// also adds epsilon transitions from accepting states to the old start state
NFA<char>* stNFAConv(regex* input, char &offset){
	starRegex* actual = (starRegex*)input;
	NFA<char>* myNFA = RegexToNFA(actual->r, offset);
	offset++;
	char stState = offset - 32;

	return new NFA<char>(
		[=](char qi) -> bool {
			return(qi == stState || myNFA->Q(qi));
		},
		myNFA->v
			,
			stState
			,
			[=](char qi, Char c) {
			vector<char> ret{};
			if(qi == stState) {
				return ret;
			} else return (myNFA->Delta(qi, c));
		},
			[=](char qi) {
			vector<char> ret{};
			vector<char> temp{};
			if (qi == stState) {
				ret.push_back(myNFA->q0);
			}
			temp = myNFA->EDelta(qi);
			ret.insert(ret.end(), temp.begin(), temp.end());
			if (myNFA->F(qi)) {
				ret.push_back(myNFA->q0);
			}
			return ret;
		},
			[=](char qi) {
			return myNFA->F(qi);
		});
}

// regex to NFA function, calls functions specefic to the type of regex given
NFA<char>* RegexToNFA(regex* input, char &offset) {
	if (input->isEpsRegex) {
		return (epNFAConv(input, offset));
	}
	else if (input->isMtRegex) {
		return (mtNFAConv(input, offset));
	}
	else if (input->isCharRegex) {
		return (chNFAConv(input, offset));
	}
	else if (input->isUnionRegex) {
		return (unNFAConv(input, offset));
	}
	else if (input->isConcatRegex) {
		return (coNFAConv(input, offset));
	}
	else if (input->isStarRegex) {
		return (stNFAConv(input, offset));
	}
}

//***************************REG END****************************//

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
		else if (qi == 1) {
			if (c.c == '0') { return 0; }
			else return 1;
		}
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

	// third from the end is 1
	auto thirdFromEnd1 =
		new NFA<char>
		([](int qi) { return qi == 'a' || qi == 'b' || qi == 'c' || qi == 'd'; },
			binary,
			'a',
			[](char qi, Char c) {
				vector<char> vec{};
				vector<char> veca{ 'a' };
				vector<char> vecb{ 'b' };
				vector<char> vecc{ 'c' };
				vector<char> vecd{ 'd' };
				vector<char> vecab{ 'a','b' };
				if (qi == 'a') {
					if (c.c == '1') {
						return vecab;
					}
					else {
						return veca;
					}
				}
				else if (qi == 'b') {
					return vecc;
				}
				else if (qi == 'c') {
					return vecd;
				}
				else if (qi == 'd') {
					return vec;
				}
				else return vec;
			},
			[](char qi) {
				vector<char> vec{};
				return vec;
			},
				[](char qi) { return qi == 'd'; });

	// accepts a string of zeros with length divisable by 2 or 3
	auto zero32 =
		new NFA<char>
		([](char qi) { return qi == 'a' || qi == 'b' || qi == 'c' || qi == 'd' || qi == 'e' || qi == 'f'; },
			binary,
			'a',
			[](char qi, Char c) {
				vector<char> vec{};
				vector<char> veca{ 'a' };
				vector<char> vecb{ 'b' };
				vector<char> vecc{ 'c' };
				vector<char> vecd{ 'd' };
				vector<char> vece{ 'e' };
				vector<char> vecf{ 'f' };
				if (qi == 'a') {
					return vec;
				}
				else if (qi == 'b') {
					if (c.c == '0') {
						return vecc;
					}
					else return vec;
				}
				else if (qi == 'c') {
					if (c.c == '0') {
						return vecb;
					}
					else return vec;
				}
				else if (qi == 'd') {
					if (c.c == '0') {
						return vece;
					}
					else return vec;
				}
				else if (qi == 'e') {
					if (c.c == '0') {
						return vecf;
					}
					else return vec;
				}
				else if (qi == 'f') {
					if (c.c == '0') {
						return vecd;
					}
					else return vec;
				}
			},
			[](char qi) {
				vector<char> vec{};
				vector<char> vecbd{'b','d'};
				if (qi == 'a') {
					return vecbd;
				}
				else return vec;
			},
			[](char qi) { return qi == 'b' || qi == 'd'; });

	// ends in a 0
	auto ends0 =
		new NFA<char>
		([](int qi) { return qi == 'a' || qi == 'b'; },
			binary,
			'a',
			[](char qi, Char c) {
				vector<char> vec{};
				vector<char> vecb{ 'b' };
				vector<char> veca{ 'a' };
				vector<char> vecab{'a','b'};
				if (qi == 'a') {
					if (c.c == '0') {
						return vecab;
					}
					else return veca;
				}
				else return vec;
			},
			[](char qi) {
				vector<char> vec{};
				return vec;
			},
			[](char qi) { return qi == 'b'; });

	auto startsWithZero = 
		new NFA<char>
		([](int qi) { return qi == 'a' || qi == 'b'; },
			binary,
			'a',
			[](char qi, Char c) {
				vector<char> vec{};
				vector<char> vecb{ 'b' };
				vector<char> veca{ 'a' };
				if (qi == 'a') {
					if (c.c == '0') {
						return vecb;
					}
					else return vec;
				}
				else if (qi == 'b') {
					return vecb;
				}
				else return vec;
			},
			[](char qi) {
				vector<char> vec{};
				return vec;
			},
				[](char qi) { return qi == 'b'; });

	auto containsOne =
		new NFA<char>
		([](int qi) { return qi == 'a' || qi == 'b'; },
			binary,
			'a',
			[](char qi, Char c) {
				vector<char> vec{};
				vector<char> vecb{ 'b' };
				vector<char> veca{ 'a' };
				vector<char> vecab{ 'a', 'b' };
				if (qi == 'a') {
					if (c.c == '1') {
						return vecab;
					}
					else return veca;
				}
				else return vecab;
			},
			[](char qi) {
				vector<char> vec{};
				return vec;
			},
				[](char qi) { return qi == 'b'; });

	// union
	auto Ends0ANDThirdEnd1 = UnionNFA(*ends0, *thirdFromEnd1);

	auto Ends0ANDContains1 = UnionNFA(*ends0, *containsOne);

	auto Contains1ANDThirdEnd1 = UnionNFA(*containsOne, *thirdFromEnd1);
	
	auto Ends0ANDstarts0 = UnionNFA(*startsWithZero, *ends0);
	// testing union with a union
	auto Ends0ANDstarts0ANDContains1 = UnionNFA(*ends0, *containsOne);

	// concatination
	auto Ends0ThenContains1 = ConcatNFA(*ends0, *containsOne);

	auto Contains1ThenContains1 = ConcatNFA(*containsOne, *containsOne);
	// testing concatination with a concat
	auto Ends0ThenContains1ThenStarts0 = ConcatNFA(*Ends0ThenContains1, *startsWithZero);

	// Kleene star
	auto KSEnds0 = KleeneStar(*ends0);

	auto KSThirdEnd1 = KleeneStar(*thirdFromEnd1);

	// NFA -> DFA
	auto ends0DFA = NFAtoDFA(*ends0);

	auto thirdEnd1DFA = NFAtoDFA(*thirdFromEnd1);

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
	String* zeroDfaTest = nString(16, binary);
	//	370
	OneString* test8 = new OneString(Char('3'), new OneString(Char('7'), new OneString(Char('0'), new epsilon())));
	//	523
	OneString* test9 = new OneString(Char('5'), new OneString(Char('2'), new OneString(Char('3'), new epsilon())));
	//	00
	OneString* test10 = new OneString(Char('0'), new OneString(Char('0'), new epsilon()));
	//	00010
	OneString* test11 = new OneString(Char('0'), new OneString(Char('0'), new OneString(Char('0'), new OneString(Char('1'), new OneString(Char('0'), new epsilon())))));
	//	11
	OneString* test12 = new OneString(Char('1'), new OneString(Char('1'), new epsilon()));
	//	000
	OneString* test13 = new OneString(Char('0'), new OneString(Char('0'), new OneString(Char('0'), new epsilon())));
	//	100
	OneString* test14 = new OneString(Char('1'), new OneString(Char('0'), new OneString(Char('0'), new epsilon())));
	//	1100
	OneString* test15 = new OneString(Char('1'), new OneString(Char('1'), new OneString(Char('0'), new OneString(Char('0'), new epsilon()))));
	//	101
	OneString* test16 = new OneString(Char('1'), new OneString(Char('0'), new OneString(Char('1'), new epsilon())));
	//	1
	OneString* test17 = new OneString(Char('1'), new epsilon());
	//	00100
	OneString* test18 = new OneString(Char('0'), new OneString(Char('0'), new OneString(Char('1'), new OneString(Char('0'), new OneString(Char('0'), new epsilon())))));
	//	100100100
	OneString* test19 = new OneString(Char('1'), new OneString(Char('0'), new OneString(Char('0'), new OneString(Char('1'), new OneString(Char('0'), new OneString(Char('0'), new OneString(Char('1'), new OneString(Char('0'), new OneString(Char('0'), new epsilon())))))))));	
	//	2
	OneString* test20 = new OneString(Char('2'), new epsilon());


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

	cout << "Testing DFA functions" << endl << endl;
	// Trivial tests for subset and equality
	bool subTest = subsetDFA(zeroDFA, evenN);
	bool subTest2 = subsetDFA(oddNC, evenN);
	cout << "zeroDFA subset of evenN: " << subTest << endl;
	cout << "oddNC subset of evenN: " << subTest2 << endl << endl;

	bool equalTest = equalityDFA(evenN, evenN);
	bool equalTest2 = equalityDFA(evenN, evenL);
	cout << "evenN equal to evenN: " << equalTest << endl;
	cout << "evenN equal to evenL: " << equalTest2 << endl << endl;

	// Better test for equality
	bool betterEqual1 = equalityDFA(oddN, oddNC);
	bool betterEqual2 = equalityDFA(oddL, oddLC);
	cout << "oddN equal to oddNC: " << betterEqual1 << endl;
	cout << "oddL equal to oddLC: " << betterEqual2 << endl << endl;

	cout << "accepted string for the name DFA: ";
	String* boop = nameDFA->acceptedString();
	boop->print();
	cout << endl;

	cout << "a trace of '";
	nameString->print();
	cout << "' on nameDFA: ";
	nameDFA->trace(*nameString); // States 0-1-2-3-4
	cout << endl;

	cout << "a trace of '";
	test7->print();
	cout << "' on eveN: ";
	evenN->trace(*test7);		// States 0-0-1
	cout << endl;

	cout << "a trace of '";
	test7->print();
	cout << "' on evenL: ";
	evenL->trace(*test7);		// States 0-1-0
	cout << endl << endl;


	/*trace of a union shows each trace
	individually, but also vertically next to eachother
	in this case:
	00
	01
	10*/
	cout << "a trace of '";
	test7->print();
	cout << "' on the union of evenN and evenL: " << endl;
	unionTest->trace(*test7);	// trace the union of two DFA's
	cout << endl;

	// END OF DFA TESTING


	// NFA TESTING
	
	// traces of nfa behavior
	// zero32 with string '0000'
	// accepted
	TraceString* z32trace1 = new TraceString(mtChar(), Char('a'), new TraceString(mtChar(), Char('b'), new TraceString(Char('0'), Char('c'), new TraceString(Char('0'), Char('b'), new TraceString(Char('0'), Char('c'), new TraceString(Char('0'), Char('b'), new TraceEpsilon()))))));
	// not accepted
	TraceString* z32trace2 = new TraceString(mtChar(), Char('a'), new TraceString(mtChar(), Char('d'), new TraceString(Char('0'), Char('e'), new TraceString(Char('0'), Char('f'), new TraceString(Char('0'), Char('d'), new TraceString(Char('0'), Char('e'), new TraceEpsilon()))))));
	// not a proper trace
	TraceString* z32trace3 = new TraceString(mtChar(), Char('a'), new TraceString(mtChar(), Char('d'), new TraceString(Char('0'), Char('f'), new TraceString(Char('0'), Char('f'), new TraceString(Char('0'), Char('d'), new TraceString(Char('0'), Char('e'), new TraceEpsilon()))))));	

	// testing oracle function
	cout << "testing values for oracle:" << endl;
	cout << "inputString: ";
	zeroDfaTest->print();
	cout << endl << "traceString: ";
	z32trace1->print();
	cout << endl;
	bool val = zero32->valid((OneString&)*zeroDfaTest, *z32trace1);
	cout << "Result on zero32: " << val << endl << endl;

	cout << "inputString: ";
	zeroDfaTest->print();
	cout << endl << "traceString: ";
	z32trace2->print();
	cout << endl;
	bool val2 = zero32->valid((OneString&)*zeroDfaTest, *z32trace2);
	cout << "Result on zero32: " << val2 << endl << endl;
	
	// correctly returns false
	cout << "inputString: ";
	zeroDfaTest->print();
	cout << endl << "traceString: ";
	z32trace3->print();
	cout << endl;
	bool val3 = zero32->valid((OneString&)*zeroDfaTest, *z32trace3);
	cout << "Result on zero32: " << val3 << endl << endl;

	// testing accepts function
	cout << "testing the accepts function for NFAs:" << endl;
	cout << "input of '";
	test10->print();
	cout << "' on zero32" << endl;
	cout << "result: " << zero32->accepts(*test10) << endl;
	cout << "input of '";
	test3->print();
	cout << "' on zero32" << endl;
	cout << "result: " << zero32->accepts(*test3) << endl;
	cout << "input of '";
	test12->print();
	cout << "' on zero32" << endl;
	cout << "result: " << zero32->accepts(*test12) << endl;
	cout << "input of '";
	zeroDfaTest->print();
	cout << "' on zero32" << endl;
	cout << "result: " << zero32->accepts((OneString&)*zeroDfaTest) << endl << endl;

	cout << "input of '";
	test10->print();
	cout << "' on ends0" << endl;
	cout << "result: " << ends0->accepts(*test10) << endl;
	cout << "input of '";
	test12->print();
	cout << "' on ends0" << endl;
	cout << "result: " << ends0->accepts(*test12) << endl;
	cout << "input of '";
	zeroDfaTest->print();
	cout << "' on ends0" << endl;
	cout << "result: " << ends0->accepts((OneString&)*zeroDfaTest) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on ends0" << endl;
	cout << "result: " << ends0->accepts(*test5) << endl;
	cout << "input of '";
	test14->print();
	cout << "' on ends0" << endl;
	cout << "result: " << ends0->accepts(*test14) << endl;
	cout << "input of '";
	test4->print();
	cout << "' on ends0" << endl;
	cout << "result: " << ends0->accepts(*test4) << endl << endl;

	cout << "input of '";
	test12->print();
	cout << "' on thirdFromEnd1" << endl;
	cout << "result: " << thirdFromEnd1->accepts(*test12) << endl;
	cout << "input of '";
	test14->print();
	cout << "' on thirdFromEnd1" << endl;
	cout << "result: " << thirdFromEnd1->accepts(*test14) << endl;
	cout << "input of '";
	test15->print();
	cout << "' on thirdFromEnd1" << endl;
	cout << "result: " << thirdFromEnd1->accepts(*test15) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on thirdFromEnd1" << endl;
	cout << "result: " << thirdFromEnd1->accepts(*test5) << endl << endl;

	cout << "input of '";
	test10->print();
	cout << "' on startsWithZero" << endl;
	cout << "result: " << startsWithZero->accepts(*test10) << endl;
	cout << "input of '";
	test3->print();
	cout << "' on startsWithZero" << endl;
	cout << "result: " << startsWithZero->accepts(*test3) << endl;
	cout << "input of '";
	test12->print();
	cout << "' on startsWithZero" << endl;
	cout << "result: " << startsWithZero->accepts(*test12) << endl;
	cout << "input of '";
	zeroDfaTest->print();
	cout << "' on startsWithZero" << endl;
	cout << "result: " << startsWithZero->accepts((OneString&)*zeroDfaTest) << endl << endl;

	cout << "input of '";
	test10->print();
	cout << "' on containsOne" << endl;
	cout << "result: " << containsOne->accepts(*test10) << endl;
	cout << "input of '";
	test3->print();
	cout << "' on containsOne" << endl;
	cout << "result: " << containsOne->accepts(*test3) << endl;
	cout << "input of '";
	test12->print();
	cout << "' on containsOne" << endl;
	cout << "result: " << containsOne->accepts(*test12) << endl;
	cout << "input of '";
	zeroDfaTest->print();
	cout << "' on containsOne" << endl;
	cout << "result: " << containsOne->accepts((OneString&)*zeroDfaTest) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on containsOne" << endl;
	cout << "result: " << containsOne->accepts(*test5) << endl;
	cout << "input of '";
	test4->print();
	cout << "' on containsOne" << endl;
	cout << "result: " << containsOne->accepts(*test4) << endl << endl;

	// Testing trace tree function for strings on NFAs
	cout << "testing the trace tree function on NFAs:" << endl;
	cout << "input of '";
	test13->print();
	cout << "' on zero32" << endl;
	zero32->traceTree(test13);

	cout << "input of '";
	test2->print();
	cout << "' on zero32" << endl;
	zero32->traceTree(test2);
	cout << endl;

	// testing accepts with Ends0ANDThirdEnd1
	cout << "Testing accepts function on union NFAs" << endl;
	cout << "input of '";
	test14->print();
	cout << "' on Ends0ANDThirdEnd1" << endl;
	cout << "result: " << Ends0ANDThirdEnd1->accepts(*test14) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on Ends0ANDThirdEnd1" << endl;
	cout << "result: " << Ends0ANDThirdEnd1->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on Ends0ANDThirdEnd1" << endl;
	cout << "result: " << Ends0ANDThirdEnd1->accepts(*test16) << endl;
	cout << "input of '";
	test4->print();
	cout << "' on Ends0ANDThirdEnd1" << endl;
	cout << "result: " << Ends0ANDThirdEnd1->accepts(*test4) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on Ends0ANDThirdEnd1" << endl;
	cout << "result: " << Ends0ANDThirdEnd1->accepts(*test17) << endl << endl;

	// testing with Ends0ANDContains1
	cout << "input of '";
	test14->print();
	cout << "' on Ends0ANDContains1" << endl;
	cout << "result: " << Ends0ANDContains1->accepts(*test14) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on Ends0ANDContains1" << endl;
	cout << "result: " << Ends0ANDContains1->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on Ends0ANDContains1" << endl;
	cout << "result: " << Ends0ANDContains1->accepts(*test16) << endl;
	cout << "input of '";
	test10->print();
	cout << "' on Ends0ANDContains1" << endl;
	cout << "result: " << Ends0ANDContains1->accepts(*test10) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on Ends0ANDContains1" << endl;
	cout << "result: " << Ends0ANDContains1->accepts(*test17) << endl;
	cout << "input of '";
	test3->print();
	cout << "' on Ends0ANDContains1" << endl;
	cout << "result: " << Ends0ANDContains1->accepts(*test3) << endl << endl;

	// testing with Contains1ANDThirdEnd1
	cout << "input of '";
	test14->print();
	cout << "' on Contains1ANDThirdEnd1" << endl;
	cout << "result: " << Contains1ANDThirdEnd1->accepts(*test14) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on Contains1ANDThirdEnd1" << endl;
	cout << "result: " << Contains1ANDThirdEnd1->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on Contains1ANDThirdEnd1" << endl;
	cout << "result: " << Contains1ANDThirdEnd1->accepts(*test16) << endl;
	cout << "input of '";
	test10->print();
	cout << "' on Contains1ANDThirdEnd1" << endl;
	cout << "result: " << Contains1ANDThirdEnd1->accepts(*test10) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on Contains1ANDThirdEnd1" << endl;
	cout << "result: " << Contains1ANDThirdEnd1->accepts(*test17) << endl;
	cout << "input of '";
	test3->print();
	cout << "' on Contains1ANDThirdEnd1" << endl;
	cout << "result: " << Contains1ANDThirdEnd1->accepts(*test3) << endl << endl;

	// testing with Ends0ANDstarts0 
	cout << "input of '";
	test14->print();
	cout << "' on Ends0ANDstarts0" << endl;
	cout << "result: " << Ends0ANDstarts0->accepts(*test14) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on Ends0ANDstarts0" << endl;
	cout << "result: " << Ends0ANDstarts0->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on Ends0ANDstarts0" << endl;
	cout << "result: " << Ends0ANDstarts0->accepts(*test16) << endl;
	cout << "input of '";
	test10->print();
	cout << "' on Ends0ANDstarts0" << endl;
	cout << "result: " << Ends0ANDstarts0->accepts(*test10) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on Ends0ANDstarts0" << endl;
	cout << "result: " << Ends0ANDstarts0->accepts(*test17) << endl;
	cout << "input of '";
	test3->print();
	cout << "' on Ends0ANDstarts0" << endl;
	cout << "result: " << Ends0ANDstarts0->accepts(*test3) << endl << endl << endl;

	// Ends0ANDstarts0ANDContains1
	cout << "input of '";
	test14->print();
	cout << "' on Ends0ANDstarts0ANDContains1" << endl;
	cout << "result: " << Ends0ANDstarts0ANDContains1->accepts(*test14) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on Ends0ANDstarts0ANDContains1" << endl;
	cout << "result: " << Ends0ANDstarts0ANDContains1->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on Ends0ANDstarts0ANDContains1" << endl;
	cout << "result: " << Ends0ANDstarts0ANDContains1->accepts(*test16) << endl;
	cout << "input of '";
	test10->print();
	cout << "' on Ends0ANDstarts0ANDContains1" << endl;
	cout << "result: " << Ends0ANDstarts0ANDContains1->accepts(*test10) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on Ends0ANDstarts0ANDContains1" << endl;
	cout << "result: " << Ends0ANDstarts0ANDContains1->accepts(*test17) << endl;
	cout << "input of '";
	test3->print();
	cout << "' on Ends0ANDstarts0ANDContains1" << endl;
	cout << "result: " << Ends0ANDstarts0ANDContains1->accepts(*test3) << endl << endl << endl;

	// testing with Ends0ThenContains1
	cout << "input of '";
	test4->print();
	cout << "' on Ends0ThenContains1" << endl;
	cout << "result: " << Ends0ThenContains1->accepts(*test4) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on Ends0ThenContains1" << endl;
	cout << "result: " << Ends0ThenContains1->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on Ends0ThenContains1" << endl;
	cout << "result: " << Ends0ThenContains1->accepts(*test16) << endl;
	cout << "input of '";
	test10->print();
	cout << "' on Ends0ThenContains1" << endl;
	cout << "result: " << Ends0ThenContains1->accepts(*test10) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on Ends0ThenContains1" << endl;
	cout << "result: " << Ends0ThenContains1->accepts(*test17) << endl << endl << endl;

	// testing with Contains1ThenContains1
	cout << "input of '";
	test4->print();
	cout << "' on Contains1ThenContains1" << endl;
	cout << "result: " << Contains1ThenContains1->accepts(*test4) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on Contains1ThenContains1" << endl;
	cout << "result: " << Contains1ThenContains1->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on Contains1ThenContains1" << endl;
	cout << "result: " << Contains1ThenContains1->accepts(*test16) << endl;
	cout << "input of '";
	test10->print();
	cout << "' on Contains1ThenContains1" << endl;
	cout << "result: " << Contains1ThenContains1->accepts(*test10) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on Contains1ThenContains1" << endl;
	cout << "result: " << Contains1ThenContains1->accepts(*test17) << endl << endl << endl;

	// Ends0ThenContains1ThenStarts0
	cout << "input of '";
	test4->print();
	cout << "' on Ends0ThenContains1ThenStarts0" << endl;
	cout << "result: " << Ends0ThenContains1ThenStarts0->accepts(*test4) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on Ends0ThenContains1ThenStarts0" << endl;
	cout << "result: " << Ends0ThenContains1ThenStarts0->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on Ends0ThenContains1ThenStarts0" << endl;
	cout << "result: " << Ends0ThenContains1ThenStarts0->accepts(*test16) << endl;
	cout << "input of '";
	test10->print();
	cout << "' on Ends0ThenContains1ThenStarts0" << endl;
	cout << "result: " << Ends0ThenContains1ThenStarts0->accepts(*test10) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on Ends0ThenContains1ThenStarts0" << endl;
	cout << "result: " << Ends0ThenContains1ThenStarts0->accepts(*test17) << endl;
	cout << "input of '";
	test7->print();
	cout << "' on Ends0ThenContains1ThenStarts0" << endl;
	cout << "result: " << Ends0ThenContains1ThenStarts0->accepts(*test7) << endl << endl << endl;

	// testing with KSEnds0
	cout << "input of '";
	test4->print();
	cout << "' on KSEnds0" << endl;
	cout << "result: " << KSEnds0->accepts(*test4) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on KSEnds0" << endl;
	cout << "result: " << KSEnds0->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on KSEnds0" << endl;
	cout << "result: " << KSEnds0->accepts(*test16) << endl;
	cout << "input of '";
	test10->print();
	cout << "' on KSEnds0" << endl;
	cout << "result: " << KSEnds0->accepts(*test10) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on KSEnds0" << endl;
	cout << "result: " << KSEnds0->accepts(*test17) << endl << endl << endl;

	// testing with KSThirdEnd1
	cout << "input of '";
	test4->print();
	cout << "' on KSThirdEnd1" << endl;
	cout << "result: " << KSThirdEnd1->accepts(*test4) << endl;
	cout << "input of '";
	test5->print();
	cout << "' on KSThirdEnd1" << endl;
	cout << "result: " << KSThirdEnd1->accepts(*test5) << endl;
	cout << "input of '";
	test16->print();
	cout << "' on KSThirdEnd1" << endl;
	cout << "result: " << KSThirdEnd1->accepts(*test16) << endl;
	cout << "input of '";
	test10->print();
	cout << "' on KSThirdEnd1" << endl;
	cout << "result: " << KSThirdEnd1->accepts(*test10) << endl;
	cout << "input of '";
	test17->print();
	cout << "' on KSThirdEnd1" << endl;
	cout << "result: " << KSThirdEnd1->accepts(*test17) << endl;
	cout << "input of '";
	test18->print();
	cout << "' on KSThirdEnd1" << endl;
	cout << "result: " << KSThirdEnd1->accepts(*test18) << endl;
	cout << "input of '";
	test19->print();
	cout << "' on KSThirdEnd1" << endl;
	cout << "result: " << KSThirdEnd1->accepts(*test19) << endl;
	cout << "input of '";
	test11->print();
	cout << "' on KSThirdEnd1" << endl;
	cout << "result: " << KSThirdEnd1->accepts(*test11) << endl << endl << endl;

	// ends0DFA
	cout << "Testing NFA to DFA function. " << endl;
	cout << "Testing input of: ";
	test10->print();
	cout << " with ends0 : " << endl;
	cout << "NFA: " << ends0->accepts(*test10) << endl;
	cout << "DFA: " << ends0DFA->accepts((String*)test10) << endl;
	cout << "Testing input of: ";
	test12->print();
	cout << " with ends0 : " << endl;
	cout << "NFA: " << ends0->accepts(*test12) << endl;
	cout << "DFA: " << ends0DFA->accepts((String*)test12) << endl;
	cout << "Testing input of: ";
	zeroDfaTest->print();
	cout << " with ends0 : " << endl;
	cout << "NFA: " << ends0->accepts((OneString&)*zeroDfaTest) << endl;
	cout << "DFA: " << ends0DFA->accepts((String*)zeroDfaTest) << endl;
	cout << "Testing input of: ";
	test5->print();
	cout << " with ends0 : " << endl;
	cout << "NFA: " << ends0->accepts(*test5) << endl;
	cout << "DFA: " << ends0DFA->accepts((String*)test5) << endl;
	cout << "Testing input of: ";
	test14->print();
	cout << " with ends0 : " << endl;
	cout << "NFA: " << ends0->accepts(*test14) << endl;
	cout << "DFA: " << ends0DFA->accepts((String*)test14) << endl;
	cout << "Testing input of: ";
	test4->print();
	cout << " with ends0 : " << endl;
	cout << "NFA: " << ends0->accepts(*test4) << endl;
	cout << "DFA: " << ends0DFA->accepts((String*)test4) << endl << endl;
	
	// thirdEnd1DFA
	cout << "Testing NFA to DFA function. " << endl;
	cout << "Testing input of: ";
	test5->print();
	cout << " with thirdFromEnd1 : " << endl;
	cout << "NFA: " << thirdFromEnd1->accepts(*test5) << endl;
	cout << "DFA: " << thirdEnd1DFA->accepts((String*)test5) << endl;
	cout << "Testing input of: ";
	test16->print();
	cout << " with thirdFromEnd1 : " << endl;
	cout << "NFA: " << thirdFromEnd1->accepts(*test16) << endl;
	cout << "DFA: " << thirdEnd1DFA->accepts((String*)test16) << endl;
	cout << "Testing input of: ";
	test10->print();
	cout << " with thirdFromEnd1 : " << endl;
	cout << "NFA: " << thirdFromEnd1->accepts(*test10) << endl;
	cout << "DFA: " << thirdEnd1DFA->accepts((String*)test10) << endl;
	cout << "Testing input of: ";
	test17->print();
	cout << " with thirdFromEnd1 : " << endl;
	cout << "NFA: " << thirdFromEnd1->accepts(*test17) << endl;
	cout << "DFA: " << thirdEnd1DFA->accepts((String*)test17) << endl;
	cout << "Testing input of: ";
	test18->print();
	cout << " with thirdFromEnd1 : " << endl;
	cout << "NFA: " << thirdFromEnd1->accepts(*test18) << endl;
	cout << "DFA: " << thirdEnd1DFA->accepts((String*)test18) << endl;
	cout << "Testing input of: ";
	test19->print();
	cout << " with thirdFromEnd1 : " << endl;
	cout << "NFA: " << thirdFromEnd1->accepts(*test19) << endl;
	cout << "DFA: " << thirdEnd1DFA->accepts((String*)test19) << endl;
	cout << "Testing input of: ";
	test11->print();
	cout << " with thirdFromEnd1 : " << endl;
	cout << "NFA: " << thirdFromEnd1->accepts(*test11) << endl;
	cout << "DFA: " << thirdEnd1DFA->accepts((String*)test11) << endl << endl;


	cout << "testing equality of 2 DFAs but using NFAtoDFA function" << endl;
	bool example1 = equalityDFA(thirdEnd1DFA, ends0DFA);
	bool example2 = equalityDFA(thirdEnd1DFA, thirdEnd1DFA);
	cout << "thirdEnd1DFA equal to ends0DFA: " << example1 << endl;
	cout << "thirdEnd1DFA equal to thirdEnd1DFA: " << example2 << endl << endl;


	// Testing Regular expressions
	//	A((B U C))*
	concatRegex* re1 = new concatRegex(
		new charRegex(Char('A')), 
		new starRegex(
			new unionRegex(
				new charRegex(Char('B')), 
				new charRegex(Char('C')))));
	//	ABC
	concatRegex* re2 = new concatRegex(
		new charRegex(Char('A')), 
		new concatRegex(
			new charRegex(Char('B')), 
			new charRegex(Char('C'))));
	//	((A U BC)* U D(E)*)
	unionRegex* re3 = new unionRegex(
		new starRegex(
			new unionRegex(
				new charRegex(Char('A')),
				new concatRegex(
					new charRegex(Char('B')), 
					new charRegex(Char('C'))))), 
		new concatRegex(
			new charRegex(Char('D')), 
			new starRegex(
				new charRegex(Char('E')))));
	//	(HELLO)*
	starRegex* re4 = new starRegex(
		new concatRegex(
			new concatRegex(
				new charRegex(Char('H')),
				new charRegex(Char('E'))),
			new concatRegex(
				new concatRegex(
					new charRegex(Char('L')),
					new charRegex(Char('L'))),
				new charRegex(Char('O')))));

	//	((0 U 1))*
	starRegex* re5 = new starRegex(
		new unionRegex(
			new charRegex(Char('0')),
			new charRegex(Char('1'))));

	//	(01)*
	starRegex* re6 = new starRegex(
		new concatRegex(
			new charRegex(Char('0')),
			new charRegex(Char('1'))));

	//	(((0 U 1))* U (01)*)
	unionRegex* re7 = new unionRegex(re5, re6);

	// 0
	charRegex* re8 = new charRegex(Char('0'));

	//	(0 U 1)
	unionRegex* re9 = new unionRegex(
		new charRegex(Char('0')),
		new charRegex(Char('1')));

	//	01
	concatRegex* re10 = new concatRegex(
		new charRegex(Char('0')),
		new charRegex(Char('1')));

	//	0*
	starRegex* re11 = new starRegex(
		new charRegex(Char('0'))
	);

	// accepting: 1,3		rejecting: 2,4,5,6,7
	OneString* reTest1 = new OneString(Char('A'), new OneString(Char('B'), new epsilon()));

	// accepting: 1,3		rejecting: 2,4,5,6,7
	OneString* reTest2 = new OneString(Char('A'), new OneString(Char('C'), new epsilon()));

	// accepting: 3			rejecting: 1
	OneString* reTest3 = new OneString(Char('B'), new OneString(Char('C'), new epsilon()));

	// accepting: 			rejecting: 1,2,3,4,5,6,7
	OneString* reTest4 = new OneString(Char('C'), new OneString(Char('B'), new epsilon()));

	// accepting: 2			rejecting: 1,3,4,5,6,7
	OneString* reTest5 = new OneString(Char('A'), new OneString(Char('B'), new OneString(Char('C'), new epsilon())));

	// accepting: 3			rejecting: 1,2,4,5,6,7
	OneString* reTest6 = new OneString(Char('D'), new OneString(Char('E'), new epsilon()));
	
	// accepting: 4			rejecting: 1,2,3,5,6,7
	OneString* reTest7 = new OneString(Char('H'), new OneString(Char('E'), new OneString(Char('L'), new OneString(Char('L'), new OneString(Char('O'), new epsilon())))));

	// accepting: 5,7		rejecting: 1,2,3,4,6
	OneString* reTest8 = new OneString(Char('0'), new epsilon());

	// accepting: 5,7		rejecting: 1,2,3,4,6
	OneString* reTest9 = new OneString(Char('1'), new epsilon());

	// accepting: 6,7		rejecting: 1,2,3,4,5
	OneString* reTest10 = new OneString(Char('0'), new OneString(Char('1'), new epsilon()));


	cout << "Testing regular expressions:" << endl;
	cout << "Examples of regular expressions:" << endl;
	re1->print();
	cout << endl;
	re2->print();
	cout << endl;
	re3->print();
	cout << endl;
	re4->print();
	cout << endl;
	re5->print();
	cout << endl;
	re6->print();
	cout << endl;
	re7->print();
	cout << endl;


	// testing generator
	cout << endl << "Testing generator function for each regex:" << endl;
	cout << "Testing with: ";
	re1->print();
	cout << endl;
	OneString* gen1 = re1->generate();
	gen1->print();
	cout << endl;
	cout << "Testing with: ";
	re2->print();
	cout << endl;
	OneString* gen2 = re2->generate();
	gen2->print();
	cout << endl;
	cout << "Testing with: ";
	re3->print();
	cout << endl;
	OneString* gen3 = re3->generate();
	gen3->print();
	cout << endl;
	cout << "Testing with: ";
	re4->print();
	cout << endl;
	OneString* gen4 = re4->generate();
	gen4->print();
	cout << endl;
	cout << "Testing with: ";
	re5->print();
	cout << endl;
	OneString* gen5 = re5->generate();
	gen5->print();
	cout << endl;
	cout << "Testing with: ";
	re6->print();
	cout << endl;
	OneString* gen6 = re6->generate();
	gen6->print();
	cout << endl;
	cout << "Testing with: ";
	re7->print();
	cout << endl;
	OneString* gen7 = re7->generate();
	gen7->print();
	cout << endl << endl;

	// converting and testing turning a regex into an NFA
	// 0
	cout << "Testing Regex to NFA function with NFAaccepted: " << endl;
	bool init = 0;
	char start = 'a';
	cout << "testing regex: ";
	re8->print();
	cout << endl << "with input: ";
	test1->print();
	auto regNFA1a = RegexToNFA(re8, start);
	init = regNFA1a->accepts(*test1);
	cout << endl << "accepted: " << init << endl << endl;

	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re8->print();
	cout << endl << "with input: ";
	test17->print();
	auto regNFA1b = RegexToNFA(re8, start);
	init = regNFA1b->accepts(*test17);
	cout << endl << "accepted: " << init << endl << endl;

	// (0 U 1)
	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re9->print();
	cout << endl << "with input: ";
	test1->print();
	auto regNFA2a = RegexToNFA(re9, start);
	init = regNFA2a->accepts(*test1);
	cout << endl << "accepted: " << init << endl << endl;

	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re9->print();
	cout << endl << "with input: ";
	test17->print();
	auto regNFA2b = RegexToNFA(re9, start);
	init = regNFA2b->accepts(*test17);
	cout << endl << "accepted: " << init << endl << endl;

	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re9->print();
	cout << endl << "with input: ";
	test10->print();
	auto regNFA2c = RegexToNFA(re9, start);
	init = regNFA2c->accepts(*test10);
	cout << endl << "accepted: " << init << endl << endl;

	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re9->print();
	cout << endl << "with input: ";
	test20->print();
	auto regNFA2d = RegexToNFA(re9, start);
	init = regNFA2d->accepts(*test20);
	cout << endl << "accepted: " << init << endl << endl;

	// 01
	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re10->print();
	cout << endl << "with input: ";
	test5->print();
	auto regNFA3a = RegexToNFA(re10, start);
	init = regNFA3a->accepts(*test5);
	cout << endl << "accepted: " << init << endl << endl;

	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re10->print();
	cout << endl << "with input: ";
	test4->print();
	auto regNFA3b = RegexToNFA(re10, start);
	init = regNFA3b->accepts(*test4);
	cout << endl << "accepted: " << init << endl << endl;

	//	(0)*
	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re11->print();
	cout << endl << "with input: ";
	zeroDfaTest->print();
	auto regNFA4a = RegexToNFA(re11, start);
	init = regNFA4a->accepts((OneString&)*zeroDfaTest);
	cout << endl << "accepted: " << init << endl << endl;

	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re11->print();
	cout << endl << "with input: ";
	test1->print();
	auto regNFA4b = RegexToNFA(re11, start);
	init = regNFA4b->accepts(*test1);
	cout << endl << "accepted: " << init << endl << endl;

	init = 0;
	start = 'a';
	cout << "testing regex: ";
	re11->print();
	cout << endl << "with input: ";
	test5->print();
	auto regNFA4c = RegexToNFA(re11, start);
	init = regNFA4c->accepts(*test5);
	cout << endl << "accepted: " << init << endl << endl;
}
