#include <iostream>
#include "Fib.h"
#define DEFAULT_CAPACITY 3 // 默认初始容量(实际应用中可设置为更大)
using namespace std;
using Rank = int; // 秩

template <typename T>
class Vector
{
protected:
    Rank _size;    // 规模
    int _capacity; // 容量
    T *_elem;      // 数据区

    void copyFrom(T const *A, Rank lo, Rank hi); // 复制数组区间A[lo,hi)
    void expand();                               // 空间不足时扩容
    void shrink();                               // 装填因子过小时压缩

    bool bubble(Rank lo, Rank hi);         // 扫描交换
    void bubbleSort(Rank lo, Rank hi);     // 起泡排序算法
    void merge(Rank lo, Rank mi, Rank hi); // 归并算法
    void mergeSort(Rank lo, Rank hi);      // 归并排序算法
    void quickSort(Rank lo, Rank hi);      // 快速排序算法
    void heapSort(Rank lo, Rank hi);       // 堆排序
public:
    // 构造函数

    // 默认构造方法: 容量为c,规模为s,所有元素初始为v
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v)
            ;
    }

    // 数组整体复制
    Vector(T const *A, Rank n) { copyFrom(A, 0, n); }

    // 数组区间复制
    Vector(T const *A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }

    // 向量整体复制
    Vector(Vector<T> const &v) { copyFrom(v._elem, 0, v._size); }

    // 向量区间复制
    Vector(Vector<T> const &v, Rank lo, Rank hi) { copyFrom(v._elem, lo, hi); }

    // 拷贝赋值函数
    Vector &operator=(Vector<T> const &v)
    {
        if (_elem)
            delete[] _elem;
        copyFrom(v._elem, 0, v.size());
        return *this;
    }

    // 析构函数
    ~Vector() { delete[] _elem; }
    // 只读访问接口
    static bool lt(T *a, T *b) { return lt(*a, *b); }          // less than
    static bool lt(T const &a, T const &b) { return a < b; }   // less than
    static bool eq(T *a, T *b) { return eq(*a, *b); }          // equal
    static bool eq(T const &a, T const &b) { return a == b; }  // equal
    Rank size() const { return _size; }                        // 规模
    bool empty() const { return !_size; }                      // 判空
    int disordered() const;                                    // 判断向量是否已排序
    Rank find(T const &e) const { return find(e, 0, _size); }; // 无序向量整体查找
    Rank find(T const &e, Rank lo, Rank hi) const;             // 无序向量区间查找
    Rank binSearch(T *A, T const &e, Rank lo, Rank hi) const;
    Rank fibSearch(T *A, T const &e, Rank lo, Rank hi) const;
    Rank search(T const &e) const // 有序向量整体查找
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    Rank search(T const &e, Rank lo, Rank hi) const; // 有序向量区间查找

    // 可写访问接口
    T &operator[](Rank r) const;                         // 重载下标操作符，可以类似于数组形式引用各元素
    T remove(Rank r);                                    // 删除秩为r的元素
    int remove(Rank lo, Rank hi);                        // 删除秩在区间[lo,hi)之内的区间
    void unsort(Rank lo, Rank hi);                       // 对[lo,hi)置乱
    void unsort() { unsort(0, _size); }                  // 整体置乱
    Rank insert(Rank r, T const &e);                     // 插入元素
    Rank insert(T const &e) { return insert(_size, e); } // 默认作为末元素插入
    int deduplicate();                                   // 无序去重
    int uniquify();                                      // 有序去重
    // 遍历
    void traverse(void (*)(T &)); // 遍历(使用函数指针，只读或者局部性修改)
    template <typename VST>
    void traverse(VST &); // 遍历(使用函数对象，可全局性修改)

}; // Vector

template <typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi)
    {
        _elem[_size++] = A[lo++];
    }
}

template <typename T>
void Vector<T>::expand()
{
    if (_size < _capacity)
    {
        return;
    }
    // else... _size >= _capacity
    if (_capacity < DEFAULT_CAPACITY)
    {
        _capacity = DEFAULT_CAPACITY;
    }
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1];

    for (int i = 0; i < _size; ++i)
    {
        _elem[i] = oldElem[i];
    }

    delete[] oldElem;
}

template <typename T>
void Vector<T>::shrink()
{
    if (_capacity < (DEFAULT_CAPACITY << 1))
    {
        return;
    }
    if ((_size << 2) > _capacity)
    { // 装填因子以25%为界
        return;
    }

    T *oldElem = _elem;
    _elem = new T[_capacity >>= 1];
    for (int i = 0; i < _size; ++i)
    {
        _elem[i] = oldElem[i];
    }

    delete[] oldElem;
}

template <typename T>
T &Vector<T>::operator[](Rank r) const
{
    return _elem[r];
}

template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi)
{
    T *v = _elem + lo; // 子向量_elem[hi,lo] --> v[0,hi-lo)
    int size = hi - lo;
    for (int i = size; i > 0; --i)
    {
        swap(v[i - 1], v[rand() % i]); // swap( v[i-1],v[0,...,i)中的一个元素 )
    }
}

template <typename T>
Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const
{
    // assert: 0<=lo<=hi<=_size
    // 从后往前找 [lo,hi)  找到返回最后一个元素e的位置,没找到返回lo-1
    while ((lo < hi--) && (e != _elem[hi]))
        ;
    return hi;
}

template <typename T> // 将e作为秩r元素插入
Rank Vector<T>::insert(Rank r, T const &e)
{
    // assert: 0<=r<=_size
    expand(); // 若有必要，扩容
    for (int i = _size; i > r; --i)
    {
        // 自后向前，后继元素_elem[r,_size)顺次后移一个单元
        _elem[i] = _elem[i - 1];
    }
    _elem[r] = e;
    _size++;
    return r;
}

template <typename T>
T Vector<T>::remove(Rank r)
{
    T e = _elem[r];   // 备份被删除元素
    remove(r, r + 1); // 调用区间删除算法，等效于对区间[r,r+1)的删除
    return e;         // 返回被删除元素
}

template <typename T> // 删除区间[lo,hi)
int Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi)
    { // 单独考虑退化情况,如remove[0.0)
        return 0;
    }

    while (hi < _size)
    {
        //[hi,_size)顺次前移hi-lo个单元
        _elem[lo++] = _elem[hi++];
    }
    _size = lo;     // 更新规模，直接丢弃尾部[lo,_size=hi)区间
    shrink();       // 若有必要，则缩容
    return hi - lo; // 返回被删除元素的数目
}

template <typename T> // 删除无序向量中重复元素
int Vector<T>::deduplicate()
{
    int oldSize = _size; // 记录原规模
    Rank i = 1;          // 从_elem[1]开始
    while (i < _size)
    {                                // 自前向后逐一考察各元素_elem[i]
        (find(_elem[i], 0, i)) < 0 ? // 在其前缀中寻找与之雷同者(至多一个)
            ++i
                                   : remove(i);
    }
    return oldSize - _size; // 返回删除元素总数
}

template <typename T>
void Vector<T>::traverse(void (*visit)(T &))
{ // 借助函数指针机制
    for (int i = 0; i < _size; ++i)
    {
        visit(_elem[i]);
    }
}

template <typename T>   // 元素类型
template <typename VST> // 操作器
void Vector<T>::traverse(VST &visit)
{ // 借助函数对象机制
    for (int i = 0; i < _size; ++i)
    {
        visit(_elem[i]);
    }
}

template <typename T>
int Vector<T>::disordered() const
{
    int n = 0; // 计数器
    for (int i = 1; i < _size; ++i)
    {
        // 逐一检查_size-1对相邻元素
        if (_elem[i - 1] > _elem[i])
        {
            ++n;
        }
    }
    return n;
}

template <typename T>
int Vector<T>::uniquify()
{
    Rank i = 0, j = 0;
    while (++j < _size)
    { // 逐一扫描，直至末元素
        if (_elem[i] != _elem[j])
        {                          // 跳过雷同元素
            _elem[++i] = _elem[j]; // 发现不同元素时，将_elem[j]向前移至紧邻于_elem[i]的右侧
        }
    }
    _size = ++i;
    shrink();
    return j - i; // 返回被删除元素的总数
}

template <typename T>
Rank Vector<T>::binSearch(T *A,T const &e,Rank lo,Rank hi) const
{
    while(lo<hi){
        Rank mi=lo+(hi-lo)>>1;
        if(e<A[mi]){
            hi=mi;
        }else{
            lo=mi+1;
        }
    }
    return --lo;
}  

template<typename T>
Rank Vector<T>::fibSearch (T* A, T const& e, Rank lo, Rank hi) const{
    for( Fib fib ( hi - lo ); lo < hi;  ) { //Fib数列制表备查
        while( hi - lo < fib.get() ) fib.prev(); //自后向前顺序查找（分摊O(1)）
        Rank mi = lo + fib.get() - 1; //确定形如Fib(k) - 1的轴点
        ( e < A[mi] ) ? hi = mi : lo = mi + 1; //比较后确定深入前半段[lo, mi)或后半段(mi, hi)
    } //成功查找不能提前终止
    return --lo; 
}

template <typename T> // 在有序向量的区间[lo,hi)内，确定不大于e的最后的一个节点的秩
Rank Vector<T>::search(T const &e, Rank lo, Rank hi) const
{
    // assert: 0<=lo<hi<=_size
        return binSerch(_elem, e, lo, hi);
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    while (!bubble(lo, hi--))
        ; // 逐趟做扫码交换，直至全序
}

template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
    bool sorted = true; // 整体有序标志
    while (++lo < hi)
    { // 自左向右，逐一检查各对相邻元素
        if (_elem[lo - 1] > _elem[lo])
        {                                   // 如果逆序
            sorted = false;                 // 意味着尚未整体有序，并需要
            swap(_elem[lo - 1], _elem[lo]); // 通过交换使得局部有序
        }
    }
    return sorted; // 返回有序标志
}

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (hi - lo < 2)
    { // 单元素区间自然有序，否则...
        return;
    }
    int mi = lo + ((hi - lo) >> 1); // 以中点为界
    mergeSort(lo, mi);              // 分别排序
    mergeSort(mi, hi);              // 分别排序
    merge(lo, mi, hi);              // 归并
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
    // 各自有序的子向量[lo,mi)和[mi,hi)
    T *A = _elem + lo; // 合并后的子向量A[0,hi-lo) = _elem[lo,hi)

    int lb = mi - lo;
    T *B = new T[lb]; // 前子向量B[0,lb) = _elem[lo,mi)
    for (int i = 0; i < lb; ++i)
    {
        B[i] = A[i];
    }

    int lc = hi - mi;
    T *C = _elem + mi; // 后子向量C[0,lc) = _elem[mi,hi)

    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
    { // B[j],C[k]中的小者添加至A的末尾，相等添加B[j]
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
            A[i++] = B[j++];
        if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
            A[i++] = C[k++];
    }

    delete[] B; // 释放临时空间
} // 归并后得到完整的有序向量[lo,hi)
