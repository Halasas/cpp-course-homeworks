#include "set_restore_guard.h"
#include <iostream>

using namespace std;

void f(int & var) {

	cout << var << endl;
	set_restore_guard<int> g(var, 44);
	cout << var << endl;
	var = 13;
	cout << var << endl;

}
struct a {
	a() = delete;
	a(int const &x) {
		f = x;
	}
	a& operator=(a const& other) 
	{
		this->f = other.f;
		return *this;
	}
	int f;
};

std::ostream &operator<<(std::ostream& s, a const& x) {
	return s << x.f;
}

int main() {
	int var = 33;
	f(var);
	cout << var << endl;
	cout << " ----- " << endl;

	set_restore_guard<int> g(var, 44);
	if (g)
		cout << "is_stores = true" << endl;
	cout << var << endl;
	cout << g.old_value() << endl;
	g.restore();
	if (g)
		cout << "is_stores = true" << endl;
	g.set(var, 55);
	cout << var << endl;
	cout << g.old_value() << endl;
	g.set(var, 44);
	cout << var << endl;
	cout << g.old_value() << endl;

	a nvar = 44;
	set_restore_guard<a> ng;
	ng.set(nvar, 55);
}