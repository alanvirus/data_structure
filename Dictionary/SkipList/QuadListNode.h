#include"../Entry.h"
#define QuadListNodePosition(T) QuadListNode<T>*

template<typename T>
struct QuadListNode{
    T entry;
    QuadListNodePosition(T) pred;
    QuadListNodePosition(T) succ;
    QuadListNodePosition(T) above;
    QuadListNodePosition(T) below;
    QuadListNode(T e=T(),QuadListNodePosition(T) p=nullptr,QuadListNodePosition(T) s=nullptr,QuadListNodePosition(T) a=nullptr,QuadListNodePosition(T) b=nullptr):
        entry(e),pred(p),succ(s),above(a),below(b){}
    QuadListNodePosition(T) insertAsSuccAbove(T const& e,QuadListNodePosition(T) b=nullptr);
};
template<typename T>
QuadListNodePosition(T) QuadListNode<T>::insertAsSuccAbove(T const& e,QuadListNodePosition(T) b=nullptr){
    QuadListNodePosition(T) x=new QuadListNode<T>(e,this,succ,nullptr,b);
    succ->pred=x;
    succ=x;
    if(b){//b->above总是nullptr
        // x->below=b;
        // x->above=b->above;
        // if(b->above)b->above->below=x;
        b->above=x;
    }
    return x;
}