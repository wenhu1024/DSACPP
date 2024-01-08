#ifndef FIB_H
#define FIB_H
using Rank=int;

class Fib{
private:
    Rank f,g;   //f=fib(k-1),g=fib(k),int可能溢出
public:
    Fib(Rank n) //初始化，使得g为不小于n的最小fib项
    {
        f=1,g=0;    //fib(-1)=1,fib(0)=0;
        while(g<n){
            next();
        }
    }

    Rank get(){return g;}
    Rank next(){g+=f;  f=g-f; return g;}
    Rank prev(){f=g-f; g-=f;  return g;}
};
#endif