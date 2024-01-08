#include <iostream>
#include <algorithm>

using namespace std;

void bubblesort1A(int A[],int n){
    bool sorted = false;
    while(!sorted){
        sorted=true;
        for(int i=1;i<n;++i){
            if(A[i-1]>A[i]){
                swap(A[i-1],A[i]);                
                sorted=false;
            }
        }
        n--;
    }
}


void bubblesort1B(int A[],int n){
    for(bool sorted=false;sorted=!sorted;n--){
        for(int i=1;i<n;++i){
            if(A[i-1]>A[i]){
                swap(A[i-1],A[i]);
                sorted=false;
            }
        }
    }
}
int main(){
    int num1[6]={9,2,3,10,7,6};
    int num2[6]={9,2,3,10,7,6};
    int num3[6]={9,2,3,10,7,6};


//bubblesort1A:

    bubblesort1A(num1,6);
    for(auto e:num1){
        cout << e << endl;
    }
cout << "__________" << endl;

//bubblesort1B:
    bubblesort1B(num2,6);
    for(auto e:num1){
        cout << e << endl;
    }
    return 0;
}