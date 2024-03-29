//compie with g++ -std=c++20 binary_sempahore.cpp

// C++ program to illustrate the binary semaphores 
#include <iostream> 
#include <semaphore> 
#include <thread> 
using namespace std; 

// Initialize with a count of 1 (binary) 
binary_semaphore semaphore(1); 

void worker(int id) 
{ 
	// aquire semaphore 
	semaphore.acquire(); 
	cout << "Thread " << id << " acquired the semaphore."
		<< endl; 

	// Do some work 
	semaphore.release(); 
	// release 
	cout << "Thread " << id << " released the semaphore."
		<< endl; 
} 

// driver code 
int main() 
{ 
	thread t1(worker, 1); 
	thread t2(worker, 2); 
	t1.join(); 
	t2.join(); 
	return 0; 
}
