#include "Stack.h"
#include <string>
//表达式括号匹配，可以兼顾3种括号: (),[],{}
bool paren(const char exp[],int lo,int hi){ //exp[lo,hi) 
    Stack<char> S;
    for(int i=lo; i<hi; ++i){
        switch (exp[i]){
            case '(':
            case '[':
            case '{':
                S.push(exp[i]);
                break;
            case ')':
                if(S.empty() || '(' != S.pop()){ // 注意这里的|| ，短路性质
                    return false;
                }
                break;
            case ']':
                if(S.empty() || '[' != S.pop()){
                    return false;
                }
                break;        
            case '}':
                if(S.empty() || '{' != S.pop()){
                    return false;
                }
                break;
            default:
                break;  //非括号字符，一律忽视
        }
    }
    return S.empty();
}

int main(){
    string str;
    cin>>str;
    cout<<paren(str.c_str(),0,str.length())<<endl;
    printf("%d\n", paren("{()[({})]]", 0, 10));
    printf("%d\n", paren("{()[({})]}", 0, 10));
    return 0;
}

