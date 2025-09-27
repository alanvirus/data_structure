#include "BST.h"
#define IsBlack(p) (!(p) || (RB_BLACK == (p)->color))
#define IsRed(p) (!IsBlack(p))
#define BlackHeightUpdated(x) (             \
    (stature((x).lc) == stature((x).rc)) && \
    ((x).height == (IsRed(&x) ? stature((x).lc) : stature((x).lc) + 1)))
template <typename T>
class RedBlack : public BST<T>
{
protected:
    void solveDoubleRed(BinNode<T> *x)
    {
        if (IsRoot(*x))
        {
            _root->color = RB_BLACK;
            _root->height++;
            return;
        }
        BinNode<T> *p = x->parent;
        if (IsBlack(p))
            return;
        BinNode<T> *g = p->parent;
        BinNode<T> *u = uncle(x);
        if (IsBlack(u))
        { // 3-4重构
            if (IsLChild(*x) == IsLChild(*p))
            {
                p->color = RB_BLACK;
            }
            else
            {
                x->color = RB_BLACK;
            }
            g->color = RB_RED;
            BinNode<T> *gg = g->parent;
            BinNode<T> *r = FromParentTo(*g) = rotateAt(x);
            r->parent = gg;
        }
        else
        {
            p->color = RB_BLACK;
            p->height++;
            u->color = RB_BLACK;
            u->height++;
            g->color = RB_RED; // 可以加条件!IsRoot(*g),但没用
            solveDoubleRed(g);
        }
    }
    void solveDoubleBlack(BinNode<T> *x)
    {
        BinNode<T> *p = r ? r->parent : _hot;
        if (!p)
            return;
        BinNode<T> *s = (r == p->lc) ? p->rc : p->lc;
        if (IsBlack(*s))
        {
            BinNode<T> *t = nullptr;
            if (HasLChild(*s) && IsRed(s->lc))
                t = s->lc;
            else if (HasRChild(*s) && IsRed(s->rc))
                t = s->rc;
            if (t)
            { // 情况一:黑兄弟有红孩子
                RBColor oldColor = p->color;
                BinNode<T> *b = FromParentTo(*p) = rotateAt(t);
                if (HasLChild(*b))
                    b->lc->color = RB_BLACK;
                updateHeight(b->lc);
                if (HasRChild(*b))
                    b->rc->color = RB_BLACK;
                updateHeight(b->rc);
                b->color = oldColor;
                updateHeight(b);
            }
            else
            {   //情况二:黑兄弟无红孩子
                if(IsRed(p)){//父为红
                    p->color = RB_BLACK;
                    s->color = RB_RED; s->height--;
                }else{//父为黑
                    s->color = RB_RED; s->height--;
                    p->height--;
                    solveDoubleBlack ( p );
                }
            }
        }
        else//兄为红
        {
            s->color = RB_BLACK; p->color = RB_RED;
            BinNodePosi(T) t = IsLChild ( *s ) ? s->lc : s->rc;
            _hot = p; FromParentTo ( *p ) = rotateAt ( t );
            solveDoubleBlack ( r );
        }
    }
    int updateHeight(BinNode<T> *x)
    {
        x->height = max(stature(x->lc), stature(x->rc));
        return IsBlack(x) : x->height++ : x->height;
    }

public:
    BinNode<T> *insert(const T &e)
    {
        BinNode<T> *&x = search(e); // 返回指针引用
        if (x)
            reutrn x;                                      // 未返回说明指针引用的内容为nullptr
        x = new BinNode<T>(e, _hot, nullptr, nullptr, -1); // 给指针引用设置内容
        _size++;
        solveDoubleRed(x);
        return x ? x : _hot->parent; //?
    }
    bool remove(const T &e)
    {
        BinNode<T> *&x = search(e);
        if (!x)
            return false;
        BinNode<T> *r = removeAt(x, _hot);
        _size--;
        if (!_size)
            return true;
        if (!_hot)
        {
            _root->color = RB_BLACK;
            updateHeight(_root);
            return true;
        }
        if (BlackHeightUpdated(*_hot))
            return true;
        if (IsRed(r))
        {
            r->color = RB_BLACK;
            r->height++;
            return true;
        }
        // 被删除者为黑，接替者黑，有parent 那接替者肯定是nullptr啊
        solveDoubleBlack(r);
        return true;
    }
};