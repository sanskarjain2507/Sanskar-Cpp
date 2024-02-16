// C++ program to demonstrate
// multithreading using three
// different callables.
#include <iostream>
#include <thread>
#include<unistd.h>
using namespace std;

// A dummy function
void foo(int Z,int *q)
{
	for (int i = 0; i < Z; i++) {
		cout << "Thread using function"
				" pointer as callable"<<*q<<" \n";
        sleep(1);
	}
}

// A callable object
class thread_obj {
public:
	void operator()(int x)
	{
		for (int i = 0; i < x; i++)
        {
			cout << "Thread using function"
					" object as callable\n";
            sleep(1);
        }
	}
};

// class definition
class Base {
public:
	// non-static member function
	void foo(int* p)
	{
		cout << "Thread using non-static member function "
				"as callable"
			<< endl;
	}
	// static member function
	static void foo1()
	{
		cout << "Thread using static member function as "
				"callable"
			<< endl;
	}
};

// Driver code
int main()
{
	cout << "Threads 1 and 2 and 3 "
			"operating independently"
		<< endl;

	// This thread is launched by using
	// function pointer as callable
    int q=9;
	thread th1(foo, 3,&q);

	// This thread is launched by using
	// function object as callable
	thread th2(thread_obj(), 3);

	// Define a Lambda Expression
	auto f = [](int x) {
		for (int i = 0; i < x; i++)
        {
			cout << "Thread using lambda"
					" expression as callable\n";
            sleep(1);
        }
	};

	// This thread is launched by using
	// lambda expression as callable
	thread th3(f, 3);

	// object of Base Class
	Base b;

	thread th4(&Base::foo, &b,&q);

	thread th5(&Base::foo1);

	// Wait for the threads to finish
	// Wait for thread t1 to finish
	th1.join();

	// Wait for thread t2 to finish
	th2.join();

	// Wait for thread t3 to finish
	th3.join();

	// Wait for thread t4 to finish
	th4.join();

	// Wait for thread t5 to finish
	th5.join();

	return 0;
}
