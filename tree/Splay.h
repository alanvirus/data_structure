#include"BST.h"
template<typename Pos> inline
void attachAsLChild(Pos v,Pos lc){v->lc=lc;if(lc)lc->parent=v;}
template<typename Pos> inline
void attachAsRChild(Pos v,Pos rc){v->rc=rc;if(rc)rc->parent=v;}
template <typename T>
class Splay:public BST<T>{
    protected:
        BinNode<T>* splay(BinNode<T>* v){
            if(!v)return nullptr;
            BinNode<T>*p;BinNode<T>*g;
            while((p=v->parent)&&(g=p->parent)){
                BinNode<T>*gg=g->parent;
                if(IsLChild(*v)){
                    if(IsLChild(*p)){//zigzig
                        attachAsLChild(g,p->rc);
                        attachAsLChild(p,v->rc);
                        attachAsRChild(v,p);
                        attachAsRChild(p,g);                        
                    }else{//zigzag
                        attachAsRChild(g,v->lc);
                        attachAsLChild(p,v->rc);
                        attachAsRChild(v,p);
                        attachAsLChild(v,g);         
                    }
                }else{
                    if(IsRChild(*p)){//zagzag
                        attachAsRChild(g,p->lc);
                        attachAsRChild(p,v->lc);
                        attachAsLChild(v,p);
                        attachAsLChild(p,g);       
                    }else{//zagzig
                        attachAsLChild(g,v->rc);
                        attachAsRChild(p,v->lc);
                        attachAsLChild(v,p);
                        attachAsRChild(v,g);    
                    }
                }
                if(!gg)v->parent=nullptr;
                else (g==gg->lc)?attachAsLChild ( gg, v ) : attachAsRChild ( gg, v ); 
                updateHeight ( g ); updateHeight ( p ); updateHeight ( v ); 
            }
            if(p=v->parent){
                if(IsLChild(*v)){
                    attachAsLChild(p,v->lc);
                    attachAsRChild(v,p);
                }else{
                    attachAsRChild(p,v->rc);
                    attachAsLChild(v,p);
                }
                updateHeight ( p ); updateHeight ( v ); 
            }
            v->parent=nullptr;
            return v;
        }
    public:
        //重写
        BinNode<T>* & search(const T&e){
            _hot=nullptr;
            BinNode<T>* p=searchIn(_root,e,_hot);
            _root=splay(p?p:_hot);//没找到会把最后一个点伸展至根
            return _root;
        }
        BinNode<T>* insert(const T&e){
            if(!_root){
                _size++;
                _root=new BinNode<T>(e);
                return _root;
            }
            if(e==search(e)->data)return _root;
            _size++;
            BinNode<T>*t=_root;
            if(_root->data<e){
                t->parent=_root=new BinNode<T>(e,nullptr,t,t->rc);
                if(HasRChild(*t)){
                    t->rc->parent=_root;
                    t->rc=nullptr;
                }
            }else{
                t->parent=_root=new BinNode<T>(e,nullptr,t->lc,t);
                if(HasLChild(*t)){
                    t->lc->parent=_root;
                    t->lc=nullptr;
                }
            }
            updateHeightAbove(t);
            return _root;
        }
        bool remove(const T&e){
            if(!_root||(e!=search(e)->data))return false;
            BinNode<T>*t=_root;
            if(!HasLChild(*_root)){
                _root=_root->rc;
                if(_root)_root->parent=nullptr;
            }else if(!HasRChild(*_root)){
                _root=_root->lc;
                if(_root)_root->parent=nullptr;
            }else{
                BinNode<T>*lTree=_root->lc;
                _root->lc=nullptr;
                _root=_root->rc;
                _root->parent=nullptr;
                search(e);
                _root->lc=lTree;
                lTree->parent=_root;
            }
            delete t;//release ( w->data ); release ( w );
            _size--;
            if(_root)updateHeight(_root);//第三种情况需要
            return true;
        }
};