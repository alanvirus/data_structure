#include"BST.h"
#define Balanced(x) (stature((x).lc)==stature((x).rc))
#define BalFac(x) (stature((x).lc)-stature((x).rc))
#define AvlBalanced(x) ((-2<BalFac(x))&&(BalFac(x)<2))
#define tallerChild(x) ( \
        stature((x)->lc)>stature((x)->rc)?(x)->lc:( \ 
        stature((x)->lc)<stature((x)->rc)?(x)->rc:( \
        IsLChild(*(x))?(x)->lc:(x)->rc \
        )\
     )\         
    )
template <typename T>
class AVL:public BST<T>{
    public:
        //重写插入删除，search不变
        BinNode<T>* insert(const T&e){
            BinNode<T> * &x=search(e);//返回指针引用
            if(x)reutrn x;//未返回说明指针引用的内容为nullptr
            x=new BinNode<T>(e,_hot);//给指针引用设置内容
            _size++;
            for(BinNode<T> *g=_hot;g;g=g->parent){
                if(!AvlBalanced(*g)){
                    FromParentTo(*g)=rotateAt(tallerChild(tallerChild(g)));
                    break;//高度自动恢复，不用update了
                }else{
                    updateHeight(g);
                }
            }
            return x;//返回指针
        }
        bool remove(const T&e){
            BinNode<T> * &x=search(e);
            if(!x)return false;
            removeAt(x,_hot);
            _size--;
            for(BinNode<T> *g=_hot;g;g=g->parent){
                if(!AvlBalanced(*g)){
                    g=FromParentTo(*g)=rotateAt(tallerChild(tallerChild(g)));
                }
                updateHeight(g);
            }
            return true;
        }
};