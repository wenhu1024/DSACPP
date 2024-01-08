#include <stack>
#include <iostream>

using namespace std;

template <typename T>
bool stackWashing(stack<T> A,stack<T> B){
    stack<T> S,RB;

    while(!B.empty()){
        RB.push(B.top());
        B.pop();
    }

    while(!A.empty()){
        S.push(A.top());
        A.pop();

        if(S.top()==RB.top()){
            while(!S.empty() && S.top() == RB.top()){
                S.pop();
                RB.pop();
            }
        }
    }
    return S.empty();
}

int main()
{
    stack<int> a;
    a.push(4);
    a.push(3);
    a.push(2);
    a.push(1);

    stack<int> b;
    b.push(4);
    b.push(2);
    b.push(3);
    b.push(1);

    stack<int> c;
    c.push(1);
    c.push(2);
    c.push(3);
    c.push(4);

    cout<<stackWashing(a,b)<<endl;
    cout<<stackWashing(a,c)<<endl;
    
    return 0;
}