/**
 * C++ 语言: 红黑树
 * 参考origin实现
 */
#include <iomanip>
#include <iostream>
using namespace std;
enum RBTColor
{
    RED,
    BLACK
};
template <class T>
class RBTNode
{
public:
    RBTColor color;
    T key;
    RBTNode *left;
    RBTNode *right;
    RBTNode *parent;
    RBTNode(T value, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) : key(value), color(c), parent(), left(l), right(r) {}
};
template <class T>
class RBTree
{
private:
    RBTNode<T> *mRoot;

public:
    RBTree();
    ~RBTree();
    void preOrder();
    void inOrder();
    void postOrder();
    RBTNode<T> *search(T key);
    RBTNode<T> *iterativeSearch(T key);
    T minimum();
    T maximum();
    RBTNode<T> *successor(RBTNode<T> *x);
    RBTNode<T> *predecessor(RBTNode<T> *x);
    void insert(T key);
    void remove(T key);
    void destroy();
    void print();

private:
    void preOrder(RBTNode<T> *tree) const;
    void inOrder(RBTNode<T> *tree) const;
    void postOrder(RBTNode<T> *tree) const;
    RBTNode<T> *search(RBTNode<T> *x, T key) const;
    RBTNode<T> *iterativeSearch(RBTNode<T> *x, T key) const;
    RBTNode<T> *minimum(RBTNode<T> *tree);
    RBTNode<T> *maximum(RBTNode<T> *tree);
    void leftRotate(RBTNode<T> *&root, RBTNode<T> *x);
    void rightRotate(RBTNode<T> *&root, RBTNode<T> *y);
    void insert(RBTNode<T> *&root, RBTNode<T> *node);
    void insertFixUp(RBTNode<T> *&root, RBTNode<T> *node);
    void remove(RBTNode<T> *&root, RBTNode<T> *node);
    void removeFixUp(RBTNode<T> *&root, RBTNode<T> *node, RBTNode<T> *parent);
    void destroy(RBTNode<T> *&tree);
    void print(RBTNode<T> *tree, T key, int direction);
#define rb_parent(r) ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r) ((r)->color == RED)
#define rb_is_black(r) ((r)->color == BLACK)
#define rb_set_black(r)     \
    do                      \
    {                       \
        (r)->color = BLACK; \
    } while (0)
#define rb_set_red(r)     \
    do                    \
    {                     \
        (r)->color = RED; \
    } while (0)
#define rb_set_parent(r, p) \
    do                      \
    {                       \
        (r)->parent = (p);  \
    } while (0)
#define rb_set_color(r, c) \
    do                     \
    {                      \
        (r)->color = (c);  \
    } while (0)
};
template <class T>
RBTree<T>::RBTree() : mRoot(NULL)
{
    mRoot = NULL;
}
template <class T>
RBTree<T>::~RBTree()
{
    destroy();
}
template <class T>
void RBTree<T>::preOrder(RBTNode<T> *tree) const
{
    if (tree != NULL)
    {
        cout << tree->key << "(" << (tree->color ? "b" : "r") << ")" << " ";
        preOrder(tree->left);
        preOrder(tree->right);
    }
}
template <class T>
void RBTree<T>::preOrder()
{
    preOrder(mRoot);
}
template <class T>
void RBTree<T>::inOrder(RBTNode<T> *tree) const
{
    if (tree != NULL)
    {
        inOrder(tree->left);
        cout << tree->key << "(" << tree->color << ")" << " ";
        inOrder(tree->right);
    }
}
template <class T>
void RBTree<T>::inOrder()
{
    inOrder(mRoot);
}
template <class T>
void RBTree<T>::postOrder(RBTNode<T> *tree) const
{
    if (tree != NULL)
    {
        postOrder(tree->left);
        postOrder(tree->right);
        cout << tree->key << "(" << tree->color << ")" << " ";
    }
}
template <class T>
void RBTree<T>::postOrder()
{
    postOrder(mRoot);
}
template <class T>
RBTNode<T> *RBTree<T>::search(RBTNode<T> *x, T key) const
{
    if (x == NULL || x->key == key)
        return x;
    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}
template <class T>
RBTNode<T> *RBTree<T>::search(T key)
{
    search(mRoot, key);
}
template <class T>
RBTNode<T> *RBTree<T>::iterativeSearch(RBTNode<T> *x, T key) const
{
    while ((x != NULL) && (x->key != key))
    {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}
template <class T>
RBTNode<T> *RBTree<T>::iterativeSearch(T key)
{
    iterativeSearch(mRoot, key);
}
template <class T>
RBTNode<T> *RBTree<T>::minimum(RBTNode<T> *tree)
{
    if (tree == NULL)
        return NULL;
    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}
template <class T>
T RBTree<T>::minimum()
{
    RBTNode<T> *p = minimum(mRoot);
    if (p != NULL)
        return p->key;
    return (T)NULL;
}
template <class T>
RBTNode<T> *RBTree<T>::maximum(RBTNode<T> *tree)
{
    if (tree == NULL)
        return NULL;
    while (tree->right != NULL)
        tree = tree->right;
    return tree;
}
template <class T>
T RBTree<T>::maximum()
{
    RBTNode<T> *p = maximum(mRoot);
    if (p != NULL)
        return p->key;
    return (T)NULL;
}
template <class T>
RBTNode<T> *RBTree<T>::successor(RBTNode<T> *x)
{
    if (x->right)
        return minimum(x->right);
    RBTNode<T> *y = x->parent;
    while ((y != NULL) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }
    return y;
}
template <class T>
RBTNode<T> *RBTree<T>::predecessor(RBTNode<T> *x)
{
    if (x->left != NULL)
        return maximum(x->left);
    RBTNode<T> *y = x->parent;
    while ((y != NULL) && (x == y->left))
    {
        x = y;
        y = y->parent;
    }
    return y;
}
/*
 * 对红黑树的节点(x)进行左旋转, 默认y存在，如果需要可断言
 *
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \      --(左旋)-->           / \                #
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 *
 *
 */
template <class T>
void RBTree<T>::leftRotate(RBTNode<T> *&root, RBTNode<T> *x)
{
    RBTNode<T> *y = x->right;
    if ((x->right = y->left))
        x->right->parent = x;
    if ((y->parent = x->parent))
    {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    else
        root = y;
    y->left = x;
    x->parent = y;
}
/*
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 *
 */
template <class T>
void RBTree<T>::rightRotate(RBTNode<T> *&root, RBTNode<T> *y)
{
    RBTNode<T> *x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
    {
        root = x; // 如果 “y的父亲” 是空节点，则将x设为根节点
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x; // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
        else
            y->parent->left = x; // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
    }
    x->right = y;
    y->parent = x;
}
template <class T>
void RBTree<T>::insertFixUp(RBTNode<T> *&root, RBTNode<T> *node)
{
    RBTNode<T> *parent, *gparent, *uncle;
    int rotate_direct;
    while (node->parent && node->parent->color == RED)
    {
        parent = node->parent;
        gparent = parent->parent;
        if (gparent->left == parent)
        {
            rotate_direct = 0;
            uncle = gparent->right;
        }
        else
        {
            rotate_direct = 1;
            uncle = gparent->left;
        }
        if (uncle == NULL || uncle->color == BLACK)
        {
            if (rotate_direct == 1 && node == parent->left)
            {
                rightRotate(root, parent);
                parent = node;
            }
            else if (rotate_direct == 0 && node == parent->right)
            {
                leftRotate(root, parent);
                parent = node;
            }
            parent->color = BLACK;
            gparent->color = RED;
            if (rotate_direct)
                leftRotate(root, gparent);
            else
                rightRotate(root, gparent);
            break;
        }
        uncle->color = BLACK;
        parent->color = BLACK;
        gparent->color = RED;
        node = gparent;
    }
    root->color = BLACK;
}
template <class T>
void RBTree<T>::insert(RBTNode<T> *&root, RBTNode<T> *node)
{
    RBTNode<T> *parent = NULL;
    RBTNode<T> *pos = root;
    while (pos != NULL)
    {
        parent = pos;
        if (node->key < pos->key)
        {
            pos = pos->left;
        }
        else
        {
            pos = pos->right;
        }
    }
    if (parent == NULL)
    {
        root = node;
    }
    else
    {
        if (node->key < parent->key)
        {
            parent->left = node;
        }
        else
        {
            parent->right = node;
        }
    }
    node->parent = parent;
    node->color = RED;
    insertFixUp(root, node);
}
template <class T>
void RBTree<T>::insert(T key)
{
    RBTNode<T> *z = NULL;
    if ((z = new RBTNode<T>(key, BLACK, NULL, NULL, NULL)) == NULL)
        return;
    insert(mRoot, z);
}
/*
 * 红黑树删除修正函数
 * 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 * node和parent中间被删去了一个黑点
 * 参数说明：
 *     root 红黑树的根
 *     node 待修正的节点
 */
template <class T>
void RBTree<T>::removeFixUp(RBTNode<T> *&root, RBTNode<T> *node, RBTNode<T> *parent)
{
    RBTNode<T> *bro;
    int direction;
    while ((!node || node->color == BLACK) && parent != NULL)
    {
        if (node == parent->left)
        {
            bro = parent->right;
            direction = 0;
        }
        else
        {
            bro = parent->left;
            direction = 1;
        }
        if (bro->color == RED)
        {
            bro->color = BLACK;
            parent->color = RED;
            if (direction)
                rightRotate(root, parent);
            else
                leftRotate(root, parent);
            bro = (direction == 0 ? parent->right : parent->left);
        }
        if ((bro->left == NULL || bro->left->color == BLACK) && (bro->right == NULL || bro->right->color == BLACK))
        {
            bro->color = RED;
            node = parent;
            parent = node->parent;
        }
        else
        {
            RBTNode<T> *nephew = (direction == 0 ? bro->right : bro->left);
            if (nephew == NULL || nephew->color == BLACK)
            {
                RBTNode<T> *nephew2 = (direction == 1 ? bro->right : bro->left);
                nephew2->color = BLACK;
                bro->color = RED;
                if (direction)
                    leftRotate(root, bro);
                else
                    rightRotate(root, bro);
                bro = nephew2;
                nephew = bro;
            }
            bro->color = parent->color;
            parent->color = BLACK;
            nephew->color = BLACK;
            if (direction)
                rightRotate(root, parent);
            else
                leftRotate(root, parent);
            // node=root;//应该没必要
            break;
        }
    }
    if (node)
        node->color = BLACK;
}
template <class T>
void RBTree<T>::remove(RBTNode<T> *&root, RBTNode<T> *node)
{
    if (node->left != NULL && node->right != NULL)
    {
        RBTNode<T> *suc = successor(node);
        node->key = suc->key;
        node = suc;
    }
    RBTNode<T> *child, *parent = node->parent;
    if (node->left)
        child = node->left;
    else
        child = node->right;
    if (child)
        child->parent = parent;
    if (parent == NULL)
        root = child;
    else if (parent->left == node)
        parent->left = child;
    else
        parent->right = child;
    if (node->color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}
/*
 * 删除红黑树中键值为key的节点
 *
 * 参数说明：
 *     tree 红黑树的根结点
 */
template <class T>
void RBTree<T>::remove(T key)
{
    RBTNode<T> *node;
    if ((node = search(mRoot, key)) != NULL)
        remove(mRoot, node);
}
/*
 * 销毁红黑树
 */
template <class T>
void RBTree<T>::destroy(RBTNode<T> *&tree)
{
    if (tree == NULL)
        return;
    if (tree->left != NULL)
        destroy(tree->left);
    if (tree->right != NULL)
        destroy(tree->right);
    delete tree;
}
template <class T>
void RBTree<T>::destroy()
{
    destroy(mRoot);
}

template <class T>
void RBTree<T>::print(RBTNode<T> *tree, T key, int direction)
{
    if (tree != NULL)
    {
        if (direction == 0) // tree是根节点
            cout << setw(2) << tree->key << "(B) is root" << endl;
        else // tree是分支节点
            cout << setw(2) << tree->key << (rb_is_red(tree) ? "(R)" : "(B)") << " is " << setw(2) << key << "'s " << setw(12) << (direction == 1 ? "right child" : "left child") << endl;
        print(tree->left, tree->key, -1);
        print(tree->right, tree->key, 1);
    }
}
template <class T>
void RBTree<T>::print()
{
    if (mRoot != NULL)
        print(mRoot, mRoot->key, 0);
}
