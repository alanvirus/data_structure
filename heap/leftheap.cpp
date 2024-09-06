//
// Created by Jayson on 2023/8/8.
// modify merge->mergeIter
#include "bintree.cpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;
#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) ((2 * (i)) + 1)
#define RIGHT(i) ((2 * (i)) + 2)
template <typename T>
class PQ_LeftHeap : public BinTree<T, int>
{
public:
    PQ_LeftHeap() : BinTree<T, int>() {}
    PQ_LeftHeap(vector<T> &new_elems) : BinTree<T, int>() // Considering that complete binary tree conforms to the NPL principle, use Floyd method to create leftheap in O(n)
    {
        int size = new_elems.size();
        TreeNode<T, int> **new_elem_vec = new TreeNode<T, int> *[size];
        for (int i = 0; i < size; i++)
        {
            new_elem_vec[i] = new TreeNode<T, int>(new_elems[i], 1);
        }
        if (size > 1)
        {
            for (int i = PARENT(size - 1); i >= 0; i--)
            {
                while (1)
                {
                    int biggest = i;
                    int l = LEFT(i);
                    int r = RIGHT(i);
                    if (l < size && new_elem_vec[l]->key > new_elem_vec[i]->key)
                        biggest = l;
                    if (r < size && new_elem_vec[r]->key > new_elem_vec[biggest]->key)
                        biggest = r;
                    if (biggest == i)
                        break;
                    swap(new_elem_vec[i], new_elem_vec[biggest]);
                    i = biggest;
                }
            }
        }
        for (int i = size - 1; i > 0; i--)
        {
            new_elem_vec[i]->parent = new_elem_vec[PARENT(i)];
            if (i % 2)
                new_elem_vec[PARENT(i)]->left = new_elem_vec[i];
            else
                new_elem_vec[PARENT(i)]->right = new_elem_vec[i];
            updateNPL_single(new_elem_vec[i]);
        }
        if (size)
        {
            new_elem_vec[0]->parent = nullptr;
            updateNPL_single(new_elem_vec[0]);
            this->root = new_elem_vec[0];
        }
        delete[] new_elem_vec;
    }
    PQ_LeftHeap(vector<T> &new_elems, bool method2) : BinTree<T, int>()
    {
        int size = new_elems.size();
        if (size == 0)
            return;
        TreeNode<T, int> **new_elem_vec = new TreeNode<T, int> *[size];
        for (int i = 0; i < size; i++)
        {
            new_elem_vec[i] = new TreeNode<T, int>(new_elems[i], 1);
        }
        while (size > 1)
        {
            for (int cur = 0; cur < size - 1 - cur; cur++)
                new_elem_vec[cur] = mergeIter(new_elem_vec[cur], new_elem_vec[size - 1 - cur]);
            size = (size + 1) / 2;
        }
        this->root = new_elem_vec[0];
        delete[] new_elem_vec;
    }
    void insert(T e);
    void insert_ori(T e);
    T getMax();
    T delMax()
    {
        TreeNode<T, int> *lheap = this->root->left;
        TreeNode<T, int> *rheap = this->root->right;
        T e = this->root->key;
        delete this->root;
        this->root = mergeIter(lheap, rheap);
        if (this->root)
            this->root->parent = nullptr;
        return e;
    }
    T deleteKey(TreeNode<T, int> *del)
    {
        TreeNode<T, int> *lheap = del->left;
        TreeNode<T, int> *rheap = del->right;
        TreeNode<T, int> *parent = del->parent;
        T e = del->key;
        int direct = 0;
        if (del->parent)
            direct = (del == del->parent->left) ? 1 : 0;
        delete del;
        del = mergeIter(lheap, rheap);
        if (del)
            del->parent = parent;
        if (!parent)
            this->root = del;
        else if (direct)
            parent->left = del;
        else
            parent->right = del;
        while (parent)
        {
            updateNPL_single(parent);
            if (!parent->left && parent->right)
                swap(parent->left, parent->right);
            else if (parent->left && parent->right && parent->left->val < parent->right->val)
                swap(parent->left, parent->right);
            else
                break;
            parent = parent->parent;
        }
        return e;
    }
    void changeKey(TreeNode<T, int> *del, T new_val)
    {
        del->key = new_val;
        while (del->parent && del->key > del->parent->key)
        {
            T tmp = del->key;
            del->key = del->parent->key;
            del->parent->key = tmp;
            del = del->parent;
        }
        while (1)
        {
            TreeNode<T, int> * biggest = del;
            TreeNode<T, int> * l = del->left;
            TreeNode<T, int> * r = del->right;
            if (l && l->key>del->key)
                biggest = l;
            if (r && r->key>biggest->key)
                biggest = r;
            if (biggest == del)
                break;
            T tmp = del->key;
            del->key = biggest->key;
            biggest->key = tmp;
            del=biggest;
        }
    }
    PQ_LeftHeap<T> copy()
    {
        PQ_LeftHeap<T> newtree;
        newtree.root = this->copyTree(this->root);
        return newtree;
    }
};
template <typename T>
int updateNPL(TreeNode<T, int> *v)
{ // 离外部节点的最近距离
    if (!v->left || !v->right)
    {
        v->val = 1;
        return 1;
    }
    int n = updateNPL(v->left);
    int m = updateNPL(v->right);
    v->val = min(n, m) + 1;
    return v->val;
}
template <typename T>
int updateNPL_single(TreeNode<T, int> *v)
{
    if (!v->left || !v->right)
    {
        v->val = 1;
        return 1;
    }
    int n = v->left->val;
    int m = v->right->val;
    v->val = min(n, m) + 1;
    return v->val;
}

template <typename T>
TreeNode<T, int> *merge(TreeNode<T, int> *a, TreeNode<T, int> *b)
{
    if (!a)
        return b;
    if (!b)
        return a;
    if (a->key < b->key)
        swap(a, b);
    a->right = merge(a->right, b);
    a->right->parent = a;
    updateNPL(a);
    if (!a->left || a->left->val < a->right->val)
        swap(a->left, a->right);
    return a;
}
template <typename T>
TreeNode<T, int> *mergeIter(TreeNode<T, int> *a, TreeNode<T, int> *b)
{
    TreeNode<T, int> parent(T(), -1);
    TreeNode<T, int> *current = &parent;
    while (a && b)
    {
        TreeNode<T, int> *&tmp = (a->key < b->key) ? b : a; // 这里引用必须每次新创建一个
        current->right = tmp;
        tmp->parent = current;
        current = tmp;
        tmp = tmp->right;
    }
    if (a)
    {
        current->right = a;
        a->parent = current;
    }
    else if (b)
    {
        current->right = b;
        b->parent = current;
    }
    while (current->val > 0)
    { // 能进入该loop说明两个树都有元素，current的右孩子不为空
        updateNPL_single(current);
        if (!current->left || current->left->val < current->right->val)
            swap(current->left, current->right);
        current = current->parent;
    }
    if (current->right)
        current->right->parent = nullptr;
    return current->right;
}
template <typename T>
PQ_LeftHeap<T> merge_tree(PQ_LeftHeap<T> &a, PQ_LeftHeap<T> &b) // 保留原来的tree
{
    PQ_LeftHeap<T> a_copy = a.copy();
    PQ_LeftHeap<T> b_copy = b.copy();
    PQ_LeftHeap<T> tmp;
    tmp.root = mergeIter(a_copy.root, b_copy.root);
    return tmp;
}
template <typename T>
void PQ_LeftHeap<T>::insert(T e)
{
    if (!this->root)
    {
        this->root = new TreeNode<T, int>(e, 1);
        return;
    }
    TreeNode<T, int> *v = new TreeNode<T, int>(e, 1);
    this->root = mergeIter(this->root, v);
    this->root->parent = nullptr;
}
template <typename T>
void PQ_LeftHeap<T>::insert_ori(T e)
{
    if (!this->root)
    {
        this->root = new TreeNode<T, int>(e, 1);
        return;
    }
    TreeNode<T, int> *v = new TreeNode<T, int>(e, 1);
    this->root = merge(this->root, v);
    this->root->parent = nullptr;
}
template <typename T>
T PQ_LeftHeap<T>::getMax()
{
    if (this->root)
        return this->root->key;
    return (T)NULL;
}
template <typename T>
void create(PQ_LeftHeap<T> &tree, int num, int key)
{
    while (num > 0)
    {
        tree.insert(key);
        num--;
    }
}
template <typename T>
void create_ori(PQ_LeftHeap<T> &tree, int num, int key)
{
    while (num > 0)
    {
        tree.insert_ori(key);
        num--;
    }
}
int main()
{
    std::vector<int> elements = {10, 20, 5, 6, 1, 8, 9, 3, 6, 5};
    PQ_LeftHeap<int> heap(elements, true);
    heap.preOrder();
    TreeNode<int, int> *node = heap.root->left->left->left;
    // cout << heap.deleteKey(node) << endl;
    heap.changeKey(heap.root,1);
    heap.preOrder();
    // PQ_LeftHeap<int> leftHeap1; // 3右
    // PQ_LeftHeap<int> leftHeap2; // 2右
    // PQ_LeftHeap<int> leftHeap3; // 空
    // PQ_LeftHeap<int> leftHeap4; // 空
    // PQ_LeftHeap<int> leftHeap5; // 1右
    // // std::cout<<(1<<2-1);
    // // auto start2 = std::chrono::high_resolution_clock::now();
    // // create_ori(leftHeap2,(1<<15)-1,1);
    // // auto end2 = std::chrono::high_resolution_clock::now();
    // // std::chrono::duration<double, std::milli> duration2 = end2 - start2;
    // // std::cout << "运行时间: " << duration2.count() << " 毫秒" << std::endl;

    // auto start = std::chrono::high_resolution_clock::now();
    // create(leftHeap1, (1 << 12) - 1, 1);
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double, std::milli> duration = end - start;
    // std::cout << "runtime: " << duration.count() << " ms" << std::endl;
    // leftHeap1.preOrder();
    // leftHeap2.preOrder();
    // Insert elements into the leftist heap
    // leftHeap1.insert(10);
    // leftHeap1.insert(20);
    // leftHeap1.insert(5);
    // leftHeap1.insert(30);
    // leftHeap1.insert(15);
    // leftHeap1.insert(10);
    // leftHeap1.insert(10);

    // leftHeap2.insert(10);
    // leftHeap2.insert(20);
    // leftHeap2.insert(5);
    // leftHeap2.insert(25);
    // leftHeap2.insert(13);

    // leftHeap5.insert(13);

    // // Get the maximum element
    // // std::cout << "Max element: " << leftHeap1.getMax() << std::endl; // Should print 30
    // leftHeap1.preOrder();
    // leftHeap2.preOrder();
    // leftHeap3.preOrder();
    // leftHeap4.preOrder();
    // leftHeap5.preOrder();

    // // PQ_LeftHeap<int> leftHeap6 = merge_tree(leftHeap3, leftHeap5);
    // // // PQ_LeftHeap<int> leftHeap7 = merge_tree(leftHeap4, leftHeap3);
    // // leftHeap6.preOrder();
    // // // leftHeap7.preOrder();
    // // std::cout<<std::endl<<(leftHeap6.root->parent==nullptr);
    // // Delete the maximum element
    // std::cout << "Deleted max element: " << leftHeap1.delMax() << std::endl; // Should print 30
    // std::cout << "New max element: " << leftHeap1.getMax() << std::endl;     // Should print 20
    // leftHeap1.preOrder();
    // // Delete the max element again
    // std::cout << "Deleted max element: " << leftHeap1.delMax() << std::endl; // Should print 20
    // std::cout << "New max element: " << leftHeap1.getMax() << std::endl;     // Should print 15
    // leftHeap1.preOrder();
    // // Continue deleting elements
    // std::cout << "Deleted max element: " << leftHeap1.delMax() << std::endl; // Should print 15
    // std::cout << "New max element: " << leftHeap1.getMax() << std::endl;     // Should print 10
    // leftHeap1.preOrder();
    // std::cout << "Deleted max element: " << leftHeap1.delMax() << std::endl; // Should print 10
    // // After this, heap should have only one element (5)
    // std::cout << "New max element: " << leftHeap1.getMax() << std::endl; // Should print 5
    // leftHeap1.preOrder();
    // std::cout << "Deleted max element: " << leftHeap1.delMax() << std::endl; // Should print 5
    // // Now heap should be empty, let's check the max again
    // if (leftHeap1.getMax())
    // {
    //     std::cout << "New max element: " << leftHeap1.getMax() << std::endl; // Should not exist
    // }
    // else
    // {
    //     std::cout << "Heap is empty now." << std::endl; // Expected output
    // }
    // leftHeap1.preOrder();
    return 0;
}