#include <iostream>
#include <cstdio>

using namespace std;

int n,m,a,b;
void merge(int *indexs,int lo,int mid,int hi){
    int *A=indexs+lo;

    int lb=mid-lo;
    int *B=new int[lb];
    for(int i=0;i<lb;++i){B[i]=A[i];}

    int lc=hi-mid;
    int *C=indexs+mid;

    for(int i=0,j=0,k=0; (j < lb) || (k < lc);){
        if( (j < lb) &&( (lc <=k )|| ( B[j]<=C[k]) )) {A[i++]=B[j++];}
        if( (k < lc) &&( (lb <=j )|| ( C[k]<B[j]) )) {A[i++]=C[k++];}
    }
    delete []B;
}

void mergeSort(int *indexs,int lo,int hi){
    if(hi-lo<2) return;

    int mid=lo+ ( (hi-lo)>>1 );
    mergeSort(indexs,lo,mid); mergeSort(indexs,mid,hi);
    merge(indexs,lo,mid,hi);
}


int leftFound(int *indexs,int e){
    int lo=0,hi=n;
    int mid;
    while(lo<hi){
        mid=lo+((hi-lo)>>1);
        (indexs[mid] < e) ? lo=mid+1 : hi=mid; 
    }
    return lo;
}

int rightFound(int *indexs,int e){
    int lo=0,hi=n;
    int mid;
    while(lo<hi){
        int mid=lo+((hi-lo)>>1);
        (e < indexs[mid]) ?  hi=mid: lo=mid+1;
    }
    return --lo;
}
int main(){
    
    cin >> n >> m;
    int *indexs=new int[n];
    for(int i=0;i<n;++i){
        scanf("%d",&indexs[i]);
    }

    mergeSort(indexs,0,n);
    int cnt = 0;
    while(0<m--){
        scanf("%d %d",&a,&b);
        cnt=rightFound(indexs,b)-leftFound(indexs,a)+1;
        printf("%d\n",cnt);
        cnt=0;
    }

    delete []indexs;
    return 0;
}

