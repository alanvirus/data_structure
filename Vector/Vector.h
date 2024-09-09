#include "Fib.h"
typedef int Rank;
#define DEFAULT_CAPACITY 3 // 默认初始容量
template <typename T>
class Vector
{
private:
    Rank _size;     // 规模
    Rank _capacity; // 容量 load factor=_size/_capacity
    T *_elem;       // 数据区,动态分配，因此向量克隆需重载赋值运算符
protected:
    void copyFrom(T const *A, Rank lo, Rank hi); // 用于实现复制构造,复制数组区间A[lo,hi)
    void expand();                               // 空间不足时扩容
    void shrink();                               // 装填因子过小时压缩
    bool bubble(Rank lo, Rank hi);
    void bubbleSort(Rank lo, Rank hi);
    void insertionSort(Rank lo, Rank hi);
    Rank max(Rank lo, Rank hi)const;
    void selectionSort(Rank lo, Rank hi); // 固定O(n^2),selectMax耗时O(n)，改用就地堆排序可以使selectmax变O(logn),整体变O(nlogn)  向量的冒泡就是一种selectionSort，但是还是找最大值再交换常数更小
    void merge(Rank lo, Rank mi, Rank hi);
    void mergeSort(Rank lo, Rank hi);
    Rank partition(Rank lo, Rank hi); // 轴点构造算法
    Rank partition_B(Rank lo, Rank hi);
    void quickSort(Rank lo, Rank hi);
    void heapSort(Rank lo, Rank hi);
    void radixSort(Rank lo, Rank hi);
    // void insertionSort(ListNodePosition(T)& p,Rank n);//O(n)-O(n^2)-O(n^2) search操作平均是O(n)的，对向量做insertionsort虽然searchO(1),但是insertO(n)可以冒泡实现,所以同样是O(n^2) //序列平均有O(logn)个无需移动
    //                                                   //若所有逆序对的间距不超过k，由于insert时要么消灭逆序对，要么逆序对间距变小或不变，因此insert[r]时最多searchk次，总共就小于O(kn),可见k为常数就变成O（n）
    //                                                   //0-r-1的insert不会影响L[r]与前面序列形成的逆序对数i[r],那么需要关键码比较i(r)+1次，i(r)求和为I，故比较总次数为(I+n);另外移动是O(n)的，时间取决于I，输入敏感算法
    // void radixSort(ListNodePosition(T)& p,Rank n);//改进版计数排序，将数据按位加桶，这样只用准备十个桶就行，最大值有round位，就需要排round次，先排个位再拍十位，... （计数排序是桶范围为1的桶排序），要求使用稳定排序保留前面的成果
public:
    // 默认构造,复制构造,析构
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v)
            ;
    }
    Vector(T const *A, Rank n) { copyFrom(A, 0, n); }                           // 整体复制
    Vector(T const *A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }               // 区间复制
    Vector(Vector<T> const &V) { copyFrom(V._elem, 0, V._size); }               // 整体复制
    Vector(Vector<T> const &V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } // 区间复制
    ~Vector()
    {
        delete[] _elem;
        _elem = nullptr;
    } //_elem指向动态分配内存时的释放：谁申请谁释放，由上层调用者确定
    // 只读访问接口:规模，判空，是否非降序排列，查找（有序/无序）
    Rank size() const { return _size; }
    bool empty() const { return !_size; }
    Rank disordered() const;
    Rank find(T const &e) const
    {
        return find(e, 0, _size);
    }
    Rank find(T const &e, Rank lo, Rank hi) const;
    Rank search(T const &e) const
    { // call-by-value有序列表查找 O(1)-O(n)-O(n) 有序向量 O(1)-O(logn)-O(logn)
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    Rank search(T const &e, Rank lo, Rank hi) const;
    // 可写访问接口：call-by-rank，克隆向量，插入，删除，排序，置乱，唯一化（有序，无序），反转，遍历
    T &operator[](Rank r) const; // call-by-rank O(1) list为O(n)
    Vector<T> &operator=(Vector<T> const &V);
    Rank insert(Rank r, T const &e);
    Rank insert(T const &e) { return insert(_size, e); }
    T remove(Rank r);
    int remove(Rank lo, Rank hi);
    void sort() { sort(0, _size); }
    void sort(Rank lo, Rank hi);
    void unsort() { unsort(0, _size); }
    void unsort(Rank lo, Rank hi);
    Rank deduplicate(); // 无序列表唯一化，O(n)-O(n^2)实现
    Rank uniquify();    // 有序列表唯一化，O(n)
    void reverse();
    void traverse(void (*visit)(T &));
    template <typename VST>
    void traverse(VST &); // 函数对象相较于函数指针的好处，函数对象中可以存东西，这样在遍历时可以根据某些值修改另一个值
};
template <typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{ // 习题2-2 若新旧elem有重叠区域，且新数组的某一前缀和旧数组一后缀重叠，需要从后向前转移元素；否则从前向后就ok
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi)
    {
        _elem[_size++] = A[lo++];
    }
}
template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> const &V)
{
    if (_elem)
        delete[] _elem;
    copyFrom(V._elem, 0, V._size);
    return *this;
}
template <typename T>
void Vector<T>::expand()
{ // 使用定长扩容分摊O(n)
    if (_size < _capacity)
        return; // 懒惰策略，不得已时才扩容，装填因子不低于0.5
    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY;
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; _elem[i] = oldElem[i++])
        ;
    delete[] oldElem;
}
template <typename T>
void Vector<T>::shrink()
{ // 分摊O(1),对单次操作执行速度敏感可以忽略该操作
    if (_capacity < DEFAULT_CAPACITY << 1)
        return; // 不能缩到DEFAULT_CAPACITY以下
    if (_size << 2 > _capacity)
        return; // 装填因子超25%,不缩容 或更低，防止出现频繁交替缩容扩容的情况
    T *oldElem = _elem;
    _elem = new T[_capacity >>= 1];
    for (int i = 0; i < _size; _elem[i] = oldElem[i++])
        ;
    delete[] oldElem;
}
template <typename T>
T &Vector<T>::operator[](Rank r) const // 返回引用，取代get和set
{
    return _elem[r]; // assert 0<=r<_size,没有自适应需求
}
template <typename T>
void permute(Vector<T> &V) // 使各元素等概率出现在各位置
{
    for (int i = V.size(); i > 0; i--)
    {
        swap(V[i - 1], V[rand() % i]); // 这里的[]是Vector重载的
    }
}
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi)
{
    T *V = &_elem[lo];
    for (int i = hi - lo; i > 0; i--)
    {
        swap(V[i - 1], V[rand() % i]); // 这里的[]是数组下标访问
    }
}
template <typename T>
Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const // O(1)-O(n)(默认所有元素成为目标概率相等)-O(n)input sensitive
{                                                        // 利用c++短路求值特性，lo<hi--失败后右侧不执行，因此不会越界访问数组
    while ((lo < hi--) && (e != _elem[hi]))
        ;      // 后者优先
    return hi; // 全局查找失败统一返回-1
}
template <typename T>
Rank Vector<T>::insert(Rank r, T const &e)
{ // assert 0<=r<=_size
    expand();
    for (int i = _size; i > r; i--)
    {
        _elem[i] = _elem[i - 1];
    }
    _elem[r] = e;
    _size++;
    return r;
}
template <typename T>
T Vector<T>::remove(Rank r)
{ // assert 0<=r<_size
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{ // 先实现单个删除，再用单个删除实现区间删除?不行，O(n^2)  //assert 0<=lo<=hi<=_size
    if (lo == hi)
        return 0; // 出于效率考虑
    while (hi < _size)
        _elem[lo++] = _elem[hi++];
    _size = lo;
    shrink();
    return hi - lo;
}
template <typename T>
Rank Vector<T>::deduplicate()
{ // sort+uniquify O(nlogn); 会改变原本的相对次序，或许可以克隆数组，但是携带位置标记，sort，标记待删除元素，然后根据位置标记获得一张原向量的待删除bitmap
  // 教材中的方法为从前向后，如果元素与它已经唯一化的前缀中的元素雷同，删除该元素，若元素k雷同，删除O(n-k),不雷同，查找O(k)，因此最优也O(n^2)
  // 优化：Bitmap结构标记得删除元素，最后一块删除,这样查找O(n^2),删除O(n),查找比删除快
  // 在元素集有限时可用桶排序，桶记录元素出现个数以及是否保留；遍历L，若B[V[i]]>0,记录已保留，后续遇到相同元素删除 O(n)
    Rank old_size = _size;
    ListNodePosition(T) p = header;
    Rank r = 0;
    for (int i = _size - 2; i >= 0; i--)
    {
        if (r = find(_elem[i], i + 1, _size))
        {
            remove(r);
        }
    }
    return old_size - _size;
}

template <typename T>
void Vector<T>::traverse(void (*visit)(T &))
{
    for (int i = 0; i < _size; i++)
    {
        visit(_elem[i]);
    }
}
template <typename T>
template <typename VST>
void Vector<T>::traverse(VST &visit)
{
    for (int i = 0; i < _size; i++)
    {
        visit(_elem[i]);
    }
}
template <typename T>
Rank Vector<T>::disordered() const
{
    int n = 0;
    for (int i = 1; i < _size; i++)
    {
        if (_elem[i - 1] > _elem[i])
            n++;
    }
    return n; // 返回相邻逆序对数量，有序（非降序）当且仅当n==0
}
template <typename T>
Rank Vector<T>::uniquify()
{ // O(n)
    Rank i = 0, j = 0;
    while (++j < _size)
    {
        if (_elem[j] != _elem[i])
        {
            _elem[++i] = _elem[j];
        }
    }
    _size = ++i;
    shrink();
    return j - i;
}
template <typename T>
Rank Vector<T>::search(T const &e, Rank lo, Rank hi) const
{ // 0<=lo<hi<=_size,要求不能是空区间，find可以
    // 有序向量中，rank有了新的含义，小于，等于S[r]的元素有i，k个，则该元素及其雷同元素分布于[i,i+k)
    return (rand() % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
}
template <typename T>
static Rank binSearch_three_partition(T *A, T const &e, Rank lo, Rank hi) // 比较操作未必是O(1)的,先认为比较操作用时为常数，并认为查询成功或失败的所有可能情况都等可能，那么成功的平均查找长度为3/2k-1+1.5k/(2^k-1)，失败平均查找长度为1.5k,故渐进均为O(logn),常系数1.5
{
    while (lo < hi)
    {
        Rank mi = (lo + hi) >> 1;
        if (e < A[mi])
            hi = mi;
        else if (e > A[mi])
            lo = mi + 1;
        else
            return mi;
    }
    return -1;
}
template <typename T>
static Rank fibSearch(T *A, T const &e, Rank lo, Rank hi)
{
    Fib fib(hi - lo); // log_phi(n) 该步一分摊就没了
    while (lo < hi)
    {
        while (hi - lo < fib.get())
            fib.prev()
                Rank mi = lo + fib.get() - 1; // fib()-1型分割点
        if (e < A[mi])
            hi = mi;
        else if (e > A[mi])
            lo = mi + 1;
        else
            return mi;
    }
    return -1;
}
template <typename T>
static Rank binSearch_two_partition(T *A, T const &e, Rank lo, Rank hi) // 最好情况不如三分法，但性能稳定且常系数是1
{
    while (lo + 1 < hi)
    {
        Rank mi = (lo + hi) >> 1;
        if (e < A[mi])
            hi = mi;
        else
            lo = mi;
    }
    return (A[lo] == e) ? lo : -1; // 基本是返回小于等于e的元素中最靠后的，如果均比e大，返回第一个，因此对一个做单独判断
}
// 关于search的进一步要求:在有重雷同元素时返回最后者，这种性质对于插入排序的稳定性很关键；查询失败时返回不大于e的最后一个元素的rank而非总是-1
template <typename T>
static Rank binSearch(T *A, T const &e, Rank lo, Rank hi) // 最好情况不如三分法，但性能稳定且常系数是1
{                                                         // 课本证明：[0,lo)中元素均不大于e，[hi,n)中元素均大于e，初始两集合均空成立，后续归纳可证，最终lo==hi时--lo就是不大于e的最靠后元素
    while (lo < hi)                                       // 如果[lo,hi)中有大于e的，返回lo=最靠左的，否则lo=hi返回；边界情况hi-lo==1or2单独证，剩下的归纳可证
    {
        Rank mi = (lo + hi) >> 1;
        if (e < A[mi])
            hi = mi;
        else
            lo = mi + 1;
    }
    return --lo; // 返回小于等于e的元素中最靠后的
}
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
    switch (rand() % 4)
    {
    case 1:
        mergeSort(lo, hi);
        break;
    case 2:
        insertionSort(lo, hi);
        break;
    case 3:
        selectionSort(lo, hi);
        break;
    case 4:
        bubbleSort(lo, hi);
        break;
    case 5:
        heapSort(lo, hi);
        break;
    case 6:
        radixSort(lo, hi);
        break; // bucket sort
    default:
        quickSort(lo, hi);
        break;
    }
}
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{ // 费时版selectionSort
    while (!(bubble(lo, hi--)))
        ;
}
template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
    bool Sorted = true; // 利用该标记进行了一定的优化，渐进无意义
    while (++lo < hi)
    {
        if (_elem[lo - 1] > _elem[lo])
        {
            Sorted = false;
            swap(_elem[lo - 1], _elem[lo]);
        }
    }
    return Sorted;
}
// template <typename T>
// void List<T>::insertionSort(ListNodePosition(T) & p, Rank n)
// {
//     ListNodePosition(T) pp = p->pred;
//     for (int r = 0; r < n; r++)
//     {
//         insertA(search(p->data, r, p), p->data);
//         p = p->succ;
//         remove(p->pred);
//     }
//     p = pp->succ;
// }
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) // 稳定排序
{                                                // lo<=mi<=hi
    T *A = &_elem[lo];
    int lb = mi - lo;
    T *B = new T[lb];
    for (Rank i = 0; i < lb; B[i] = A[i++])
        ;
    int lc = hi - mi;
    T *C = &_elem[mi];
    for (Rank i = 0, j = 0; k = 0; (j < lb) || (k < lc);)
    { // s=i+j递增且0~hi-lo，theta(n)
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
            A[i++] = B[j++];
        if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
            A[i++] = C[k++];
    }
    delete[] B;
}
template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) // 在[lo,hi]中选择pivot mi，使得[lo,mi)中的元素不大于_elem[mi],(mi,hi]中的元素不小于_elem[mi]
                                            // pivot存在，则满足：排序后mi秩不变，A[lo,hi)和S[lo,hi)中的元素一致，(hi,lo]中一致、
                                            // pivot可能不存在，但是可以人为构造
                                            // 该算法并不稳定 655
                                            // 两个子问题规模取决于随机选取的pivot在有序向量中的rank r=0或n-1,最坏的情况
                                            // 退化情况：几乎有序的向量:T(n)=T(0)+T(n-1)+O(n)=T(n-1)+O(n)=...=T(1)+O(n^2)=O(n^2)
                                            // 退化情况：元素大量重复：和上面类似
                                            // 解决办法：1.随机选取pivot 2.三者取中法：随机取三个点，用居中者做pivot，统计上更居中
                                            // T(n)=O(n)+sigma_0^n-1(T(i)+T(n-1-i))/n 在pivot在n个数中等概率随机选取时，O(1.386nlog_2(n))
{
    swap(_elem[lo], _elem[lo + rand() % (hi - lo + 1)]);
    T pivot = _elem[lo];
    while (lo < hi)
    {
        while ((lo < hi) && (_elem[hi] >= pivot))
        {
            hi--;
        }
        _elem[lo] = _elem[hi]; //_elem[lo++]=_elem[hi];会导致lo和hi哪个是空位无法确定，参考B版本
        while ((lo < hi) && (_elem[lo] <= pivot))
        {
            lo++;
        }
        _elem[hi] = _elem[lo]; // 被挪动的元素会重复比较两次_elem[hi--]=_elem[lo];
    }
    _elem[lo] = pivot;
    return lo;
}
template <typename T>
Rank Vector<T>::partition_B(Rank lo, Rank hi) // 勤于交换重复元素，应对退化情况2
{
    swap(_elem[lo], _elem[lo + rand() % (hi - lo + 1)]);
    T pivot = _elem[lo];
    while (lo < hi)
    {
        while (lo < hi)
        {
            if (_elem[hi] > pivot)
            {
                hi--;
            }
            else
            {
                _elem[lo++] = _elem[hi];
            }
        }
        while (lo < hi)
        {
            if (_elem[lo] < pivot)
            {
                lo++;
            }
            else
            {
                _elem[hi--] = _elem[lo];
            }
        }
    }
    _elem[lo] = pivot;
    return lo;
}
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (hi - lo < 2) // 递归基
        return;
    Rank mi = (lo + hi) >> 1; // 子问题划分O(1),子任务独立，子任务规模接近
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi); // 由子问题的解得到原问题的解O(n)
}
template <typename T>
void Vector<T>::quickSort(Rank lo, Rank hi)
{
    if (hi - lo < 2)
        return;
    Rank mi = partition(lo, hi - 1); // 子任务划分O(n),子任务独立，但无法保证子任务规模接近
    quickSort(lo, mi);
    quickSort(mi + 1, hi); // 由子问题的解立马得到原问题的解
}
// template <typename T>
// void List<T>::radixSort(ListNodePosition(T) & p, Rank n)
// {
//     // 默认data为非负整数，取基数k=16
//     ListNodePosition(T) max = selectMax(p, n);
//     int max_data = max->data;
//     int d = 0;
//     while (max_data > 0)
//     {
//         d++;
//         max_data = max_data >> 4;
//     }
//     std::vector<int> bucket[1 << 4];
//     for (int r = 1; r <= d; r++)
//     {
//         int m = n;
//         while (m--)
//         {
//             bucket[int(p->data % (1 << (r * 4)) / (1 << (4 * r - 4)))].push_back(p->data);
//             p = p->succ;
//         }
//         for (int k = 15; k >= 0; k--)
//         {
//             while (!bucket[k].empty())
//             {
//                 (p = p->pred)->data = bucket[k].back();
//                 bucket[k].pop_back();
//             }
//         }
//     }
// }
// template <typename T>
// void List<T>::reverse()
// { // 在T的swap较耗时的情况下，可以选择交换每个节点的succ和pred，并交换header和trailer
//     if (_size < 2)
//         return;
//     ListNodePosition(T) p = header;
//     ListNodePosition(T) q = trailer;
//     while ((p = p->succ) != (q = q->pred))
//     {
//         T x = p->data;
//         p->data = q->data;
//         q->data = x;
//     }
// }
template <typename T>
Rank Vector<T>::max(Rank lo, Rank hi) const
{
    Rank max=lo;
    while((++lo<hi)&&(_elem[lo]>_elem[max])){
        max=lo;
    }
    return max;
}
template <typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi)
{
    int m = n;
    ListNodePosition(T) pp = p->pred;
    ListNodePosition(T) tail = p;
    while (m--)
    {
        tail = tail->succ;
    }
    while (n > 1)
    {
        ListNodePosition(T) max = selectMax(pp->succ, n); // 这里不能用p。因为p有可能因为是max而被删掉
        insertB(tail, remove(max));                       // 涉及动态申请与释放内存，其实交换max和tail->pred的值就可以，这样pp也可以省略了
                                    // 交换前判断tail->pred==max: 在列表为一个0-n-1的排列时，该情况发生次数为循环节数-1（因为n>1）,若元素等概率独立分布，该情况发生概率为当前未排序队列长度分之一，故总数为调和级数（或者直接求排列的循环解暑期望
                                    // ）,由于logn/n趋于0，该优化在渐进意义上无意义
        tail = tail->pred;
        n--;
    }
    p = pp->succ;
}