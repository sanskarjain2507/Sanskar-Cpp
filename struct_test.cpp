#include<iostream>

#define func1(x) #x

using namespace std;

 typedef struct s1{
    int ac;
    int b;
 }s1;

 typedef struct s2{
    int c;
    int d;
 }s2;

 typedef struct s3{
    int e;
    int f;
 }s3;

 typedef struct s{
    s1 A;
    s2 B;
    s3 C;
 }s;




 int main()
 {
    s ab;
    ab.A.ac = 10;
    cout<<ab.A.func1(ac);
 }

 