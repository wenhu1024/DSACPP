#include "BST.h"

template <typename T>
class AVL:public BST<T>
{
public:
    BinNode<T> *insert(const T&e); // 虚函数重写
    bool remove(const T&e); // 虚函数重写

    //BST::search()等其余接口可以直接沿用
};

template <typename T>
BinNode<T> *
AVL<T>::insert(const T &e)
{

    BinNode<T> *&x=this->search(e);
    if(x){
        return x;     //    存在要插入的目标节点，直接返回该节点
    }

    //  确认目标节点不存在, 创建新节点x
    BinNode<T> *xx=x=new BinNode<T>(e,this->_hot); 
    this->_size++;  

    for(BinNode<T> *g=this->_hot;g;g=g->parent){    //  从x之父出发，逐层检查各代祖先g
        if(!AvlBalanced(*g)){   //  一旦发现g失衡，则(采用 “ 3+4 ” 算法)使之平衡
            this->FromParentTo(*g)=this->rotateAt(tallerChild(tallerChild(g)));  // 并将子树重新接入原树 
            break;  // g复衡后，局部子树高度必然复原；其祖先亦必如此，故调整随即结束 
        }else{  //  否则g依然平衡，只用简单地更新其高度
            this->updateHeight(g);  //  更新高度        
        }
    }
    //   至多只需要一次调整；若调整过一次，则全树高度必然复原
    return xx;  //返回新节点位置
}

template <typename T>
bool
AVL<T>::remove(const T &e)
{
    BinNode<T> * &x=search(e);  
    if(!x){
        return false;   //  如果目标不存在，返回false
    }

    //  目标存在
    this->removeAt(x,_hot);
    this->_size--;
    for(BinNode<T>* g=this->_hot;g;g=g->parent){
        if(!AvlBalanced(*g)){   //  一旦发现g失衡，则(采用 “ 3+4 ” 算法)使之平衡
            this->FromParentTo(*g)=this->rotateAt(tallerChild(tallerChild(g)));  // 并将子树重新接入原树 
            this->updateHeight(g);  //  更新高度        
        }
    }

    return true;    //  删除成功


}