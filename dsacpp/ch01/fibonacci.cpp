#include <iostream>
#include <algorithm>

using namespace std;

/**
 * fibnonacci:
 * 0,1,1,2,3,5,...
 * use n to represent the order of the number
 * n starts from 0 
*/

int fib0(int n){
//divide-and-conquer
    return n>2?
            n:
            fib0(n-1)+fib0(n-2);
}

int fib1(int n){
//iterator-while()
    int g=0,f=1;
    while(n-->0){
        g+=f;
        f=g-f;
    }
    return g;
}
int main()
{

//fib0:
    cout << fib0(5) << endl;
//fib1:
    cout << fib1(5) << endl;;

    return 0;
}