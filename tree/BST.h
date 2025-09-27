#include"BinTree.h"
template <typename T>
class BST:public BinTree<T>{
protected:
    BinNode<T>* _hot;
    BinNode<T>* connect34(BinNode<T>* a,BinNode<T>* b,BinNode<T>* c,BinNode<T>* T0,BinNode<T>* T1,BinNode<T>* T2,BinNode<T>* T3){
        a->lc=T0;if(T0)T0->parent=a;
        a->rc=T1;if(T1)T1->parent=a;
        updateHeight(a);
        c->lc=T2;if(T2)T2->parent=c;
        c->rc=T3;if(T3)T3->parent=c;
        updateHeight(c);
        b->lc=a;b->rc=c;
        a->parent=b; c->parent=b;
        updataHeight(b);
        return b;
    }
    BinNode<T>* rotateAt(BinNode<T>* v){
        BinNode<T>* p=v->parent;
        BinNode<T>* g=p->parent;
        if(IsLChild(*p)){
            if(IsLChild(*v)){
                p->parent=g->parent;
                return connect34(v,p,g,v->lc,v->rc,p->rc,g->rc);
            }else{
                v->parent=g->parent;
                return connect34(p,v,g,p->lc,v->lc,v->rc,g->rc);
            }
        }else{
            if(IsLChild(*v)){
                v->parent=g->parent;
                return connect34(g,v,p,g->lc,v->lc,v->rc,p->rc);
            }else{
                p->parent=g->parent;
                return connect34(g,p,v,g->lc,p->lc,v->lc,v->rc);
            }
        }
    }
public: 
    virtual BinNode<T>* & search(const T&e){
        _hot=nullptr;
        return searchIn(_root,e,_hot);
    }
    virtual BinNode<T>* insert(const T&e){
        BinNode<T> * &x=search(e);//返回指针引用
        if(x)reutrn x;//未返回说明指针引用的内容为nullptr
        x=new BinNode<T>(e,_hot);//给指针引用设置内容
        _size++;
        updataHeightAbove(x);//从指针指向节点开始更新高度
        return x;//返回指针
    }
    virtual bool remove(const T&e){
        BinNode<T> * &x=search(e);
        if(!x)return false;
        removeAt(x,_hot);
        _size--;
        updataHeightAbove(_hot);
        return true;
    }
};

template <typename T> 
static BinNode<T>* &searchIn(BinNode<T>* & v, const T& e, BinNode<T>* &hot){
    if(!v||v->data==e)return v;
    hot=v;
    return searchIn(((e<v->data)?v->lc:v->rc),e,hot);
}
template <typename T> 
//调用该函数前可保证x不是空指针引用
//x指向待删除节点，如果是根节点，x就是_root的引用；否则x是父节点的lc或rc
//hot是独立指针，如果x为_root引用则hot为nullptr，否则指向父节点
static BinNode<T>* removeAt(BinNode<T>* & x,BinNode<T>* & hot){
    BinNode<T>* w=x;
    BinNode<T>* succ=nullptr;
    if(!x->lc){
        x=x->rc;//指向被删除节点的指针引用指向其右孩子
        succ=x;//succ指向接替者
    }else if(!x->rc){
        x=x->lc;
        succ=x;
    }else{
        w=w->succ();//w指向被删除节点
        swap(x->data,w->data);
        succ=w->rc;
        if(w==x->rc){//或者w->parent==x
            w->parent->rc=succ;
        }else{
            w->parent->lc=succ;
        }
    }
    hot=w->parent;//因为被删节点未必是search到的节点，因此要让hot重新变成被删节点的parent
    if(succ)succ->parent=hot;//如果接替者不为空
    delete w;//release(w->data);release(w);需要考虑一下w->data为指针的情况
    return succ;//返回接替者
}
