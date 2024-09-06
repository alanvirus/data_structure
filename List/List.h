#include "ListNode.h"
#include <cassert>
#include <vector>
#include <iostream>
template <typename T>
class List
{
private:
    Rank _size;
    ListNodePosition(T) header;//head sentinel
    ListNodePosition(T) trailer;//tail sentinel
    // header  <->  first <-> ... <-> last <-> trailer
protected:
    void init();
    Rank clear();
    void copyNodes(ListNodePosition(T) p,Rank n);//用于实现复制构造
    void merge(ListNodePosition(T)& p,Rank n ,List<T>& L,ListNodePosition(T) q,Rank m);
    void mergeSort(ListNodePosition(T)& p,Rank n);
    void selectionSort(ListNodePosition(T)& p,Rank n);//固定O(n^2),selectMax耗时O(n)，改用就地堆排序可以使selectmax变O(logn),整体变O(nlogn)  向量的冒泡就是一种selectionSort，但是还是找最大值再交换常数更小
    void insertionSort(ListNodePosition(T)& p,Rank n);//O(n)-O(n^2)-O(n^2) search操作平均是O(n)的，对向量做insertionsort虽然searchO(1),但是insertO(n)可以冒泡实现,所以同样是O(n^2) //序列平均有O(logn)个无需移动 
                                                      //若所有逆序对的间距不超过k，由于insert时要么消灭逆序对，要么逆序对间距变小或不变，因此insert[r]时最多searchk次，总共就小于O(kn),可见k为常数就变成O（n）
                                                      //0-r-1的insert不会影响L[r]与前面序列形成的逆序对数i[r],那么需要关键码比较i(r)+1次，i(r)求和为I，故比较总次数为(I+n);另外移动是O(n)的，时间取决于I，输入敏感算法
    void radixSort(ListNodePosition(T)& p,Rank n);//改进版计数排序，将数据按位加桶，这样只用准备十个桶就行，最大值有round位，就需要排round次，先排个位再拍十位，... （计数排序是桶范围为1的桶排序），要求使用稳定排序保留前面的成果
public:
    //默认构造,复制构造,析构
    List(){init();}
    List(List<T> const& L);
    List(List<T> const& L,Rank r,Rank n);
    List(ListNodePosition(T) p,Rank n);
    ~List();
    //只读访问接口:规模，判空，首尾节点地址，call-by-rank，地址合法性，是否有序，查找（有序/无序），找最大值
    Rank size()const{return _size;}
    bool empty()const{return _size<=0;}//判空用的是<=不是==？
    ListNodePosition(T) first() const { return header->succ; }
    ListNodePosition(T) last() const { return trailer->pred; }
    ListNodePosition(T) operator[](Rank r) const; // call-by-rank O(n) 向量为O(1)
    bool valid(ListNodePosition(T) p){
        return p&&(p!=header)&&(p!=trailer);}
    Rank disordered()const;
    ListNodePosition(T) find(T const &e) const
    { // call-by-value无序列表查找 O(n) 和无序向量查找一样
        return find(e, _size, trailer);
    }
    ListNodePosition(T) find(T const &e, Rank n, ListNodePosition(T) p) const;
    ListNodePosition(T) search(T const &e) const
    { //call-by-value有序列表查找 O(1)-O(n)-O(n) 有序向量 O(1)-O(logn)-O(logn)
        return search(e, _size, trailer);
    }
    ListNodePosition(T) search(T const &e, Rank n, ListNodePosition(T) p) const;
    ListNodePosition(T) selectMax() const{return selectMax(first(),_size);}
    ListNodePosition(T) selectMax(ListNodePosition(T) p,Rank n) const;//从p及其n-1个后继中选取最大者
    //可写访问接口：插入，删除，二路合并，排序，唯一化（有序，无序），反转，遍历
    ListNodePosition(T) insertAsFirst(T const& e);//插入本身O(1),但寻找插入的位置就不一定了,维护_size,哨兵简化实现
    ListNodePosition(T) insertAsLast(T const& e);
    ListNodePosition(T) insertA(ListNodePosition(T) p,T const& e);
    ListNodePosition(T) insertB(ListNodePosition(T) p,T const& e);
    T remove(ListNodePosition(T) p);//删除本身O(1),但寻找删除的位置就不一定了,维护_size,哨兵简化实现
    void merge(List<T>& L){merge(first(),size,L,L.first(),L.size());}
    void sort(){sort(first(),_size);}
    void sort(ListNodePosition(T) p, Rank n);
    Rank deduplicate();//无序列表唯一化，O(n)-O(n^2)实现
    Rank uniquify();//有序列表唯一化，O(n)
    void reverse();
    void traverse( void(* visit)(T&) );
    template<typename VST>
    void traverse( VST& ); //函数对象相较于函数指针的好处，函数对象中可以存东西，这样在遍历时可以根据某些值修改另一个值
};
template <typename T>
void List<T>::init()
{
    header = new ListNode<T>;
    trailer = new ListNode<T>;
    header->succ = trailer;
    header->pred = nullptr;
    trailer->pred = header;
    trailer->succ = nullptr;
    _size = 0;
}
template <typename T>
ListNodePosition(T) List<T>::operator[](Rank r) const
{ // 一种改进是r>=size/2时从trailer开始倒着找，平均效率而言无意义
    ListNodePosition(T) p = first();
    assert(r < _size && r >= 0);
    while (0 < r--)
    {
        p = p->succ;
    }
    //self adjusting list         访问集中于list的某个子集，也就是data locality较强时可使用，插入总是使用insertAsFirst
    // ListNodePosition(T) firstNode = first();
    // T x=firstNode->data;
    // firstNode->data=p->data;
    // p->data=x;
    // return firstNode;
    return p;
}
template <typename T>
ListNodePosition(T) List<T>::find(T const &e, Rank n, ListNodePosition(T) p) const
{
    while (0 < n--)
    {
        if (e == (p = p->pred)->data)//这里应该保证n不会覆盖header？
            return p;
    }
    return nullptr;
}
template <typename T>
ListNodePosition(T) List<T>::search(T const &e, Rank n, ListNodePosition(T) p) const
{
    while (0 <= n--)//这里是<=，这是和无序列表查找不同的地方
    {
        if (e >= (p = p->pred)->data)
            break;
    }
    return p;
}
template <typename T>
ListNodePosition(T) List<T>::insertAsFirst(T const& e){
    _size++;
    return header->insertAsSucc(e);
}
template <typename T>
ListNodePosition(T) List<T>::insertAsLast(T const& e){
    _size++;
    return trailer->insertAsPred(e);
}
template <typename T>
ListNodePosition(T) List<T>::insertA(ListNodePosition(T) p,T const& e){
    _size++;
    return p->insertAsSucc(e);
}
template <typename T>
ListNodePosition(T) List<T>::insertB(ListNodePosition(T) p,T const& e){
    _size++;
    return p->insertAsPred(e);
}
template <typename T>
void List<T>::copyNodes(ListNodePosition(T) p,Rank n){
    init();
    while(n--){
        insertAsLast(p->data);
        p=p->succ;
    }
}
template <typename T>
List<T>::List(ListNodePosition(T) p,Rank n){
    copyNodes(p,n);
}
template <typename T>
List<T>::List(List<T> const& L,Rank r,Rank n){
    copyNodes(L[r],n);
}
template <typename T>
List<T>::List(List<T> const& L){
    copyNodes(L.first(),L.size());
}
template <typename T>
T List<T>::remove(ListNodePosition(T) p){
    p->succ->pred=p->pred;
    p->pred->succ=p->succ;
    T e=p->data;
    delete p;
    _size--;
    return e;
}
template <typename T>
List<T>::~List(){
    clear();
    delete trailer;
    delete header;
}
template <typename T>
Rank List<T>::clear(){
    Rank old_size=_size;
    while(_size){
        remove(first());
    }
    return old_size;
}
template <typename T>
Rank List<T>::deduplicate(){ //sort+uniquify O(nlogn); 在元素集有限时可用桶排序，桶记录元素出现个数以及是否保留；遍历L，若B[L[i]]>0,记录已保留，后续遇到相同元素删除 O(n)
    Rank old_size=_size;
    ListNodePosition(T) p=header;
    Rank r=0;
    while(trailer!= (p=p->succ)){
        ListNodePosition(T) q=find(p->data,r,p);
        q? remove(q) : r++;
    }
    return old_size-_size;
}
template <typename T>
void List<T>::traverse( void(* visit)(T&) ){
    for(ListNodePosition(T) p=header->succ ; p!=trailer ;p=p->succ){
        visit(p->data);
    }
}
template <typename T> template<typename VST>
void List<T>::traverse( VST& visit){
    for(ListNodePosition(T) p=header->succ ; p!=trailer ;p=p->succ){
        visit(p->data);
    }
}
template <typename T>
Rank List<T>::uniquify(){
    Rank old_size=_size;
    if(old_size<2)//平凡列表无重复
        return 0;
    ListNodePosition(T) p=header->succ;
    ListNodePosition(T) q;
    while(trailer!= (q=p->succ)){
        if(q->data==p->data){
            remove(q);
        }else{
            p=q;
        }
    }
    return old_size-_size;
}
template <typename T>
void List<T>::sort(ListNodePosition(T) p, Rank n){
    radixSort(p,n);
    // switch(rand()%4){
    //     case 1: mergeSort(p,n); break;
    //     case 2: insertionSort(p,n); break;
    //     case 3: selectionSort(p,n); break;
    //     default: radixSort(p,n); break;
    // }
}
template <typename T>
void List<T>::insertionSort(ListNodePosition(T)& p,Rank n){
    ListNodePosition(T) pp=p->pred;
    for(int r=0;r<n;r++){
        insertA(search(p->data,r,p),p->data);
        p=p->succ; remove(p->pred);
    }
    p=pp->succ;
}
template <typename T>
void List<T>::selectionSort(ListNodePosition(T)& p,Rank n){
    int m=n;
    ListNodePosition(T) pp=p->pred;
    ListNodePosition(T) tail=p;
    while(m--){
        tail=tail->succ;
    }
    while(n>1){
        ListNodePosition(T) max=selectMax(pp->succ,n);//这里不能用p。因为p有可能因为是max而被删掉
        insertB(tail,remove(max));//涉及动态申请与释放内存，其实交换max和tail->pred的值就可以，这样pp也可以省略了
                                  //交换前判断tail->pred==max: 在列表为一个0-n-1的排列时，该情况发生次数为循环节数-1（因为n>1）,若元素等概率独立分布，该情况发生概率为当前未排序队列长度分之一，故总数为调和级数（或者直接求排列的循环解暑期望
                                  //）,由于logn/n趋于0，该优化在渐进意义上无意义
        tail=tail->pred;
        n--;
    }
    p=pp->succ;
}
template <typename T>
void List<T>::merge(ListNodePosition(T)& p,Rank n ,List<T>& L,ListNodePosition(T) q,Rank m){
    //valid(p) && rank(p)+n<=_size && sorted(p,n)
    //valid(q) && rank(q)+m<=L._size &&sorted(q,m)
    //归并排序时有this == L && rank(p)+n == rank(q) （重叠是更不能的）
    ListNodePosition(T) pp=p->pred;
    while(m){
        if(n>0&&p->data<=q->data){
            if(q==(p=p->succ)){//这里的break要注意
                break;
            }
            n--;
        }else{
            q=q->succ;
            insertB(p,L.remove(q->pred));
            m--;
        }
    }
    p=pp->succ;//参数p使用引用的原因,p指向新列表起点
}
template <typename T>
void List<T>::mergeSort(ListNodePosition(T)& p,Rank n){
    if(n<2)return;
    Rank m=n>>1;
    ListNodePosition(T) q=p;
    for(int i=0;i<m;i++){  //如果改为选取min{c,n/2}会变为O(n^2),c==1时退化为倒着的插入排序
        q=q->succ;
    }
    mergeSort(p,m);
    mergeSort(q,n-m);
    merge(p,m,*this,q,n-m);
}
template <typename T>
void List<T>::radixSort(ListNodePosition(T)& p,Rank n){
    //默认data为非负整数，取基数k=16
    ListNodePosition(T) max=selectMax(p,n);
    int max_data=max->data;
    int d=0;
    while(max_data>0){
        d++;
        max_data=max_data>>4;
    }
    std::vector<int> bucket[1<<4];
    for(int r=1;r<=d;r++){
        int m=n;
        while(m--){
            bucket[int(p->data%(1<<(r*4))/(1<<(4*r-4)))].push_back(p->data);
            p=p->succ;
        }
        for(int k=15;k>=0;k--){
            while(!bucket[k].empty()){
                (p=p->pred)->data=bucket[k].back();
                bucket[k].pop_back();
            }
        }
    }
}
template <typename T>
void List<T>::reverse(){ //在T的swap较耗时的情况下，可以选择交换每个节点的succ和pred，并交换header和trailer
    if(_size<2)
        return;
    ListNodePosition(T) p=header;
    ListNodePosition(T) q=trailer;
    while((p=p->succ)!=(q=q->pred)){
        T x=p->data;
        p->data=q->data;
        q->data=x;
    }
}
template <typename T>
ListNodePosition(T) List<T>::selectMax(ListNodePosition(T) p,Rank n) const{
    ListNodePosition(T) max=p;
    while(n--){
        if(p->data>=max->data){//后者优先,(前者优先的话雷同元素将完全颠倒顺序)
            max=p;
        }
        p=p->succ;
    }
    return max;
}
template <typename T>
Rank List<T>::disordered()const{//还不如写两个sorted
    if(_size<3)
        return _size;
    ListNodePosition(T) q=header->succ;
    ListNodePosition(T) p=q->succ;
    //根据最靠左的两个不相等元素大小确定order，<为0，>为1
    int order=-1;
    int r=1;
    while(_size>r){
        if(q<p){
            if(order==1){
                return r;
            }
            if(order==-1){
                order=0;
            }
        }
        if(q>p){
            if(order==0){
                return r;
            }
            if(order==-1){
                order=1;
            }
        }
        q=p;
        p=q->succ;
        r++;
    }
    return _size;
}