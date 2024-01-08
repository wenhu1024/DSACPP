#include <iostream>
using namespace std;

int power(int a,int n){ 
    //a^n  O(logn)
    //r:the number of the bits of n    O(r)=O(logn)
    int pow=1;
    int p=a;
    while(n>0){
        if(n&1==1){
            pow*=p;
        }
        n>>=1;
        p*=p;
    }
    return pow;
}

int power1(int n){
    //2^n  
    //r:the number of the bits of n    O(2^r)
    int pow=1;
    while(n>0){
        pow*=2;
        n--;
    }
    return pow;
}

inline int sqr(int a){return a*a;}
int power2(int n){
    //2^n 
    //recursion
    //r:the number of the bits of n    O(r)=O(logn)
    if(n==0){
        return 1;
    }
    return (n&1==1)? sqr(power2(n>>1))<<1 :sqr(power2(n>>1));
}
int main(){
    cout<<power(3,3)<<endl;
    cout<<power1(3)<<endl;
    cout<<power2(5)<<endl;
    return 0;
}