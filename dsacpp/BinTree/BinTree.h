#include "BinNode.h"
#include "../release/release.h"

template <typename T>
class BinTree
{
protected:
    int _size;                                         // 规模
    BinNode<T> *_root;                                 // 根节点
    virtual int updateHeight(BinNode<T> *x);           // 更新节点x的高度
    void updateHeightAbove(BinNode<T> *x);             // 更新节点x及其祖先的高度
    BinNode<T> *&FromParentTo(const BinNode<T> &node); // 返回来自父亲节点的引用
public:
    BinTree() : _size(0), _root(nullptr) {} // 构造函数
    ~BinTree()
    {
        if (_size > 0)
            remove(_root);
    } // 析构函数

    int size() const { return _size; }                    // 规模
    bool empty() const { return !_root; }                 // 判空
    BinNode<T> *root() const { return _root; }            // 树根
    BinNode<T> *insertAsRoot(T const &);                  // 插入根节点
    BinNode<T> *insertAsLC(BinNode<T> *, T const &);      // 插入左节点,原本无左孩子
    BinNode<T> *insertAsRC(BinNode<T> *, T const &);      // 插入右节点,原本无右孩子
    BinNode<T> *attachAsLC(BinNode<T> *, BinTree<T> *&T); // T作为x左子树接入,x原本无左子树
    BinNode<T> *attachAsRC(BinNode<T> *, BinTree<T> *&T); // T作文x右子树接入,x原本无右子树
    int remove(BinNode<T> *x);                            // 删除以位置x处节点为根的子树，返回该子树原先的规模
    BinNode<T> *secede(BinNode<T> *x);                    // 将子树x从当前树中摘除，并将其转化为一颗独立子树

    template <typename VST>
    void travLevel(VST &visit)
    {
        if (_root)
            _root->travLevel(visit);
    } // 层次遍历
    template <typename VST>
    void travPre(VST &visit)
    {
        if (_root)
            _root->travPre(visit);
    } // 先序遍历
    template <typename VST>
    void travIn(VST &visit)
    {
        if (_root)
            _root->travIn(visit);
    } // 中序遍历
    template <typename VST>
    void travPost(VST &visit)
    {
        if (_root)
            _root->travPost(visit);
    } // 后序遍历

    bool operator<(const BinTree<T> &t) { return _root && t._root && ((*_root) < (*t._root)); }
    bool operator>(const BinTree<T> &t) { return _root && t._root && ((*_root) > (*t._root)); }
    bool operator==(const BinTree<T> &t) { return _root && t._root && _root == t._root; }
    bool operator!=(const BinTree<T> &t) { return !(*this == t); }
};

template <typename T> // 删除二叉树中位置x处的节点及其后代，返回被删除节点的总数值
static int removeAt(BinNode<T> *x)
{
    if (!x)
        return 0;
    int n = 1 + removeAt(x->lc) + removeAt(x->rc);
    release(x->data);
    release(x);
    return n;
}

template <typename T> // 更新节点x高度
int BinTree<T>::updateHeight(BinNode<T> *x)
{
    return x->height = 1 + max(stature(x->lc), stature(x->rc));
}

template <typename T>
void BinTree<T>::updateHeightAbove(BinNode<T> *x)
{
    while (x)
    { // 从x出发，覆盖历代祖先
        updateHeight(x);
        x = x->partent;
    }
}

template <typename T>
BinNode<T> *&
BinTree<T>::FromParentTo(const BinNode<T> &node)
{
    if (IsRoot(node))
    {
        return this->_root;
    }
    else if (IsLChild(node))
    {
        return node.parent->lc;
    }
    else
    {
        return node.parent->rc;
    }
}

template <typename T> // 删除二叉树中位置x处的节点及其后代，返回被删除节点的总数值
int BinTree<T>::remove(BinNode<T> *x)
{
    this->FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    int n = removeAt(x);
    _size -= n;
    return n;
}

template <typename T>
BinNode<T> *
BinTree<T>::insertAsRoot(T const &e)
{
    _size++;
    return _root = new BinNode<T>(e);
}

template <typename T> // 前提：x节点原本没有左孩子
BinNode<T> *
BinTree<T>::insertAsLC(BinNode<T> *x, T const &e)
{
    _size++;
    x->insertAsLc(e);
    updateHeightAbove(x);
    return x->lc;
}

template <typename T> // 前提：x节点原本没有右孩子
BinNode<T> *
BinTree<T>::insertAsRC(BinNode<T> *x, T const &e)
{
    _size++;
    x->insertAsRc(x);
    updateHeightAbove(x);
    return x->rc;
}

template <typename T> // 二叉树子树分离法：将子树x从当前树中摘除，将其封装为一颗独立子树返回
BinNode<T> *
BinTree<T>::secede(BinNode<T> *x)
{
    this->FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    BinTree<T> *S = new BinTree<T>;
    S->_root = x;
    x->parent = nullptr;
    S->_size = x->size();
    return S;
}

template <typename T>
BinNode<T> *
BinTree<T>::attachAsLC(BinNode<T> *x, BinTree<T> *&S)
{ // x->lc==nullptr
    if (x->lc = S->_root)
    {
        x->lc->parent = x;
    }

    _size += S->_size;
    updateHeightAbove(x);
    S->_root = nullptr; // 重要一点，根节点赋值为nullptr
    S->_size = 0;
    release(S);
    S = nullptr;
    return x;
}

template <typename T>
BinNode<T> *
BinTree<T>::attachAsRC(BinNode<T> *x, BinTree<T> *&s)
{ // x->rc==nullptr
    if (x->rc = S->_root)
    {
        x->rc->parent = x;
    }

    _size += S->_size;
    updateHeightAbove(x);
    S->_root = nullptr; // 重要一点，根节点赋值为nullptr
    S->_size = 0;
    release(S);
    S = nullptr;
    return x;
}