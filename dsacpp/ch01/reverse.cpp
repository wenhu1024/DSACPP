#include <iostream>
#include <algorithm>
using namespace std;

//A[lo,hi]
void reverse0(int A[],int lo,int hi){
//recursion
    if(lo<hi){
        swap(A[lo],A[hi]);
        reverse0(A,lo+1,hi-1);
    }    
}

void reverse1(int A[],int lo,int hi){
//iterator-while()
    while(lo<hi){
        swap(A[lo++],A[hi--]);
    }    
}
int main()
{
    int num1[6]={1,2,3,4,5,6};
    int num2[6]={1,2,3,4,5,6};

//reverse0:
    reverse0(num1,0,6-1);
    for(auto e:num1){
        cout << e <<"  ";
    }
    cout << endl;


//reverse1:
    reverse1(num2,0,6-1);
    for(auto e:num2){
        cout << e <<"  ";
    }
    cout << endl;
    return 0;
}