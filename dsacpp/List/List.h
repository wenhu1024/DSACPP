#include "ListNode.h" //引入列表节点类

template <typename T> class List{ //列表模板类

private:
    int _size; ListNodePosi<T> header; ListNodePosi<T> trailer; //规模，头哨兵，尾哨兵

protected:
    void init();    //列表创建时的初始化
    int  clear();    //清除所有节点
    void copyNodes(ListNodePosi<T>,int);    //复制列表中自位置p起的n项
    void merge(ListNodePosi<T>&,int,List<T>&,ListNodePosi<T>,int);  //归并
    void mergeSort(ListNodePosi<T>&,int);   //对从p开始连续的n个节点归并排序

    void selectionSort(ListNodePosi<T>,int);    //对从p开始连续的n个节点选择排序
    void insertionSort(ListNodePosi<T>,int);    //对从p开始连续的n个节点插入排序

public:
//  构造函数
    List(){init();} //默认
    List(List<T> const& L); //整体复制列表L
    List(List<T> const& L,Rank r,int n);    //复制列表L中自第r项起的n项
    List(ListNodePosi<T> p,int n);  //复制列表自位置p起的n项

//  析构函数
    ~List();    //释放(包含头、尾哨兵在内的)所有节点

//  只读访问接口
    Rank size() const {return _size;}   //规模
    Rank empty() const {return _size<=0;}   //判空
    T& operator[] (Rank r) const; //重载，支持循秩访问(效率低)
    ListNodePosi<T> first() const {return header->succ;}  //首节点位置
    ListNodePosi<T> last() const {return trailer->prep;}  //末节点位置
    bool valid(ListNodePosi<T> p)
    {   return p && (p != header) && (p != trailer); }  //将头、尾节点等同于nullptr
    ListNodePosi<T> find(T const& e) const{ //无序列表查找
        return find(e,_size,trailer);
    }
    ListNodePosi<T> find(T const& e, int n, ListNodePosi<T> p) const;  //无序区间查找
    ListNodePosi<T> search(T const& e) const   //有序列表查找
    {   return search(e,_size,trailer); }
    ListNodePosi<T> search(T const& e,int n,ListNodePosi<T> p); //有序区间查找
    ListNodePosi<T> selectMax(ListNodePosi<T> p,int n);    //在p及其n-1个后继中选出最大者
    ListNodePosi<T> selectMax() {return selectMax(header->succ,_size);}   //整体最大者

//  可写访问接口
    ListNodePosi<T> insertAsFirst(T const&e);   //将e当作首节点插入
    ListNodePosi<T> insertAsLast(T const&e);    //将e当作末节点插入
    ListNodePosi<T> insertA(ListNodePosi<T> p,T const& e);  //将e当作p的后驱插入
    ListNodePosi<T> insertB(ListNodePosi<T> p,T const& e);  //将e当作p的前驱插入
    T remove(ListNodePosi<T> p);    //删除合法位置p处的节点，返回被删除节点对应的数值
    
    
    int deduplicate();  //无序去重
    int uniquify(); //有序去重

//  遍历
    void traverse( void(*)(T&));    //遍历，依次实施visit操作(函数指针)
    template <typename VST>
    void traverse(VST&);    //遍历，依次实施visit操作(函数对象)
};  //List

template <typename T>
void List<T>::init(){   //列表初始化,在创建列表对象时统一调用
    header=new ListNode<T>; //创建头哨兵节点
    trailer=new ListNode<T>;    //创建尾哨兵节点
    header->succ=trailer; header->prep=nullptr;
    trailer->prep=header; trailer->succ=nullptr;
    _size=0;    //  记录规模
}

template <typename T>
T& List<T>::operator[](Rank r) const{//assert: 0<=r<size
    ListNodePosi<T> p=first();  //从首节点出发
    while(0<r--){
        p=p->succ;  //顺数第r个节点即是
    }
    return p->data; //目标节点，返回其中所存元素
}

template <typename T>
ListNodePosi<T> List<T>::find(T const&e,int n,ListNodePosi<T> p) const{//在无序列表内节点p的n个前驱中，找到等于e的最后者
    while(0 < n--){ // 0 <=n < _size 对于p的最近的n个前驱，从右向左
        if( e == ( p = p->prep) -> data)  return p;   //逐个比对，直至命中或范围越界   
    }
    return nullptr; //p越出左边界意味着区间内不含额，查找失败。失败时,返回nullptr
}

template <typename T>
ListNodePosi<T> List<T>::insertAsFirst(T const&e){
    _size++;
    return header->insertAsSucc(e); //e当作首节点插入
}

template <typename T>
ListNodePosi<T> List<T>::insertAsLast(T const&e){
    _size++;
    return trailer->insertAsPred(e);    //e当作末节点插入
}

template <typename T>
ListNodePosi<T> List<T>::insertA(ListNodePosi<T> p,T const& e){
    _size++;
    p->insertAsSucc(e); //e当作p的后驱插入
}

template <typename T>
ListNodePosi<T> List<T>::insertB(ListNodePosi<T> p,T const&e){
    _size++;
    p->insertAsPred(e); //e当作p的前驱插入
}

template <typename T>   //复制列表中自位置p起的n项
void List<T>::copyNodes(ListNodePosi<T> p,int n){   //p合法，且至少有n-1个真后继节点
    init(); //初始化
    while(n--){ //将n项依次作为末节点插入
        insertAsLast(p->data);
        p=p->succ;
    }
}

template <typename T>   //整体复制列表L
List<T>::List(List<T> const& L){
    copyNodes(L.first(),L._size());
}

template <typename T>   //assert: p合法，且至少有n-1个后继节点
List<T>::List(ListNodePosi<T> p,int n){
    copyNodes(p,n);
}

template <typename T>   //复制列表L中第r项起的n项
List<T>::List(List<T> const& L,Rank r,int n){
    copyNodes(L[r],n);
}

template <typename T>
T List<T>::remove(ListNodePosi<T> p){
    T e=p->data;
    p->prep->succ=p->succ;
    p->succ->prep=p->prep;
    delete p;
    _size--;
    return e;
}

template <typename T>
int List<T>::clear(){   //清空列表
    int oldSize=_size;
    while(0<_size){
        remove(header->succ);
    }
    return oldSize;
}

template <typename T>
List<T>::~List(){
    clear();    // 清空列表
    delete header; delete trailer;  //释放头、尾哨兵节点
}

template <typename T>
int List<T>::deduplicate(){
    if(_size <2) return 0;  //列表元素小于2
    int oldSize=_size;
    ListNodePosi<T> p=header; 
    Rank r=0;    //通过下面循环里的操作，p从首节点开始，r表示p节点的前缀节点数目
    while(trailer != (p=p->succ)){
        ListNodePosi<T> q=find(p->data,r,p);    //在p的r个真前驱中查找相同数值的节点
        q? remove(q): ++r;
    }
    return oldSize - _size; //返回删除节点数目
}

template <typename T>
void List<T>::traverse(void (*visit)(T&)){   //借助函数指针机制遍历
    for(ListNodePosi<T> p=header->succ;p!=trailer;p=p->succ){
        visit(p->data);
    }
}

template <typename T> template <typename VST>   //元素类型、操作器
void List<T>::traverse(VST& visit){ //借助函数对象机制遍历
    for(ListNodePosi<T> p=header->succ;p!=trailer;p=p->succ){
        visit(p->data);
    }
}

template <typename T>
int List<T>::uniquify(){    //成批剔除重元素,效率更高
    if(_size < 2) return 0;    //平凡列表,自然无重复元素
    int oldSize=_size;
    ListNodePosi<T> p=first(); ListNodePosi<T> q=nullptr;   //while循环中,p为各相同数值区段的起点,q为其后继
    while(trailer!=(q=p->succ)){    //反复考察紧邻节点对(p.q)
        if(p->data!=q->data){
            p=q;    //若互异，则转向下一区段
        }
        else{
            remove(q);  //否则相同,剔除q所指相同数值的节点
        }
    }
    return oldSize - _size; //返回删除的总节点数
}

template <typename T>   //在有序列表内节点p(可以是trailer)的前驱中,找到不大于e的最后者
ListNodePosi<T> List<T>::search(T const& e,int n,ListNodePosi<T> p){
    while(0 <= n--) //注意是 <=
        if(((p = p->pred)->data) <= e) break;
    return p;   //失败时，返回左边界的前驱
}

template <typename T>   //列表元素排序，对起始于位置p的n个元素的排序
void List<T>::selectionSort(ListNodePosi<T> p,int n){   //valid(p) && rank(p)+n <= size
    ListNodePosi<T> head=p->prep;
    ListNodePosi<T> tail=p;
    for(int i=0; i<n; ++i){tail=tail->succ;}    //待排序区间为(head,tail)
    while (1<n){    //在至少还剩2个节点之前，在待排序区间内
        ListNodePosi<T> max = selectMax(head->succ,n);  //找出最大者
        insertB(tail,remove(max));  //作为有序区间的首元素,移至到无需区间末尾
        tail=tail->prep; 
        --n;    //无序区间节点数目减一
    }
}

template <typename T>   //从起始位置p的n个元素中选出最大者
ListNodePosi<T> List<T>::selectMax(ListNodePosi<T> p,int n){
    ListNodePosi<T> max=p;  //最大者先暂定为p
    for(ListNodePosi<T> cur = p; 1 < n; --n){   //注意这里 1<n
        if( !( (cur=cur->succ)->data < max->data ) ){   //若当前元素不小于max，则
            max = cur;  //更新最大元素位置记录
        }
    }
    return max; //返回最大节点位置
}

template <typename T>
void List<T>::insertionSort(ListNodePosi<T> p,int n){
    for(int r=0; r<n; ++r){ //逐一为各节点
        insertA(search(p->data,r,p),p->data);   //查找适当的位置并插入
        p=p->succ; remove(p->prep); //转向下一个节点
    }
}


template <typename T>   //有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
void List<T>::merge(ListNodePosi<T> &p,int n,List<T> &L,ListNodePosi<T> q,int m){
    //在归并排序之类的场合，有可能 this == L && rank(p)+n == rank(q)
    ListNodePosi<T> pp=p->prep; //记忆首节点
    while(0<m){
        if( (0<n) && (p->data <= q->data)){
            if( q==(p=p->succ)){
                break;
            }
            --n;
        }
        else{
            insertB(p,L.remove( (q=(q->succ))->pred ));
            --m;
        }
    }
    p=pp->succ; //确定归并后区间的起点
}

template <typename T>   
void List<T>::mergeSort(ListNodePosi<T> &p,int n){
    if(n<2) return;
    int m = n>>1;
    ListNodePosi<T> q=p;
    for(int i=0;i<m;++i){q=q->succ;}    //均分列表
    mergeSort(p,m); mergeSort(q,n-m);   //对前，后子列表分别排序
    merge(p,m,*this,q,n-m);             //归并
}   //排序后，p依然指向归并后的起点