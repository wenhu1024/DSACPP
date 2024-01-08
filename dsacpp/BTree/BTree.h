#include "BTNode.h"
#include "../release/release.h"

template <typename T>
class BTree
{
protected:
    int _size;        //  存放的关键码总数
    int _order;       //  B-树的阶次，至少为3——创建时指定，一般不能修改
    BTNode<T> *_root; //  根节点
    BTNode<T> *_hot;  //  BTree::search()最后访问的非空的节点位置(除非树空)
    void solveOverflow(BTNode<T> *);
    void solveUnderflow(BTNode<T> *);

public:
    //  构造函数，默认为最低的3阶
    BTree(int order = 3) : _order(order), _size(0) { _root = new BTNode<T>(); }

    ~BTree()
    {
        if (_root)
            release(_root);
    }

    int const order() { return _order; }
    int const size() { return _size; }
    BTNode<T> *&root() { return _root; }
    bool empty() const { return !_root || _root->key.empty(); }

    BTNode<T> *search(const T &e);
    bool insert(const T &e);
    bool remove(const T &e);
};

template <typename T>
BTNode<T> *
BTree<T>::search(const T &e)
{
    BTNode<T> *v = _root;
    _hot = nullptr;

    while (v)
    {
        Rank r = v->key.search(e);
        if ((r >= 0) && (e == v->key[r]))
        {
            return v;
        }
        else
        {
            _hot = v;
            v = v->child[r + 1];
        }
    }
    return nullptr; //  失败时，返回外部节点,_hot指代其父节点
}

template <typename T>
void BTree<T>::solveOverflow(BTNode<T> *v)
{
    while (v->key.size() >= _order)
    {                        //  此为上溢条件
        Rank s = _order / 2; //  轴点

        //  右部分(right节点)
        BTNode<T> *right = new BTNode<T>(); //  转移v节点右侧key元素和child元素(不含s轴点)
        for (int i = 0; i < _order - s - 1; ++i)
        {
            right->key.insert(i, v->key.remove(s + 1));
            right->child.insert(i, v->child.remove(s + 1));
        }
        right->child[_order - s - 1] = v->child.remove(s + 1); //  移动v最靠右的孩子
        if (right->child[0])
        { //  若right孩子部分非空
            for (int j = 0; j < _order - s; ++j)
            {
                right->child[j]->parent = right; //  则将孩子节点的父亲指向right
            }
        }

        //  左部分及s处理部分(v剩余部分)
        BTNode<T> *p = v->parent; //  v的父节点
        if (!p)
        { //  p为nullptr，也就是v已经为根
            _root = p = new BTNode<T>();
            p->child[0] = v;
            v->parent = p;
        }
        Rank r = 1 + p->key.search(v->key[0]); //  p的孩子中指向v的元素，其对应的秩(注意这里的 +1 )
        p->key.insert(r, v->key.remove(s));    //  转移v的s关键没至p

        //  right与父节点互联
        p->child.insert(r + 1, right);
        right->parent = p;
    }
}

template <typename T>
void BTree<T>::solveUnderflow(BTNode<T> *v)
{
    while (v->child.size() < (_order + 1) / 2)
    { // 此为下溢条件
        BTNode<T> *p = v->parent;
        if (!p)
        { //  已经到达根节点
            if (v->key.size() == 0 && v->child[0] != nullptr)
            {
                //  此时情况：作为树根的v已不含关键码，却有（唯一的）非空孩子
                printf("collapse\n");
                _root = v->child[0];
                _root->parent = nullptr;
                v->child[0] = nullptr;
                release(v);
                return;
            }
        }
        //  未到达树根
        Rank r = 0;
        while (p->child[r] != v)
        {
            r++;
        }
        printf("\nrank = %d", r);
        // 确定v是p的第r个孩子——此时v可能不含关键码，故不能通过关键码查找

        // 向左兄弟借关键码
        if (r > 0)
        {                                          //  v存在左兄弟
            BTNode<T> *ls = p->child[r - 1];       // v的左兄弟ls
            if ((_order + 1) / 2 < ls->key.size()) //  ls的分支数大于 ceil(_order/2)
            {
                printf(" ... case 1\n");
                v->key.insert(0, p->key[r - 1]);                    //  p借出一个关键码给v（作为最小关键码），注意p关键码的秩
                p->key[r - 1] = ls->key.remove(ls->key.size() - 1); // ls的最大关键码转入p
                v->child.insert(0, ls->child.remove(ls->child.size() - 1));
            }
            if (v->child[0])
            {
                v->child[0]->parent = v;
            }
            return; // 至此，通过右旋已完成当前层（以及所有层）的下溢处理
        }

        // 向右兄弟借关键码
        if (r < p->child.size() - 1) //  若v不是p最右边的孩子
        {
            BTNode<T> *rs = p->child[r + 1]; //  v的右兄弟
            if (rs->child.size() > (_order + 1) / 2)
            { //  rs的分支数大于 ceil(_order/2)
                printf(" ... case 2\n");
                v->key.insert(v->key.size(), p->key[r]); //  p借出一个关键码给v（作为最大关键码），注意p关键码的秩
                p->key[r] = rs->key.remove(0);           // ls的最小关键码转入p
                v->child.insert(v->child.size(), rs->child.remove(0));
            }
            if (v->child[v->child.size()])
            {
                v->child[v->child.size()]->parent = v;
            }
            return; // 至此，通过左旋已完成当前层（以及所有层）的下溢处理
        }

        //  左右合并
        if (r > 0)
        { //  存在左孩子，左合并
            BTNode<T> *ls = p->child[r - 1];
            ls->key.insert(ls->key.size(), p->key.remove(r - 1));
            p->child->remove(r);
            ls->child.insert(ls->child.size(), v->child.remove(0));
            if (ls->child[ls->child.size() - 1])
            {
                ls->child[ls->child.size() - 1]->parent = ls;
            }
            while (!v->key.empty())
            {
                ls->key.insert(ls->key.size(), v->key.remove(0));
                ls->child.insert(ls->child->size(), v->child.remove(0));
                if (ls->child[ls->child.size() - 1])
                {
                    ls->child[ls->child.size() - 1]->parent = ls;
                }
            }
            release(v);
            //  此处没有return ，合并可能会出现下溢上传
        }
        else
        { //  存在右孩子，右合并
            printf( " ... case 3R\n" );
            BTNode<T> *rs = p->child[r + 1];
            rs->key.insert(0, p->key.remove(r));
            p->child->remove(r);
            rs->child.insert(0, v->child.remove(v->child.size() - 1));
            if (rs->child[0])
            {
                rs->child[0]->parent = rs;
            }
            while (!v->key.empty())
            {
                rs->key.insert(0, v->key.remove(v->key.size() - 1));
                rs->child.insert(0, v->child.remove(v->child.size() - 1));
                if (rs->child[0])
                {
                    rs->child[0]->parent = rs;
                }
            }
            release(v);
            //  此处没有return ，合并可能会出现下溢上传
        }
        v = p; // 上升一层，如有必要则继续旋转或合并——至多O(logn)层
    }
}

template <typename T>
bool BTree<T>::insert(const T &e)
{
    BTNode<T> *v = BTree<T>::search(e);

    //  已存在e
    if (v)
    {
        return false;
    }

    //  不存在e
    Rank r = _hot->key.search(e);
    _hot->key.insert(r + 1, e);
    _hot->child.insert(r + 2, nullptr);
    _size++;
    solveOverflow(_hot);
    return true;
}

template <typename T>
bool BTree<T>::remove(const T &e)
{
    BTNode<T> *v = BTree<T>::search(e);
    if (!v)
    {
        return false;
    }
    Rank r = v->key.search(e);

    //  如果v不是叶子，则e的直接后继必属于某叶子节点
    if (v->child[0])
    {
        BTNode<T> *u = v->child[r + 1]; //  找直接后继
        while (u->child[0])
        { //  右子树一直向左即可
            u = u->child[0];
        }

        //  交换
        v->key[r] = u->key[0];
        v = u;
        r = 0;
    }
    //  此时，v必然是叶子，且其中第r个关键码就是待删除者
    v->key.remove(r);
    v->child.remove(r + 1);
    _size--;
    solveUnderflow(v);
    return true;
}
