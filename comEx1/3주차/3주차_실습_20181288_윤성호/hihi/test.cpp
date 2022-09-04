#include <iostream>
	
using namespace std;
	
class Base
{
private:
		int *b;
	public:
		Base() { 
			cout << "Base Construction" << endl;
			b = new int[10];
		}
		~Base() { 
			cout << "Base Destruction" << endl;
			delete [] b;
		}
};
	
class Derived : public Base
{
private:
	int *d;
public:
		Derived() {
			cout << "Derived Construction" << endl;
			d = new int[5];
		}
		~Derived() {
			cout << "Derived Destruction" << endl;
			delete [] d;
		}
};

int main(void)
{
	Derived p;
}
