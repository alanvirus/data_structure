#include "QuadListNode.h"
template <typename T>
class QuadList
{
private:
    int _size;
    QuadListNodePosition(T) header;  // head sentinel
    QuadListNodePosition(T) trailer; // tail sentinel
protected:
    void init();
    int clear();
public:
    QuadList() { init(); }
    ~QuadList();
    // 只读访问接口:规模，判空，首尾节点地址，地址合法性
    int size() const { return _size; }
    bool empty() const { return _size <= 0; }
    QuadListNodePosition(T) first() const { return header->succ; }
    QuadListNodePosition(T) last() const { return trailer->pred; }
    bool valid(QuadListNodePosition(T) p)
    {
        return p && (p != header) && (p != trailer);
    }
    // 可写访问接口：插入，删除，遍历
    QuadListNodePosition(T) insertAfterAbove(T const &e, QuadListNodePosition(T) p, QuadListNodePosition(T) b=nullptr); //作为p的后继，b的上邻插入
    T remove(QuadListNodePosition(T) p);
    void traverse(void (*visit)(T &));
    template <typename VST>
    void traverse(VST &); 
};
template <typename T>
void QuadList<T>::init()
{
    header = new QuadListNode<T>;
    trailer = new QuadListNode<T>;
    header->succ = trailer;
    trailer->pred = header;
    _size = 0;
}
template <typename T>
QuadListNodePosition(T) QuadList<T>::insertAfterAbove(T const &e, QuadListNodePosition(T) p, QuadListNodePosition(T) b=nullptr){
    _size++;
    return p->insertAsSuccAbove(e,b);
}
template <typename T>
T QuadList<T>::remove(QuadListNodePosition(T) p){
    p->succ->pred=p->pred;
    p->pred->succ=p->succ;
    // if(p->above)p->above->below=p->below;//above必为空
    // if(p->below)p->below->above=p->above;//below一会儿会被删或为空
    T e=p->entry;
    delete p;
    _size--;
    return e;
}
template <typename T>
QuadList<T>::~QuadList(){
    clear();
    delete trailer;
    delete header;
}
template <typename T>
int QuadList<T>::clear(){
    int old_size=_size;
    while(_size){
        remove(first());
    }
    return old_size;
}
template <typename T>
void QuadList<T>::traverse( void(* visit)(T&) ){
    for(QuadListNodePosition(T) p=header->succ ; p!=trailer ;p=p->succ){
        visit(p->entry);
    }
}
template <typename T> template<typename VST>
void QuadList<T>::traverse( VST& visit){
    for(QuadListNodePosition(T) p=header->succ ; p!=trailer ;p=p->succ){
        visit(p->entry);
    }
}
