#ifndef _AVL_TREE_HPP_
#define _AVL_TREE_HPP_

#include <iomanip>
#include <iostream>
using namespace std;

template <class T>
class AVLTreeNode
{
public:
    T key;              
    int height;         
    AVLTreeNode *left;  
    AVLTreeNode *right; 
    AVLTreeNode(T value, AVLTreeNode *l, AVLTreeNode *r) : key(value), height(0), left(l), right(r) {}
};

template <class T>
class AVLTree
{
private:
    AVLTreeNode<T> *mRoot; 
public:
    AVLTree();
    ~AVLTree();
    int height();
    int max(int a, int b);
    void preOrder();
    void inOrder();
    void postOrder();
    AVLTreeNode<T> *search(T key);
    AVLTreeNode<T> *iterativeSearch(T key);
    T minimum();
    T maximum();
    void insert(T key);
    void remove(T key);
    void destroy();
    void print();
private:
    int height(AVLTreeNode<T> *tree);//define the height of null tree as 0
    void preOrder(AVLTreeNode<T> *tree) const;
    void inOrder(AVLTreeNode<T> *tree) const;
    void postOrder(AVLTreeNode<T> *tree) const;
    AVLTreeNode<T> *search(AVLTreeNode<T> *x, T key) const;
    AVLTreeNode<T> *iterativeSearch(AVLTreeNode<T> *x, T key) const;
    AVLTreeNode<T> *minimum(AVLTreeNode<T> *tree);
    AVLTreeNode<T> *maximum(AVLTreeNode<T> *tree);
    AVLTreeNode<T> *leftLeftRotation(AVLTreeNode<T> *k2);
    AVLTreeNode<T> *rightRightRotation(AVLTreeNode<T> *k1);
    AVLTreeNode<T> *leftRightRotation(AVLTreeNode<T> *k3);
    AVLTreeNode<T> *rightLeftRotation(AVLTreeNode<T> *k1);
    AVLTreeNode<T> *insert(AVLTreeNode<T> *&tree, T key);
    AVLTreeNode<T> *remove(AVLTreeNode<T> *&tree, AVLTreeNode<T> *z);
    void destroy(AVLTreeNode<T>* &tree);
    void print(AVLTreeNode<T> *tree, T key, int direction);
};
template <class T>
AVLTree<T>::AVLTree() : mRoot(NULL)
{
}
template <class T>
AVLTree<T>::~AVLTree()
{
    destroy(mRoot);
}
template <class T>
int AVLTree<T>::height(AVLTreeNode<T> *tree)
{
    if (tree != NULL)
        return tree->height;
    return 0;
}
template <class T>
int AVLTree<T>::height()
{
    return height(mRoot);
}
template <class T>
int AVLTree<T>::max(int a, int b)
{
    return a > b ? a : b;
}
template <class T>
void AVLTree<T>::preOrder(AVLTreeNode<T> *tree) const
{
    if (tree != NULL)
    {
        cout << tree->key << " ";
        preOrder(tree->left);
        preOrder(tree->right);
    }
}
template <class T>
void AVLTree<T>::preOrder()
{
    preOrder(mRoot);
}
template <class T>
void AVLTree<T>::inOrder(AVLTreeNode<T> *tree) const
{
    if (tree != NULL)
    {
        inOrder(tree->left);
        cout << tree->key << "(" << tree->height << ") ";
        inOrder(tree->right);
    }
}
template <class T>
void AVLTree<T>::inOrder()
{
    inOrder(mRoot);
}
template <class T>
void AVLTree<T>::postOrder(AVLTreeNode<T> *tree) const
{
    if (tree != NULL)
    {
        postOrder(tree->left);
        postOrder(tree->right);
        cout << tree->key << " ";
    }
}
template <class T>
void AVLTree<T>::postOrder()
{
    postOrder(mRoot);
}
template <class T>
AVLTreeNode<T> *AVLTree<T>::search(AVLTreeNode<T> *x, T key) const
{
    if (x == NULL || x->key == key)
        return x;
    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}
template <class T>
AVLTreeNode<T> *AVLTree<T>::search(T key)
{
    return search(mRoot, key);
}
template <class T>
AVLTreeNode<T> *AVLTree<T>::iterativeSearch(AVLTreeNode<T> *x, T key) const
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
AVLTreeNode<T> *AVLTree<T>::iterativeSearch(T key)
{
    return iterativeSearch(mRoot, key);
}
template <class T>
AVLTreeNode<T> *AVLTree<T>::minimum(AVLTreeNode<T> *tree)
{
    if (tree == NULL)
        return NULL;
    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}
template <class T>
T AVLTree<T>::minimum()
{
    AVLTreeNode<T> *p = minimum(mRoot);
    if (p != NULL)
        return p->key;

    return (T)NULL;
}
template <class T>
AVLTreeNode<T> *AVLTree<T>::maximum(AVLTreeNode<T> *tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->right != NULL)
        tree = tree->right;
    return tree;
}
template <class T>
T AVLTree<T>::maximum()
{
    AVLTreeNode<T> *p = maximum(mRoot);
    if (p != NULL)
        return p->key;
    return (T)NULL;
}
//the following 4 func update height in the subtree
template <class T>
AVLTreeNode<T> *AVLTree<T>::leftLeftRotation(AVLTreeNode<T> *k2)
{
    AVLTreeNode<T> *k1;

    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;

    return k1;
}
template <class T>
AVLTreeNode<T> *AVLTree<T>::rightRightRotation(AVLTreeNode<T> *k1)
{
    AVLTreeNode<T> *k2;

    k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;

    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;

    return k2;
}
template <class T>
AVLTreeNode<T> *AVLTree<T>::leftRightRotation(AVLTreeNode<T> *k3)
{
    k3->left = rightRightRotation(k3->left);

    return leftLeftRotation(k3);
}
template <class T>
AVLTreeNode<T> *AVLTree<T>::rightLeftRotation(AVLTreeNode<T> *k1)
{
    k1->right = leftLeftRotation(k1->right);

    return rightRightRotation(k1);
}
template <class T>
AVLTreeNode<T> *AVLTree<T>::insert(AVLTreeNode<T> *&tree, T key)//due to the balance strategy,no need to worry about stackoverflow, and the rotation no more than 1
{
    if (tree == NULL)
    {
        tree = new AVLTreeNode<T>(key, NULL, NULL);
        if (tree == NULL)
        {
            cout << "ERROR: create avltree node failed!" << endl;
            return NULL;
        }
    }
    else if (key < tree->key) 
    {
        tree->left = insert(tree->left, key);
        if (height(tree->left) - height(tree->right) == 2)
        {
            if (key < tree->left->key)
                tree = leftLeftRotation(tree);
            else
                tree = leftRightRotation(tree);
        }
    }
    else if (key > tree->key) 
    {
        tree->right = insert(tree->right, key);
        if (height(tree->right) - height(tree->left) == 2)
        {
            if (key > tree->right->key)
                tree = rightRightRotation(tree);
            else
                tree = rightLeftRotation(tree);
        }
    }
    else 
    {
        cout << "添加失败：不允许添加相同的节点！" << endl;
    }
    tree->height = max(height(tree->left), height(tree->right)) + 1;
    return tree;
}
template <class T>
void AVLTree<T>::insert(T key)
{
    insert(mRoot, key);
}

/*
 * 删除结点(z)，返回根节点
 *
 * 参数说明：
 *     tree AVL树的根结点
 *     z 待删除的结点
 * 返回值：
 *     根节点
 */
template <class T>
AVLTreeNode<T> *AVLTree<T>::remove(AVLTreeNode<T> *&tree, AVLTreeNode<T> *z)
{
    // 根为空 或者 没有要删除的节点，直接返回NULL。why return NULL when trying to remove NULL
    if (tree == NULL || z == NULL)
        return NULL;
    if (z->key < tree->key) 
    {
        tree->left = remove(tree->left, z);
        if (height(tree->right) - height(tree->left) == 2)
        {
            AVLTreeNode<T> *r = tree->right;
            if (height(r->left) > height(r->right))
                tree = rightLeftRotation(tree);
            else
                tree = rightRightRotation(tree);
        }else{
            tree->height = max(height(tree->left), height(tree->right)) + 1; // 不变或减一
        }
    }
    else if (z->key > tree->key) 
    {
        tree->right = remove(tree->right, z);
        if (height(tree->left) - height(tree->right) == 2)
        {
            AVLTreeNode<T> *l = tree->left;
            if (height(l->right) > height(l->left))
                tree = leftRightRotation(tree); // 子树高度减一
            else
                tree = leftLeftRotation(tree); // 子树高度不变或减一
        } else{
            tree->height = max(height(tree->left), height(tree->right)) + 1; // 不变或减一
        }
    }
    else 
    {
        if ((tree->left != NULL) && (tree->right != NULL))
        {
            if (height(tree->left) > height(tree->right))
            {
                // 如果tree的左子树比右子树高；
                // 则(01)找出tree的左子树中的最大节点
                //   (02)将该最大节点的值赋值给tree。
                //   (03)删除该最大节点。
                // 这类似于用"tree的左子树中最大节点"做"tree"的替身；
                // 采用这种方式的好处是：删除"tree的左子树中最大节点"之后，AVL树仍然是平衡的。
                AVLTreeNode<T> *max = maximum(tree->left);
                tree->key = max->key;
                tree->left = remove(tree->left, max);
            }
            else
            {
                AVLTreeNode<T> *min = maximum(tree->right);
                tree->key = min->key;
                tree->right = remove(tree->right, min);
            }
            tree->height = max(height(tree->left), height(tree->right)) + 1; // 不变或减一
        }
        else // 高度减一
        {
            AVLTreeNode<T> *tmp = tree;
            tree = (tree->left != NULL) ? tree->left : tree->right;
            delete tmp;
        }
    }
    return tree;
}

template <class T>
void AVLTree<T>::remove(T key)
{
    AVLTreeNode<T> *z;

    if ((z = search(mRoot, key)) != NULL)
        mRoot = remove(mRoot, z);
}

/*
 * 打印"二叉查找树"
 *
 * key        -- 节点的键值
 * direction  --  0，表示该节点是根节点;
 *               -1，表示该节点是它的父结点的左孩子;
 *                1，表示该节点是它的父结点的右孩子。
 */
template <class T>
void AVLTree<T>::print(AVLTreeNode<T> *tree, T key, int direction)
{
    if (tree != NULL)
    {
        if (direction == 0) // tree是根节点
            cout << setw(2) << tree->key << " is root" << endl;
        else // tree是分支节点
            cout << setw(2) << tree->key << " is " << setw(2) << key << "'s " << setw(12) << (direction == 1 ? "right child" : "left child") << endl;

        print(tree->left, tree->key, -1);
        print(tree->right, tree->key, 1);
    }
}

template <class T>
void AVLTree<T>::print()
{
    if (mRoot != NULL)
        print(mRoot, mRoot->key, 0);
}

template <class T>
void AVLTree<T>::destroy(AVLTreeNode<T>* &tree)
{
    if (tree==NULL)
        return ;

    if (tree->left != NULL)
        destroy(tree->left);
    if (tree->right != NULL)
        destroy(tree->right);

    delete tree;
}

template <class T>
void AVLTree<T>::destroy()
{
    destroy(mRoot);
}
#endif
