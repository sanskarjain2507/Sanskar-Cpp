#include<iostream>
#include "factorial.hpp"

using namespace std;

int factorial(int number) {
    if (number < 0) 
    {
    throw std::invalid_argument("negative number not allowed");
    }
    
    return number <= 1 ? 1 : factorial(number - 1) * number; 
}

