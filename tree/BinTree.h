#include "BinNode.h"
template <typename T>
class BinTree
{
protected:
    int _size;
    BinNode<T> *_root;
    virtual int updateHeight(BinNode<T> *x);
    void updataHeightAbove(BinNode<T> *x);

public:
    BinTree() : _size(0), _root(nullptr) {}
    ~BinTree()
    {
        if (0 < _size)
            remove(_root);
    }
    int size() const { return _size; }
    bool empty() const { return !_root; }
    BinNode<T> *root() const { return _root; }
    BinNode<T> *insertAsRoot(T const &e);
    BinNode<T> *insertAsLC(BinNode<T> *x, T const &e);
    BinNode<T> *insertAsRC(BinNode<T> *x, T const &e);
    BinNode<T> *attachAsLC(BinNode<T> *x, BinTree<T> *&T);
    BinNode<T> *attachAsRC(BinNode<T> *x, BinTree<T> *&T);
    int remove(BinNode<T> *x);
    BinTree<T> *secede(BinNode<T> *x);
    template <typename VST>
    void travLevel(VST &visit)
    {
        if (_root)
            _root->travLevel(visit);
    }
    template <typename VST>
    void travPre(VST &visit)
    {
        if (_root)
            _root->travPre(visit);
    }
    template <typename VST>
    void travIn(VST &visit)
    {
        if (_root)
            _root->travIn(visit);
    }
    template <typename VST>
    void travPost(VST &visit)
    {
        if (_root)
            _root->travPost(visit);
    }
    // bool operator<(BinTree<T> const &t)
    // {
    //     return _root && t._root && lt(_root, t._root);
    // }
    // bool operator==(BinTree<T> const &t)
    // {
    //     return _root && t._root && (_root == t._root);
    // }
};
template <typename T>
int BinTree<T>::updateHeight(BinNode<T> *x)
{
    return x->height = 1 + max(stature(x->lc), stature(x->rc));
}
template <typename T>
void BinTree<T>::updataHeightAbove(BinNode<T> *x)
{ // 节点难以发现后代的高度变化，因此更新变化节点的所有祖先
    while (x)
    {
        updateHeight(x); // 此处可在x高度不变时break
        x = x->parent;
    }
}
template <typename T>
BinNode<T> *BinTree<T>::insertAsRoot(T const &e)
{
    _size = 1;
    return _root = new BinNode<T>(e);
}
template <typename T>
BinNode<T> *BinTree<T>::insertAsLC(BinNode<T> *x, T const &e)
{
    x->insertAsLC(e);
    updataHeightAbove(x);
    _size++;
    return x->lc;
}
template <typename T>
BinNode<T> *BinTree<T>::insertAsRC(BinNode<T> *x, T const &e)
{
    x->insertAsRC(e);
    updataHeightAbove(x);
    _size++;
    return x->rc;
}
template <typename T>
BinNode<T> *BinTree<T>::attachAsLC(BinNode<T> *x, BinTree<T> *&T){//返回接入位置
    _size+=T->_size;
    if(x->lc=T->_root){
        x->lc->parent=x;
    }
    updataHeightAbove(x);
    T->_root=nullptr;
    T->_size=0;
    delete T;
    T=nullptr;
    return x;
}
template <typename T>
BinNode<T> *BinTree<T>::attachAsRC(BinNode<T> *x, BinTree<T> *&T){//返回接入位置
    _size+=T->_size;
    if(x->rc=T->_root){
        x->rc->parent=x;
    }
    updataHeightAbove(x);
    T->_root=nullptr;
    T->_size=0;
    delete T;
    T=nullptr;
    return x;
}
template <typename T>
int BinTree<T>::remove(BinNode<T> *x){
    FromParentTo(*x)=nullptr;
    updataHeightAbove(x->parent);
    int n=removeAt(x);
    _size-=n;
    return n;
}
template <typename T>
static int removeAt(BinNode<T> *x){
    if(!x){
        return 0;
    }
    int n=1;
    n+=removeAt(x->lc);
    n+=removeAt(x->rc);
    release(x->data);//有一个通用的release方法
    release(x);
    return n;
}
template <typename T>
BinTree<T> *BinTree<T>::secede(BinNode<T> *x){
    FromParentTo(*x)=nullptr;
    updataHeightAbove(x->parent);
    x->parent=nullptr;
    int n=Size(x);
    _size-=n;
    BinTree<T> newT=new BinTree<T>();
    newT->_size=n;
    newT->_root=x;
    return newT;
}
template <typename T>
static int Size(BinNode<T> *x){
    if(!x){
        return 0;
    }
    int n=1;
    n+=Size(x->lc);
    n+=Size(x->rc);
    return n;
}