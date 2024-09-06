#include <iostream>
template <typename T, typename V>
class TreeNode
{
public:
    T key;
    V val;
    TreeNode *parent;
    TreeNode *left;
    TreeNode *right;
    TreeNode(T k, V v)
        : key(k), val(v), parent(nullptr), left(nullptr), right(nullptr) {}
};
template <typename T, typename V>
class BinTree
{
public:
    BinTree() : root(nullptr) {}
    void insertRec(T key, V val = V())
    {
        TreeNode<T, V> *newNode = new TreeNode<T, V>(key, val);
        if (root == nullptr)
        {
            root = newNode;
        }
        else
        {
            insertRec(root, newNode);
        }
    }
    void insertIter(T key, V val = V())
    {
        TreeNode<T, V> *newNode = new TreeNode<T, V>(key, val);
        insertIter(root, newNode);
    }
    TreeNode<T, V> *deleteIter(T key)
    {
        TreeNode<T, V> *tmp = search(key);
        if (tmp)
            return deleteIter(root, tmp); // 不负责回收节点
        return tmp;
    }
    TreeNode<T, V> *search(T key)
    {
        return searchIter(root, key);
    }
    void preOrder()
    {
        preOrderRec(root);
        std::cout << std::endl;
    }
    void inOrder()
    {
        inOrderRec(root);
        std::cout << std::endl;
    }
    BinTree<T, V> copy()
    {
        BinTree<T, V> newtree;
        newtree.root = copyTree(this->root);
        return newtree;
    }
    TreeNode<T, V> *copyTree(TreeNode<T, V> *root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        TreeNode<T, V> *newNode = new TreeNode<T, V>(root->key, root->val);
        newNode->parent = root->parent;
        newNode->left = copyTree(root->left);
        newNode->right = copyTree(root->right);
        return newNode;
    }
    TreeNode<T, V> *root;

protected:
    void insertRec(TreeNode<T, V> *current, TreeNode<T, V> *newNode)
    {
        if (newNode->key < current->key)
        {
            if (current->left == nullptr)
            {
                current->left = newNode;
                newNode->parent = current;
            }
            else
            {
                insertRec(current->left, newNode);
            }
        }
        else
        {
            if (current->right == nullptr)
            {
                current->right = newNode;
                newNode->parent = current;
            }
            else
            {
                insertRec(current->right, newNode);
            }
        }
    }
    void insertIter(TreeNode<T, V> *&root, TreeNode<T, V> *newnode)
    {
        TreeNode<T, V> *current = root;
        TreeNode<T, V> *parent = nullptr;
        while (current && newnode->key != current->key)
        {
            if (current->key > newnode->key)
            {
                parent = current;
                current = current->left;
            }
            else
            {
                parent = current;
                current = current->right;
            }
        }
        if (current)
            return;
        newnode->parent = parent;
        if (!parent)
            root = newnode;
        else if (parent->key > newnode->key)
        {
            parent->left = newnode;
        }
        else
        {
            parent->right = newnode;
        }
    }
    TreeNode<T, V> *searchRec(TreeNode<T, V> *current, T key)
    {
        if (current == nullptr || current->key == key)
        {
            return current;
        }
        if (key < current->key)
        {
            return searchRec(current->left, key);
        }
        return searchRec(current->right, key);
    }
    TreeNode<T, V> *searchIter(TreeNode<T, V> *root, T key)
    {
        TreeNode<T, V> *current = root;
        while (current && current->key != key)
        {
            if (key < current->key)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        return current;
    }
    TreeNode<T, V> *deleteIter(TreeNode<T, V> *&root, TreeNode<T, V> *to_delete)
    {
        if (to_delete->left && to_delete->right)
        {
            TreeNode<T, V> *replace = find_next(to_delete);
            T key = to_delete->key;
            V val = to_delete->val;
            to_delete->key = replace->key;
            to_delete->val = replace->val;
            replace->key = key;
            replace->val = val;
            return deleteIter(root, replace);
        }
        TreeNode<T, V> *child;
        if (to_delete->left)
            child = to_delete->left;
        else
            child = to_delete->right;
        if (!to_delete->parent)
            root = child;
        else if (to_delete->parent->left == to_delete)
            to_delete->parent->left = child;
        else
            to_delete->parent->right = child;
        if (child)
            child->parent = to_delete->parent;
        return to_delete;
    }
    TreeNode<T, V> *find_next(TreeNode<T, V> *current)
    {
        if (current->right)
        {
            current = current->right;
            while (current->left)
                current = current->left;
            return current;
        }
        while (current->parent && current->parent->right == current)
            current = current->parent;
        return current->parent;
    }
    void preOrderRec(TreeNode<T, V> *current)
    {
        if (current != nullptr)
        {
            // std::cout << current->key << "(" << current->val << ")(" << (current->parent ? current->parent->key : -1) << ") ";
            std::cout << current->key << "(" << current->val <<") ";
            preOrderRec(current->left);
            preOrderRec(current->right);
        }
    }
    void inOrderRec(TreeNode<T, V> *current)
    {
        if (current != nullptr)
        {
            inOrderRec(current->left);
            std::cout << current->key << "(" << current->val << ") ";
            inOrderRec(current->right);
        }
    }
};
/*
int main()
{
    BinTree<int, std::string> tree;

    // 插入节点
    tree.insertIter(10, "Ten");
    tree.insertIter(5, "Five");
    tree.insertIter(15, "Fifteen");
    tree.insertIter(3, "Three");
    tree.insertIter(7, "Seven");
    tree.insertIter(12, "Twelve");
    tree.insertIter(17, "Seventeen");
    BinTree<int, std::string> tree2=tree.copyTree_Wrap();
    tree.deleteIter(5);
    tree.preOrder();
    tree2.preOrder();
    // TreeNode<int, std::string> *del=tree.deleteIter(10);
    // // std::cout<<(del->parent)<<(del->left==nullptr)<<(del->right==nullptr)<<std::endl;
    // delete del;
    // tree.deleteIter(5);
    // tree.deleteIter(15);
    // tree.deleteIter(3);
    // // tree.deleteIter(7);
    // tree.deleteIter(12);
    // tree.deleteIter(17);
    // tree.deleteIter(17);
    // // std::cout<<(del==nullptr)<<" "<<del->key<<" "<<del->val<<std::endl;
    // // std::cout<<(del->parent)<<(del->left==nullptr)<<(del->right==nullptr)<<std::endl;
    // // 前序遍历
    // std::cout << "Pre-order traversal: ";
    // tree.preOrder(); // 输出: 10 5 3 7 15 12 17

    // // 中序遍历
    // std::cout << "In-order traversal: ";
    // tree.inOrder(); // 输出: 3 5 7 10 12 15 17

    // // 搜索节点
    // int searchKey = 7;
    // TreeNode<int, std::string> *result = tree.search(searchKey);
    // if (result)
    // {
    //     std::cout << "Found: " << result->key << " with value: " << result->val << std::endl;
    // }
    // else
    // {
    //     std::cout << "Key " << searchKey << " not found." << std::endl;
    // }
    // tree.insertIter(19, "Seventeen");
    // tree.insertIter(15, "Seventeen");
    // tree.insertIter(13, "Seventeen");
    // tree.insertIter(12, "Seventeen");
    // std::cout << "Pre-order traversal: ";
    // tree.preOrder(); // 输出: 10 5 3 7 15 12 17
    // std::cout << "In-order traversal: ";
    // tree.inOrder(); // 输出: 3 5 7 10 12 15 17
    return 0;
}
*/