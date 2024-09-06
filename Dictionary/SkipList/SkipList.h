//空间复杂度
//对任意0<=k<h,s_k中某一节点在更高层出现的概率为1/2
//E(|S_k|)=n* (1/2^k)
//E(QuadListNode总数)=n*(1+1/2+1/4+...+1/2^(h-1))<2n=O(n)

//get时间复杂度
//E(h)=O(logn),纵跳期望时间O(logn),包括各种其他操作都是每层O(1)的
//对于第k层而言，随机选取一个位置开始向右横跳，跳至最后一个塔顶终止，步数分布为几何分布，期望为1，横跳步数期望肯定不高于1，横跳总期望时间O(logn)
//一层中紧邻塔顶节点构成的列的长度的期望为2，因为从最靠左的塔顶节点开始走，期望是走一步
//put分析类似O(logn)
//remove=get+拆塔+清除空QuadList O(logn)
#include "../Dictionary.h"
#include "../Entry.h"
#include "../../List/List.h"
#include "QuadList.h"
template <typename K, typename V>
class SkipList : public Dictionary<K, V>, public List<QuadList<Entry<K, V>> *>
{
protected:
    bool skipSearch(
        ListNode<QuadList<Entry<K, V>> *> *&qlist,
        QuadListNode<Entry<K, V>> *&p,
        K &k);

public:
    int size() const
    {
        return empty() ? 0 : last()->data->size();
    }
    int level()
    {
        return List::size();
    }
    bool put(K k, V v);
    V *get(K k);
    bool remove(K k);
};
template <typename K, typename V>
V *SkipList<K, V>::get(K k)
{ // 后者优先
    if (empty())
        return nullptr;
    ListNode<QuadList<Entry<K, V>>> * > *qlist = first();
    QuadListNode < Entry < K, V >>> *p = qlist->data->first();
    return skipSearch(qlist, p, k) ? &(p->entry.value) : nullptr;
}
template <typename K, typename V>
bool SkipList<K, V>::skipSearch(
    ListNode<QuadList<Entry<K, V>> *> *&qlist,
    QuadListNode<Entry<K, V>> *&p,
    K &k)
{
    while (true)
    {
        while (p->succ && p->entry.key <= k) // 找到比k大的第一个元素或者trailer
        {
            p = p->succ;
        }
        p = p->pred;
        if (p->pred && (k == p->entry.key))
        { // 这样看如果发现key值雷同的entry，会返回层高最高的entry中的后者
            return true;
        }
        qlist = qlist->succ;
        if (!qlist->succ)
        {
            return false; // 用!p->below是不是也行
        }
        p = (p->pred)? p->below : qlist->data->first(); 
    }
}
template <typename K, typename V>
bool SkipList<K,V>::put(K k, V v) //或许提前确定好高度，从上往下插也行，最好把header们也连接起来
{
    Entry<K,V> e=Entry<K,V>(k,v);
    if (empty()){
        insertAsFirst(new QuadList<Entry<K, V>>);
    }
    ListNode<QuadList<Entry<K, V>>> * > *qlist = first();
    QuadListNode < Entry < K, V >>> *p = qlist->data->first();
    if(skipSearch(qlist, p, k)){//如果没有k雷同元素，p会变成最底层中小于k的那个节点；如果有雷同元素则需转到塔底
        while(p->below) p=p->below;
    } 
    qlist=last();
    QuadListNode < Entry < K, V >>> * b=qlist->data->insertAfterAbove(e,p);
    while(rand()&1){//没有最大高限制
        while(qlist->data->valid(p)&&!p->above){
            p=p->pred;
        }
        //p要么是非塔顶，要么是header sentinel
        if(qlist->data->valid(p)){
            p=p->above;
        }else{
            if(qlist==first()){
                insertAsFirst(new QuadList<Entry<K, V>>);
            }
            p=qlist->pred->data->first()->pred;//由于header之间没有连接，这里比较麻烦
        }
        qlist=qlist->pred;
        b=qlist->data->insertAfterAbove(e,p,b);
    }
    return true;//允许key雷同，故插入必成功
}
template <typename K, typename V>
bool SkipList<K,V>::remove(K k)//有雷同时删除最靠右的
{
     if (empty()){
        return false;
    }
    ListNode<QuadList<Entry<K, V>>> * > *qlist = first();
    QuadListNode < Entry < K, V >>> *p = qlist->data->first();
    if(!skipSearch(qlist, p, k)){
        return false;
    } 
    do{
        QuadListNode < Entry < K, V >>> * b=p->below;
        qlist->data->remove(p);
        p=b;
        qlist=qlist->succ;
    }while(qlist->succ);
    while(!empty()&&fisrt()->data->empty()){
        List::remove(first());
    }
    return true;
}