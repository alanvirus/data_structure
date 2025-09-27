#include <iostream>
using namespace std;

// 节点结构体
struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// 右旋操作
Node* rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

// 左旋操作
Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// 将 key 对应的节点旋转到根
Node* splay(Node* root, int key) {
    if (!root || root->key == key)
        return root;

    // Key 在左子树
    if (key < root->key) {
        if (!root->left)
            return root;

        // Zig-Zig (左-左)
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (左-右)
        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right)
                root->left = leftRotate(root->left);
        }

        return (root->left == nullptr) ? root : rightRotate(root);
    } 
    // Key 在右子树
    else {
        if (!root->right)
            return root;

        // Zag-Zig (右-左)
        if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left)
                root->right = rightRotate(root->right);
        }
        // Zag-Zag (右-右)
        else if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        return (root->right == nullptr) ? root : leftRotate(root);
    }
}

// 插入操作
Node* insert(Node* root, int key) {
    if (!root)
        return new Node(key);

    root = splay(root, key);

    if (root->key == key)
        return root;

    Node* newNode = new Node(key);

    if (key < root->key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = nullptr;
    } else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = nullptr;
    }

    return newNode;
}

// 删除操作
Node* deleteKey(Node* root, int key) {
    if (!root)
        return nullptr;

    root = splay(root, key);

    if (root->key != key)
        return root;

    Node* temp;
    if (!root->left) {
        temp = root->right;
    } else {
        temp = splay(root->left, key);
        temp->right = root->right;
    }

    delete root;
    return temp;
}

// 查找操作
bool search(Node*& root, int key) {
    root = splay(root, key);
    return root && root->key == key;
}

// 中序遍历
void inorder(Node* root) {
    if (root) {
        inorder(root->left);
        cout << root->key << " ";
        inorder(root->right);
    }
}

// 主函数
int main() {
    Node* root = nullptr;

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);

    cout << "中序遍历: ";
    inorder(root);
    cout << endl;

    root = deleteKey(root, 20);
    cout << "删除20后: ";
    inorder(root);
    cout << endl;

    if (search(root, 30)) {
        cout << "找到30" << endl;
    } else {
        cout << "未找到30" << endl;
    }

    return 0;
}
