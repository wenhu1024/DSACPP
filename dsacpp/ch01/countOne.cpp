#include <iostream>
using namespace std;

int countOne(unsigned int n){
    //r:the number of the bits of n    O(2^r)
    int ones=0;
    while(n>0){
        if(n&1==1){
            ones++;
        }
        n>>=1;
    }
    return ones;
}

int countOne1(unsigned int n){
    //r:the number of the bits 1 of n    O(r)
    int ones=0;
    while(n>0){
        ones++;
        n&=n-1;
    }
    return ones;
}
int main(){
    cout<<countOne(15)<<endl;
    cout<<countOne1(16)<<endl;
}