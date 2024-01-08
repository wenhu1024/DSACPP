#include "BST.h"

template <typename T>
class RedBlack : public BST<T>
{
public:
    // BST<T>::search()等其余接口，可直接沿用
    BinNode<T> *insert(T const &e); //  插入(重写)
    bool remove(T const &e);        //  删除(重写)
protected:
    void solveDoubleRed(BinNode<T> *x);   //  双红修正
    void solveDoubleBlack(BinNode<T> *x); //  双黑修正
    int updateHeight(BinNode<T> *x);      //  更新节点x的高度 (黑高度)
};

template <typename T>
inline bool IsBlack(BinNode<T> *p)
{
    //  外部节点也视为黑节点
    return !p || p->color == RBColor::RB_BLACK;
}

template <typename T>
inline bool IsRed(BinNode<T> *p)
{
    return !IsBlack(p);
}

template <typename T>
int RedBlack<T>::updateHeight(BinNode<T> *x)
{
    x->height = max(stature(x.lc), stature(x.rc));
    return IsBlack(x) ? x->height++ : x->height;
    //  stature(nullptr)=-1,外节点的height为-1，比实际黑高度0少1，但不影响后续算法
}

template <typename T>
inline bool BlackUpdated(BinNode<T> *x)
{
    //  RedBlack高度已更新条件
    return (stature(x->lc) == stature(x->rc)) && x->height == (IsRed(&x) ? stature(x->lc) : stature(x->lc) + 1);
}

template <typename T>
BinNode<T> *
RedBlack<T>::insert(T const &e)
{
    BinNode<T> *&x = search(e);
    if (x)
    {
        return x;
    }

    x = new BinNode(e, _hot, nullptr, nullptr, -1); // 黑高度为-1
    _size++;

    //  如有必要,需要做双红修正
    solveDoubleRed(x);

    return x ? x : _hot->parent;
} //  最后返回必有x->date==e;

template <typename T>
void 
RedBlack<T>::solveDoubleRed(BinNode<T> *p)
{
    //
}