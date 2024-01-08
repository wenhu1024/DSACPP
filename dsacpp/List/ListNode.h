using Rank = int;   //秩

template <typename T> struct ListNode;
template <typename T> using ListNodePosi=ListNode<T>*;  //列表节点位置
template <typename T> struct ListNode{

//  成员
    T data; ListNodePosi<T> prep,succ; //数值，前驱，后驱
//  构造函数
    ListNode(){}    //针对header和trailer的构造
    ListNode(T e,ListNodePosi<T> p=nullptr,ListNodePosi<T> s=nullptr)
        :data(e),prep(p),succ(s){}  //默认构造函数
//  操作接口
    ListNodePosi<T> insertAsPred(T const& e);   //紧靠当前节点之前插入新节点
    ListNodePosi<T> insertAsSucc(T const& e);   //紧靠当前节点之后插入新节点
 };

template <typename T>
ListNodePosi<T> ListNode<T>::insertAsPred(T const&e){//将新元素e作为当前节点的前驱插至列表
    ListNodePosi<T> x = new ListNode(e,prep,this); //创建新节点
    prep->succ=x; 
    prep=x;   
    return x;   //返回新节点的位置
}

template <typename T>
ListNodePosi<T> ListNode<T>::insertAsSucc(T const&e){
    ListNodePosi<T> x = new ListNode(e,this,succ);
    succ->prep=x;
    succ=x;
    return x;
}