#include"../Vector/Vector.h"
template<typename T>
struct BTNode{
    BTNode<T>* parent;
    Vector<T> key;//n
    Vector<BTNode<T>*>child;//n+1
    BTNode(){
        parent=nullptr;
        child.insert(0,nullptr);
    }
    BTNode(T e,BTNode<T>* lc=nullptr,BTNode<T>* rc=nullptr){
        parent=nullptr;
        key.insert(0,e);
        child.insert(0,lc);
        child.insert(1,rc);
        if(lc)lc->parent=this;
        if(rc)rc->parent=this;
    }
};

template<typename T>
class BTree{
    protected:
        int _size;
        int _order;//阶数，至少为3 n+1>=m/2
        BTNode<T>* _root;
        BTNode<T>* _hot;
        void solveOverflow(BTNode<T>*v){
            if(_order>=v->child.size())return;
            int s=order/2;
            BTNode<T>* u=new BTNode<T>();
            for(int j=0;j<_order-s-1;j++){
                u->child.insert(j,v->child.remove(s+1));
                u->key.insert(j,v->key.remove(s+1));
            }
            u->child[_order-s-1]=v->child.remove(s+1);
            if(u->child[0]){
                for(int j=0;j<_order-s;j++){
                    u->child[i]->parent=u;
                }
            }
            BTNode<T>* p=v->parent;
            if(!p){
                _root=p=new BTNode<T>();
                p->child[0]=v;v->parent=p;
            }
            int r=1+p->key.search(v->key[0]);
            p->key.insert(r,v->key.remove(s));
            p->child.insert(r+1,u);
            u->parent=p;
            solveOverflow(p);
        }
        void solveUnderflow(BTNode<T>* v){
            if((_order+1)/2<=v->child.size())return;
            BTNode<T>* p=v->parent;
            if(!p){
                if(!v->key.size()&&v->child[0]){
                    _root=v->child[0];
                    _root->parent=nullptr;
                    v->child[0]=nullptr;
                    delete v;
                }
                return;
            }
            int r;
            while(p->child[r]!=v)r++;//v为p的第r个孩子
            if(r>0){//有左兄弟
                BTNode<T>* ls=p->child[r-1];
                if((_order+1)/2<ls->child.size()){
                    v->key.insert(0,p->key[r-1]);
                    p->key[r-1]=ls->key.remove(ls->key.size()-1);
                    v->child.insert(0,ls->child.remove(ls->child.size()-1));
                    if(v->child[0])v->child[0]->parent=v;
                    return;
                }
            }
            if(r<p->child.size()-1){//有右兄弟
                BTNode<T>* rs=p->child[r+1];
                if((_order+1)/2<rs->child.size()){
                    v->key.insert(v->key.size(),p->key[r]);
                    p->key[r]=rs->key.remove(0);
                    v->child.insert(v->child.size(),rs->child.remove(0));
                    if(v->child[v->child.size()-1])v->child[v->child.size()-1]->parent=v;
                    return;
                }
            }
            if(0<r){//与左兄弟合并
                ls->key.insert(ls->key.size(),p->key.remove(r-1));
                p->child.remove(r);
                ls->child.insert(ls->child.size(),v->child.remove(0));
                if(ls->child[ls->child.size()-1])ls->child[ls->child.size()-1]->parent=ls;
                while(!v->key.empty()){
                    ls->key.insert(ls->key.size(),v->key.remove(0));
                    ls->child.insert(ls->child.size(),v->child.remove(0));
                    if(ls->child[ls->child.size()-1])ls->child[ls->child.size()-1]->parent=ls;
                }
                delete v;
            }else{
                rs->key.insert(0,p->key.remove(r));
                p->child.remove(r);
                rs->child.insert(0,v->child.remove(v->child.size()-1));
                if(rs->child[0])rs->child[0]->parent=rs;
                while(!v->key.empty()){
                    rs->key.insert(0,v->key.remove(v->key.size()-1));
                    rs->child.insert(0,v->child.remove(v->key.size()-1));
                    if(rs->child[0])rs->child[0]->parent=ls;
                }
                delete v;
            }
            solveUnderflow(p);
            return;
        }
    public:
        BTree(int order=3){_order=order; _size=0;_root=new BTNode<T>()}
        ~BTree(){if(_root)delete _root};//release ( _root )释放所有节点
        int const order(){return _order;}
        int const size(){return _size;}
        BTNode<T>*& root(){return _root;}
        bool empty() const{return !_root};
        BTNode<T>* search(const T& e){
            BTNode<T>* v=_root;
            _hot=nullptr;
            while(v){
                Rank r=v->key.search(e);
                if(r>=0&&e==v->key[r])return v;
                _hot=v;v=v->child[r+1];//I/O
            }
            return nullptr;
        }
        bool insert(const T& e){
            BTNode<T>* v=search(e);
            if(v)return false;//禁止重复关键码
            int r=_hot->key.search(e);
            _hot->key.insert(r+1,e);
            _hot->child.insert(r+2,nullptr);
            _size++;
            solveOverflow(_hot);
            return true;
        }
        bool remove(const T& e){
            BTNode<T>* v=search(e);
            if(!v)return false;//没有自然删不了
            int r=v->key.search(e);
            if(v->child[0]){
                BTNode<T>* u=v->child[r+1];
                while(u->child[0])u=u->child[0];
                v->key[r]=u->key[0]lv=u;r=0;
            }
            v->key.remove(r);
            v->child.remove(r+1);
            _size--;
            solveUnderflow(v);
            return true;
        }
};