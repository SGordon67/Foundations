#include <iostream>
#include <list>

class string {
	// Test commit :)
};
class Object {
public:
	Object(char x) {
		this->a = x;
	}
	char a;
};
class MTString : public string {
	MTString() {

	}
};
class OneString : public string {
	OneString(Object a, string b) {

	}

};

int main()
{
	std::list<Object> myList;
	Object zero = Object('0');
	Object one = Object('1');
	myList.push_back(zero);
	myList.push_back(one);

	std::cout << "this is the first item\n";

	for (auto v : myList) {
		std::cout << v.a << "\n";
	}
}
