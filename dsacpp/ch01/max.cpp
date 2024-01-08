#include <iostream>
#include <climits>
using namespace std;

int maxI(int A[],int n){
    int m=INT_MIN;
    for(int i=0;i<n;++i){
        m=max(m,A[i]);
    }
    return m;
}

int maxR1(int A[],int n){
    if(n<2){
        return A[n-1];
    }
    return max(maxR1(A,n-1),A[n-1]);
}

int maxR2(int A[],int lo,int hi){
    //A[lo,hi)
    if(lo+1==hi){
        return A[lo];
    }
    else{
        int mid=lo+((hi-lo)>>1);
        return max(maxR2(A,lo,mid),maxR2(A,mid,hi));
    }

}
int main(){
    int nums[5]={2,6,1,4,9};
    cout<<maxI(nums,5)<<endl;
    cout<<maxR1(nums,5)<<endl;
    cout<<maxR2(nums,0,5)<<endl;
    return 0;
}