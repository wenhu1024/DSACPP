#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

int _size,_capacity;

void insert(char *_elem,int r,char e){
    for(int i=_size;i>r;--i){
        _elem[i]=_elem[i-1];
    }
    _elem[r]=e;
    _size++;
}

bool check(char *_elem){
    for(int i=0;i<_size;++i){
        int j=i+1;
        while( (j<_size)&&(_elem[j] == _elem[i]) ){
            ++j;
        }
        if( j-i >= 3){
           return true;
        }
    }
    return false;
}
void remove(char *_elem){
    for(int i=0;i<_size;++i){
        int j=i+1;
        while( (j<_size)&&(_elem[j] == _elem[i]) ){
            ++j;
        }
        if( j-i >= 3){
            int lo=i,hi=j;
            while(hi<_size){
                _elem[lo++]=_elem[hi++];
            }
            _size=lo;
        }
    }
}

void putout(char *_elem){
    if(_size==1){printf("-");}
    else{
        printf("%s",_elem);
    }   
    printf("\n");
}

int main(){
    string s;
    getline(cin,s);
    
    int n;
    cin>>n;
    _capacity=s.length()+n+1;
    char *_elem=new char[_capacity];

    strcpy(_elem,s.c_str());
    _size=_capacity-n;

    int r;
    char e;
    while(0<n--){
        scanf("%d %c",&r,&e);
        insert(_elem,r,e);
        while(check(_elem)) remove(_elem);
        putout(_elem);
    }
    delete [] _elem;
    return 0;
}