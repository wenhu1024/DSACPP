#include <iostream>
#include <algorithm>

using namespace std;


int sum0(int A[],int n){
    int sum=0;
    for(int i=0;i<n;++i){
        sum+=A[i];
    }
    return sum;
}

int sum1(int A[],int n){
    //decrease-and-conquer
    if(n<1){
        return 0;
    }else{
        return sum1(A,n-1)+A[n-1];
    }
}

int sum2(int A[],int lo,int hi){
    //divide-and-conquer
    if(lo==hi){
        return A[lo];
    }
    else{
        int mid=(lo+hi)>>1;
        return sum2(A,lo,mid)+sum2(A,mid+1,hi);
    }
}
int main()
{
    int num1[6]={9,2,3,10,7,6};

//sum0:
    cout << "sum0:" << sum0(num1,6) << endl;

//sum1:
    cout << "sum1:" << sum1(num1,6) << endl;

//sum2:
    cout << "sum2:" << sum2(num1,0,6-1) << endl;
    return 0;
}