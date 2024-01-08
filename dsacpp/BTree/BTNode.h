#include "../Vector/Vector.h"

template <typename T>
struct BTNode
{
    //  成员
    BTNode<T> *parent;         //  父节点
    Vector<T> key;             //  关键码向量
    Vector<BTNode<T> *> child; //  孩子向量(其长度总比key多1)
    

    //  构造函数(注意：BTNode只能作为根节点创建，而且初始时是0个关键码，以及一个空孩子指针)
    BTNode()
    {
        parent = nullptr;
        child.insert(0, nullptr);   //  注意这里，已经有一个空指针，child.size()初始即为1
    }
    BTNode(T e, BTNode<T> *lc = nullptr, BTNode<T> *rc = nullptr)
    {
        parent = nullptr;    //  作为根节点，而且初始时
        key.insert(0, e);    //  只有1个关键码e
        child.insert(0, lc); //  和两个孩子
        child.insert(1, rc);
        if (lc)
        {
            lc->parent = this;
        }
        if (rc)
        {
            rc->parent = this;
        }
    }
};