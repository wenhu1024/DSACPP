#include "Stack.h"
#include "StackB.h"

void convertA(Stack<char> &S,int n,int base){
    //十进制正整数n到base进制的转换（递归版）
    static char digital[] //n>0, 1<base<=16,若base更大，可以适当扩充digital数组
    = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    if(0<n){    //还有余数之前，反复地
        S.push(digital[n%base]);    //逆向记录最低位
        convertA(S,n/base,base);    //通过递归得到所有最高位
    }
}//新进制下由高到低的各位数，自顶向下保存于栈S中

void convertB(StackB<char> &S,int n,int base){
    //十进制正整数n到base进制的转换（迭代版）
    static char digital[] //n>0, 1<base<=16,若base更大，可以适当扩充digital数组
    = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    while(0<n){
        S.push(digital[n%base]);
        n/=base;
    }
}//新进制下由高到低的各位数，自顶向下保存于栈S中


int main()
{
    Stack<char> s1;
    StackB<char> s2;
    int v,base;
    cin>>v>>base;
    convertA(s1,v,base);
    while(!s1.empty()) cout<<s1.pop();
    cout<<endl;
    convertB(s2,v,base);
    while(!s2.empty()) cout<<s2.pop();
    return 0;
}