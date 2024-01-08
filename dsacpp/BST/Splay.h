#include "BST.h"

template <typename T>
class Splay:public BST<T>
{

protected:
    BinNode<T> *splay(BinNode<T>* v);   //将节点v伸展至根
public:
    BinNode<T> *&search(T const&e);  //  伸展树的查找也会引起整树的结构调整，所以要重写
    BinNode<T> *insert(T const&e);  
    bool remove(T const&e);
};

template <typename T> 
inline 
void attachAsLChild(BinNode<T> *& p,BinNode<T> *&lc)
{
    p->lc=lc;
    if(lc){
        lc->parent=p;
    }
}

template <typename T> 
inline 
void attachAsRChild(BinNode<T> *& p,BinNode<T> *& rc)
{
    p->rc=rc;
    if(rc){
        rc->parent=p;
    }
}

template <typename T>
BinNode<T>*
Splay<T>::splay(BinNode<T> *v)
{
    if(!v){
        return nullptr;
    }
    BinNode<T> *p=nullptr;
    BinNode<T> *g=nullptr;

    while((p=v->parent)&&(g=p->parent)){    //  自下而上，反复对*v做双层伸展
        BinNode<T> *gg=g->parent;   //  每轮之后，*v都以原曾祖父为父
        if(IsLChild(*v)){
            if(IsLChild(*p)){   //  zig-zig
                attachAsLChild(g,p->rc);
                attachAsLChild(p,v->rc);
                attachAsRChild(p,g);
                attachAsRChild(v,p);
            }else{
                //  zig-zag
                attachAsLChild(p,v->rc);
                attachAsRChild(g,v->lc);
                attachAsLChild(v,g);
                attachAsRChild(v,p);
            }
        }else{
            if(IsRChild(*p)){
                attachAsRChild(g,p->lc);
                attachAsRChild(p,v->lc);
                attachAsLChild(p,g);
                attachAsLChild(v,p);
            }else{
                attachAsRChild(p,v->lc);
                attachAsLChild(g,v->rc);
                attachAsLChild(v,p);
                attachAsRChild(v,g);
            }
        }
        if(!gg){
            v->parent=nullptr;
        }else{
            (g==gg.lc) ? attachAsLChild(gg,v):attachAsRChild(gg,v);
        }
        this->updatHeight(g);
        this->updatHeight(p);
        this->updatHeight(v);
    }   //  双层伸展结束时，必有g==nullptr，
        //  但是树的深度为奇数时,p不为nullptr，需要对p进行单旋操作

    if(p = v->parent){
        if(IsLChild(*v)) {  
            attachAsLChild(p, v->rc); 
            attachAsRChild(v, p);
        } else {
            attachAsRChild(p, v->lc);
            attachAsLChild(v, p);
        }
        this->updateHeight(p);
        this->updateHeight(v);
    }
    v->parent = nullptr;
    return v;
}

template <typename T>
BinNode<T> *& 
Splay<T>::search(T const&e)
{
    BinNode<T> *p=this->search(e);
    this->_root=splay(p?p:this->_hot);  //  将最后一个被访问的节点伸展至根
    return _root;                       //  search函数不为静态，每次调用都会改变树的结构
}

template <typename T>
BinNode<T> *
Splay<T>::insert(T const&e)
{
    //  空树
    if(!this->_root){
        this->_size++;
        this->_root=new BinNode<T>(e);
        return this->_root;
    }

    // 存在目标e
    if(e==search(e)->data){
        return this->_root;
    }

    //  不存在目标e
    this->_size++;
    BinNode<T> *t=this->_root;
    if(this->_root->data < e){
        t->parent=this->_root=new BinNode<T>(e,nullptr,t,t->rc);
        if(HasRChild(*t)){
            t->rc->parent=this->_root;
            t->rc=nullptr;
        }
    }else{
        t->parent=this->_root=new BinNode<T>(e,nullptr,t->lc,t);
        if(HasLChild(*t)){
            t->lc->parent=this->_root;
            t->lc=nullptr;
        }
    }
    this->updataHeightAbove(t);
    return this->_root;
}

template <typename T>
bool
Splay<T>::remove(const T&e)
{
    //  空树 或者 目标不存在
    if(!this->_root || e!=search(e)->data){
        return false;
    }

    //  目标e存在，经过search函数后，e成为树根
    BinNode<T> *w=this->_root;
    if(!HasLChild(*this->_root)){   //  若无左子树，则直接删除
        this->_root=this->_root->rc;
        if(this->_root){
            this->_root->parent=nullptr;
        }
    }else if(!HasRChild(*this->_root)){ //  若无右子树，则直接删除
        this->_root=this->_root->lc;
        if(this->_root){
            this->_root->parent=nullptr;
        }
    }else{  //  左右子树同时存在
        //  暂时切除左子树
        BinNode<T> *lTree=this->_root->lc;
        lTree->parent=nullptr;
        this->_root=nullptr;

        //  只保留右子树
        this->_root=this->_root->rc;
        this->_root->parent=nullptr;

        search(w->root);    //  做一次找不到目标的失败查找，此时右子树最小节点伸展至右子树树根

        //  连接左子树
        this->_root->lc=lTree;
        lTree->parent=this->_root;
    }
    release(w->data);
    release(w);

    return true;
}