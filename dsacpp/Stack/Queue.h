#include "../List/List.h"

template <typename T>   //以List为基类
class Queue:public List<T>{
public: //size(),empty()等接口可以直接沿用
    void enqueue(T const &e) { this->insertAsLast(e);}  //入队：尾部插入
    T dequeue() {return this->remove(this->first());}   //出队：首部删除
    T& front() {return this->first()->data;}    //队首
};