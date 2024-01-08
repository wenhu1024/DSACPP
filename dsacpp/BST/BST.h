#include "../BinTree/BinTree.h"

template <typename T>
class BST : public BinTree<T>
{
public:
    virtual BinNode<T> *&search(const T &e);
    virtual BinNode<T> *insert(const T &e);
    virtual bool remove(const T &e);

protected:
    BinNode<T> *_hot{nullptr};
    BinNode<T> *connect34(
        BinNode<T> *, BinNode<T> *, BinNode<T> *,
        BinNode<T> *, BinNode<T> *, BinNode<T> *, BinNode<T> *);
    BinNode<T> *rotateAt(BinNode<T> *x);
};

template <typename T>
BinNode<T> *&
BST<T>::search(const T &e)
 {
    if (!this->_root || e == this->_root->data)
    {
        _hot = nullptr;
        return this->_root;
    } // 空树，或恰在树根命中

    for (_hot = this->_root;;)
    {                                                            // 否则，自顶而下
        BinNode<T> *&v = (e < _hot->data) ? _hot->lc : _hot->rc; // 确定方向，深入一层
        if (!v || e == v->data)
            return v; // 一旦命中或抵达叶子，随即返回目标节点位置的引用，以便后续插入、删除操作
        _hot = v;     // 无论命中或失败，_hot均指向v之父亲（v是根时，hot为nullptr）
    }
}

template <typename T>
BinNode<T> *
BST<T>::insert(const T &e)
{
    BinNode<T> *&x = search(e);
    if (x)
    {
        return x;   //  防止插入重复元素
    }

    x = new BinNode<T>(e, _hot);    // _hot为插入节点的父节点
    this->_size++;
    this->updateHeightAbove(x);
    return x;
}

template <typename T>
static BinNode<T>* removeAt(BinNode<T>* &x,BinNode<T>* &hot)   // 注意这里的指针引用
{
    BinNode<T> *w=x;    //要删除的节点
    BinNode<T> *succ=nullptr;   //要接替的节点

    if(!(HasLChild(*x))){
        //右单支
        succ=x=x->rc;  // 引用x=x->rc,自上而下连接
    }else if(!(HasRChild(*x))){
        //左单支    
        succ=x=x->lc;  // 引用x=x->lc,自上而下连接
    }else{
        //左右双支
        w=w->succ(); // 因为是有左右分支的情况，所以w节点必有中序后继
        swap(x->data,w->data);
        BinNode<T> *u=w->parent;
        ((u==x)? u->rc : u->lc) = succ= w->rc;     // 自上而下连接，因为w有右孩子，所以后继必为左侧链末端，
        //左侧链末端只会有右孩子
    }

    hot=w->parent;

    if(succ){
        succ->parent=hot;   //如果接替者不为空，接替者自下而上连接
    }
    release(w->data);
    release(w);
    return succ;

}

template <typename T>
bool
BST<T>::remove(const T &e)
{
    BinNode<T>* &x=search(e);

    if(!x){
        return false;   //查询失败，不存在该元素，返回false
    }

    removeAt(e,_hot);
    this->_size--;
    this->updateHeightAbove(_hot);
    return true;
} 

template <typename T>
BinNode<T>* 
BST<T>::connect34(BinNode<T> *a,BinNode<T> *b,BinNode<T> *c,
    BinNode<T> *T0,BinNode<T> *T1,BinNode<T> *T2,BinNode<T>* T3)
{
    a->lc=T0;if(T0){T0->parent=a;}
    a->rc=T1;if(T1)(T1->parent=a;)  this->updataHeight(a);
    c->lc=T2;if(T2){T2->parent=c;}
    c->rc=T3;if(T3){T3->parent=c;}  this->updateHeight(c);
    b->lc=a;a->parent=b;
    b->rc=c;c->parent=b;    this->updateHeight(b);
    return b;   //  返回该子树的根节点
}

template <typename T>
BinNode<T>*
BST<T>::rotateAt(BinNode<T>*v)
{
    BinNode<T>* p=v->parent;
    BinNode<T>* g=p->parent;

    /*先zig(右旋)*/
    if(IsLChild(*p)){
        if(IsLChild(*v)){
            //  zig-zig
            p->parent=g->parent;    //  向上连接
            return connect34(v,p,g,v->lc,v->rc,p->rc,g->rc);
        }else{
            //  zig-zag
            v->parent=g->parent;    //  向上连接
            return connect34(p,v,g,p->lc,v->lc,v->rc,g->rc);
        }
    }
    /*后zag(左旋)*/
    else{
        if(IsRChild(*v)){
            //  zag-zag
            p->parent=g->parent;    //  向上连接
            return connect34(g,p,v,g->lc,p->lc,v->lc,v->rc);
        }else{
            //  zag-zig
            v->parent=g->parent;    //  向上连接
            return connect34(g,v,p,g->lc,v->lc,v->rc,p->rc);
        }
    }

}