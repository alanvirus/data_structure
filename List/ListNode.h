typedef int Rank;
#define ListNodePosition(T) ListNode<T>*

template<typename T>
struct ListNode{
    T data;
    ListNodePosition(T) pred;
    ListNodePosition(T) succ;
    ListNode() {}
    ListNode(T e,ListNodePosition(T) pred=nullptr,ListNodePosition(T) succ=nullptr):data(e), pred(pred),succ(succ){}
    ListNodePosition(T) insertAsPred(T const&e);
    ListNodePosition(T) insertAsSucc(T const&e);
};
template<typename T>
ListNodePosition(T) ListNode<T>::insertAsSucc(T const&e){
    ListNodePosition(T) x=new ListNode(e,this,succ);
    succ->pred=x;
    succ=x;
    return x;
}
template<typename T>
ListNodePosition(T) ListNode<T>::insertAsPred(T const&e){
    ListNodePosition(T) x=new ListNode(e,pred,this);
    pred->succ=x;
    pred=x;
    return x;
}