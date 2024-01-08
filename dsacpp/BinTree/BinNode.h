#include "../Stack/Stack.h"
#include "../Stack/Queue.h"

typedef enum
{
    RB_RED,
    RB_BLACK
} RBColor; // 节点颜色

template <typename T>
struct BinNode
{
    T data;             // 数据
    BinNode<T> *parent; // 父节点
    BinNode<T> *lc;     // 左孩子
    BinNode<T> *rc;     // 右孩子
    int height;         // 高度(通用)
    int npl;            // Null Path Length (左式堆，也可直接用height代替)
    RBColor color;      // 颜色(红黑树)

    // 构造函数
    BinNode() : parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1), color(RB_RED) {}
    BinNode(T e, BinNode<T> *p = nullptr, BinNode<T> *lc = nullptr, BinNode<T> *rc = nullptr,
            int h = 0, int l = 1, RBColor c = RBColor::RB_RED) : data(e), parent(p), lc(lc), rc(rc),
                                                                 height(h), npl(l), color(c) {}

    // 操作接口
    int size();                        // 统计当前节点后代总数，亦即以其为根的子树的规模
    BinNode<T> *insertAsLc(T const &); // 作为当前节点的左孩子插入新节点
    BinNode<T> *insertAsRc(T const &); // 作为当前节点的右孩子插入新节点
    BinNode<T> *succ();                // 取当前节点的直接后继（中序遍历次序）

    template <typename VST>
    void travLevel(BinNode<T> *x, VST &);
    template <typename VST>
    void travLevel(VST &visit) { travLevel(this, visit); };

    template <typename VST>
    void travPre(BinNode<T> *x, VST &);
    template <typename VST>
    void travPre(VST &visit) { travPre(this, visit); }

    template <typename VST>
    void travIn(BinNode<T> *x, VST &);
    template <typename VST>
    void travIn(VST &visit) { travIn(this, visit); }

    template <typename VST>
    void travPost(BinNode<T> *x, VST &);
    template <typename VST>
    void travPost(VST &visit) { travPost(this, visit); }

    // 比较器，判等器
    bool operator<(BinNode const &bn) { return data < bn.data; }
    bool operator>(BinNode const &bn) { return data > bn.data; }
    bool operator==(BinNode const &bn) { return data == bn.data; }
    bool operator!=(BinNode const &bn) { return data != bn.data; }
};

template <typename T>
int BinNode<T>::size()
{
    int s = 1;
    if (lc)
        s += lc->size();
    if (rc)
        s += rc->size();
    return s;
}

template <typename T>
inline int stature(BinNode<T> *p)
{
    return p ? p->height : -1; // 返回节点高度(空树高度为-1)
}

template <typename T>
BinNode<T> *
BinNode<T>::insertAsLc(T const &e)
{
    // 前提：lc为空
    return lc = new BinNode(e, this); // 新节点指向this节点，this的lc指向新节点
}

template <typename T>
BinNode<T> *
BinNode<T>::insertAsRc(T const &e)
{
    // 前提：rc为空
    return rc = new BinNode(e, this); // 新节点指向this节点，this的rc指向新节点
}

/****************************
 * BinNode状态与性质的判断
 * *************************/

template <typename T>
inline bool IsRoot(const BinNode<T> &x)
{
    return !x.parent;
}

template <typename T>
inline bool IsLChild(const BinNode<T> &x)
{
    return !IsRoot(x) && (&x == x.parent->lc); // 先判断x是否为根节点，之后向上找父节点，与父节点的左节点比较
}

template <typename T>
inline bool IsRChild(const BinNode<T> &x)
{
    return !IsRoot(x) && (&x == x.parent->rc); // 先判断x是否为根节点，之后向上找父节点，与父节点的右节点比较
}

template <typename T>
inline bool HasParent(const BinNode<T> &x)
{
    return !IsRoot(x);
}

template <typename T>
inline bool HasLChild(const BinNode<T> &x)
{
    return x.lc;
}

template <typename T>
inline bool HasRChild(const BinNode<T> &x)
{
    return x.rc;
}

template <typename T>
inline bool HasChild(const BinNode<T> &x)
{
    return HasLChild(x) || HasRChild(x);
}

template <typename T>
inline bool HasBothChild(const BinNode<T> &x)
{
    return HasLChild(x) && HasRChild(x);
}

template <typename T>
inline bool IsLeaf(const BinNode<T> &x)
{
    return !HasChild(x);
}

template <typename T> // 返回x的 兄弟节点
inline BinNode<T> *sibling(const BinNode<T> *&x)
{
    return IsLChild(*x) ? (x->parent->rc) : (x->parent->lc);
}

template <typename T> // 返回x的  叔叔节点
inline BinNode<T> *uncle(const BinNode<T> *&x)
{
    return IsLChild(*(x->parent)) ? (x->parent->parent->rc) : (x->parent->parent->lc);
}

template <typename T>
BinNode<T> *BinNode<T>::succ()
{
    BinNode<T> *s = this;
    if (rc)

    {
        s = rc;
        while (HasLChild(*s))
        {
            s = s->lc;
        }
    }
    else
    {
        while (IsRChild(*s))
        {
            s = s->parent;
        }
        s = s->parent;
    }
    return s;
}
/****************************
 * 二叉树遍历
 * *************************/

template <typename T, typename VST>
static void visitAlongLeftBranch(BinNode<T> *x, VST &visit, Stack<BinNode<T> *> &S)
{
    while (x)
    {
        visit(x->data);
        S.push(x->rc);
        x = x->lc;
    }
}

template <typename T>
template <typename VST>
void BinNode<T>::travPre(BinNode<T> *x, VST &visit)
{
    Stack<BinNode<T> *> S;
    while (true)
    {
        visitAlongLeftBranch(x, visit, S);
        if (S.empty())
        {
            break;
        }
        x = S.pop();
    }
}

template <typename T>
static void goAlongLeftBranch(BinNode<T> *x, Stack<BinNode<T> *> &S)
{
    while (x)
    {
        S.push(x);
        x = x->lc;
    }
}

template <typename T>
template <typename VST>
void BinNode<T>::travIn(BinNode<T> *x, VST &visit)
{
    Stack<BinNode<T> *> S;
    while (true)
    {
        goAlongLeftBranch(x, S);
        if (S.empty())
        {
            break;
        }
        x = S.pop();
        visit(x->data);
        x = x->rc;
    }
}

template <typename T>
static void gotoHLVFL(Stack<BinNode<T> *> &S)
{
    while (BinNode<T> *x = S.top())
    {
        if (HasLChild(*x))
        {
            if (HasRChild(*x))
            {
                S.push(x->rc);
            }
            S.push(x->lc);
        }
        else
        {
            S.push(x->rc);
        }
    }

    S.pop();
}

template <typename T>
template <typename VST>
void BinNode<T>::travPost(BinNode<T> *x, VST &visit)
{
    Stack<BinNode<T> *> S;
    if (x)
        S.push(x);
    while (!S.empty())
    {
        if (S.top != x->parent)
        {
            gotoHLVFL(S);
        }
        x = S.pop();
        visit(x->data);
    }
}

template <typename T>
template <typename VST>
void BinNode<T>::travLevel(BinNode<T> *x, VST &visit)
{
    Queue<BinNode<T> *> Q;
    Q.enqueue(x);
    while (!Q.empty())
    {
        BinNode<T> *x = Q.dequeue();
        visit(x->data);
        if (HasLChild(*x))
        {
            Q.enqueue(x->lc);
        }
        if (HasRChild(*x))
        {
            Q.enqueue(x->rc);
        }
    }
}

/****************************
 * AVL
 * *************************/


template <typename T>
inline bool Balanced(const BinNode<T> &x)
{
    // 理想平衡条件
    return stature(x.lc) == stature(x.rc);
}

template <typename T>
inline bool BalFac(const BinNode<T> &x)
{
    // 平衡因子
    return (stature(x.lc) - stature(x.rc));
}

template <typename T>
inline bool AvlBalanced(const BinNode<T> &x)
{
    // AVL平衡条件
    return ((-2 < BalFac(x)) && (BalFac(x) < 2));
}

template<typename T>
inline BinNode<T>* tallerChild(const BinNode<T>* x){
    return stature(x->lc) > stature(x->rc)?x->lc:   //  左高
           stature(x->lc) < stature(x->rc)?x->rc:   //  右高 
           IsLChild(*x)?x->lc:x->rc;                //  等高：与父亲x同侧者优先
}