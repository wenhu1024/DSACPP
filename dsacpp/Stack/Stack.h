#include "../Vector/Vector.h" //以向量为基类，派生出栈模板类

template <typename T> class Stack:public Vector<T>{ //将向量的首/末端作为栈底/顶
public: //size(),empty()以及其他开放接口，均可直接沿用
    void push(T const &e) {this->insert(this->size(),e);}   //入栈
    T pop() {return this->remove(this->size()-1);}                 //出栈
    T& top() {return (*this)[this->size()-1];}        //取顶
};

