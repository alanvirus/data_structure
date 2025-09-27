#include"../stack/Stack.h"
#include"../queue/Queue.h"
#define stature(p) ((p) ? (p)->height : -1)
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
#define FromParentTo(x)  \ 
  ( IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )
#define uncle(x) (IsLChild(*((x)->parent))?(x)->parent->parent->rc:(x)->parent->parent->lc) 
#define sibling(x) (IsLChild(*(x))?(x)->parent->rc:(x)->parent->lc)
typedef enum
{
    RB_RED,
    RB_BLACK
} RBColor;
template <typename T>
struct BinNode
{
    T data;
    BinNode<T> *parent;
    BinNode<T> *lc;
    BinNode<T> *rc;
    int height;
    RBColor color;

    BinNode() : parent(nullptr), lc(nullptr), rc(nullptr), height(0), color(RB_RED) {}
    BinNode(T e, BinNode<T> *p = nullptr, BinNode<T> *lc = nullptr, BinNode<T> *rc = nullptr, int h = 0, RBColor c = RB_RED)
        : data(e), parent(p), lc(lc), rc(rc), height(h), color(c) {}
    BinNode<T> *insertAsLC(T const &e) { return lc = new BinNode(e, this); }
    BinNode<T> *insertAsRC(T const &e) { return rc = new BinNode(e, this); }
    BinNode<T> *succ(){
        BinNode<T> *x;
        if(this->rc){
            x=this->rc;
            while(x->lc){
                x=x->lc;
            }
        }else{
            x=this;
            while(IsRChild(*x)){
                x=x->parent;
            }
            x=x->parent;
        }
        return x;
    }
    bool operator<(BinNode const &bn) { return data < bn.data; }
    bool operator>(BinNode const &bn) { return data > bn.data; }
    bool operator==(BinNode const &bn) { return data == bn.data; }
    bool operator!=(BinNode const &bn) { return data != bn.data; }

    template <typename VST>
    void travLevel(VST &visit); // 子树局次遍历
    template <typename VST>
    void travPre(VST &visit); // 子树先序遍历 25
    template <typename VST>
    void travIn(VST &visit); // 子树中序遍历 26
    template <typename VST>
    void travPost(VST &visit); // 子树后序遍历
};
template <typename T>
template <typename VST> 
void BinNode<T>::travIn(VST &visit)
{ 
    switch (rand() % 5)
    { 
    case 1:
        travIn_I1(this, visit);
        break; 
    case 2:
        travIn_I2(this, visit);
        break; 
    default:
        travIn_R(this, visit);
        break; 
    }
}
template <typename T>
template <typename VST> 
void BinNode<T>::travPre(VST &visit)
{ 
    switch (rand() % 5)
    { 
    case 1:
        travPre_I1(this, visit);
        break; 
    case 2:
        travPre_I2(this, visit);
        break; 
    default:
        travPre_R(this, visit);
        break; 
    }
}
template <typename T>
template <typename VST> 
void BinNode<T>::travPost(VST &visit)
{ 
    switch (rand() % 5)
    { 
    case 1:
        travPost_I1(this, visit);
        break; 
    default:
        travPost_R(this, visit);
        break; 
    }
}
template <typename T>
template <typename VST> 
void BinNode<T>::travLevel(VST &visit)
{ 
    Queue<BinNode<T>*> S;
    S.enqueue(this);
    while(!S.empty()){
        BinNode<T>*x=S.dequeue();
        visit(x->data);
        if(x->lc)S.enqueue(x->lc);
        if(x->rc)S.enqueue(x->rc);
    }
}

template <typename T,typename VST>
void travPre_R(BinNode<T>*x,VST &visit)
{ 
    if(!x)return;
    visit(x->data);
    travPre_R(x->lc,visit);
    travPre_R(x->rc,visit);
}
template <typename T,typename VST>
void travIn_R(BinNode<T>*x,VST &visit)
{ 
    if(!x)return;
    travIn_R(x->lc,visit);
    visit(x->data);
    travIn_R(x->rc,visit);
}
template <typename T,typename VST>
void travPost_R(BinNode<T>*x,VST &visit)
{ 
    if(!x)return;
    travPost_R(x->lc,visit);
    travPost_R(x->rc,visit);
    visit(x->data);
}
template <typename T,typename VST>
void travPre_I1(BinNode<T>*x,VST &visit)
{ 
    Stack<BinNode<T>*> S;
    if(x)S.push(x);
    while(!S.empty()){
        x=S.pop();
        visit(x->data);
        if(x->rc)S.push(x->rc);
        if(x->lc)S.push(x->lc);
    }
}
template <typename T,typename VST>
void travPre_I2(BinNode<T>*x,VST &visit)
{ 
    Stack<BinNode<T>*> S;
    while(true){
        if(x){
            visit(x->data);
            S.push(x);
            x=x->lc;
        }else{
            if(S.empty())break;
            x=S.pop()-rc;
        }
    }
}
template <typename T,typename VST>
void travIn_I1(BinNode<T>*x,VST &visit)
{ 
    Stack<BinNode<T>*> S;
    while(true){
        while(x){
            S.push(x);
            x=x->lc;
        }
        if(S.empty())break;
        visit(S.top()->data);
        x=S.pop()-rc;
    }
}
template <typename T,typename VST>
void travIn_I2(BinNode<T>*x,VST &visit)
{ 
    Stack<BinNode<T>*> S;
    while(true){
        if(x){
            S.push(x);
            x=x->lc;
        }else{
            if(S.empty())break;
            visit(S.top()->data);
            x=S.pop()-rc;
        }
    }
}
template <typename T,typename VST>
void travIn_I3(BinNode<T>*x,VST &visit)
{ 
    while(x&&x->lc){
        x=x->lc;
    }
    while(x){
        visit(x->data);
        if(!(x=x->succ()))break;//如果x是一颗子树，这里的succ不对，得先把子树分离出来遍历
    }
}
template <typename T,typename VST>
void travPost_I1(BinNode<T>*x,VST &visit) //I2：后序遍历等价于从右向左先序遍历然后reverse
{ 
    if(!x)return;
    Stack<BinNode<T>*> S;
    S.push(x);
    while(!S.empty()){
        while(S.top()!=x->parent){
            BinNode<T>* y;
            while(y=S.top()){
                if(HasLChild(*y)){
                    if(HasRChild(*y))S.push(y->rc);
                    S.push(y->lc);
                }else{
                    S.push(y->rc);
                }
            }
            S.pop();
        }
        x=S.pop();
        visit(x->data);
    }
}