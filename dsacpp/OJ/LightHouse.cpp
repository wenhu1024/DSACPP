#include <iostream>
#include <cstdio>

using namespace std;
using ll=long long;
struct s{
    int x;
    int y;
};
using point=s;
point *P;

void merge(int lo,int mi,int hi){
    point *A=P+lo;

    int lb=mi-lo;
    point *B=new point[lb];
    for(int i=0;i<lb;++i){
        B[i]=A[i];
    }

    int lc=hi-mi;
    point *C=P+mi;

    for(int i=0,j=0,k=0;j<lb;){
        if( (k>=lc) ||(B[j].x< C[k].x) ) { A[i++]=B[j++]; }
        if( (k<lc) && (C[k].x< B[j].x) ) { A[i++]=C[k++]; }
    }
    delete [] B;
}

void mergeSort(int lo,int hi){
    if(hi-lo<2) return;
    int mi=lo+( (hi-lo)>>1 );
    mergeSort(lo,mi);
    mergeSort(mi,hi);
    merge(lo,mi,hi);
}

ll invBetween(int lo,int mi,int hi){
    ll cnt=0;
    point *A=P+lo;

    int lb=mi-lo;
    point *B=new point[lb];
    for(int i=0;i<lb;++i){
        B[i]=A[i];
    }

    int lc=hi-mi;
    point *C=P+mi;

    for(int i=0,j=0,k=0;j<lb;){
        if( (k>=lc) ||(B[j].y<C[k].y) ) {  
            A[i++]=B[j++]; 
            if(k<lc){
                cnt+=lc-k;
            }   
        }
        if( (k<lc) && (C[k].y<B[j].y) ) {  
            A[i++]=C[k++];
            
        }
    }
    delete [] B;
    return cnt;
}

ll invInside(int lo,int hi){
    ll ans=0;
    if(hi-lo<2) return ans;

    int mi=lo+((hi-lo)>>1);
    ans+=invInside(lo,mi);
    ans+=invInside(mi,hi);
    ans+=invBetween(lo,mi,hi);
    
    return ans;
}


int main()
{
    int n;
    cin>>n;

    P=new point[n];
    
    for(int i=0;i<n;++i){
        scanf("%d %d",&P[i].x,&P[i].y);
    }

    mergeSort(0,n);

    printf("%lld\n",invInside(0,n));

    delete [] P;
    return 0;
}