#include <stdio.h>
#include <unistd.h>


int main() {
    
    int f=0;
    int k=0;
    for(int i=0;i<1000;i++)
    {
        f+=1;
        k+=f;
    }


    return 0;
}
