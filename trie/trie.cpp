// 存储字符串集S的数据结构,中文名为键树或字典树
//get O(h) put/remove O(hr)
//最坏情况下需要多达big-omega(nr)空间，S中字符串互不为前缀，每个串都对应于一个叶节点，光叶节点就nr了
//若trieNode改用列表实现，则空间正比于S中所有字符串的总长度，但是get会变成O(hr)
//PATRICIA树，将单分支节点合成大节点，渐进无意义
#include "string.h"
#include <iostream>
// #include<string>
using namespace std;
struct trieNode
{
    int r; // r表示字符集sigma的大小
    trieNode **children;
    bool *belong_to_S;
    trieNode(int r) : r(r)
    {
        children = new trieNode *[r];
        memset(children, 0, r * sizeof(trieNode *));
        belong_to_S = new bool[r];
        for (int i = 0; i < r; ++i)
        {
            belong_to_S[i] = false;
        }
    }
    ~trieNode()
    {
        for (int i = 0; i < r; ++i)
        {
            if (children[i])
            {
                delete children[i];
                children[i] = nullptr;
            }
        }
        delete[] children;
        delete[] belong_to_S;
    }
};
class trie // 不支持存空串的trie
{
public:
    int r; // r表示字符集sigma的大小
    int size;
    trieNode *root;
    trie(int r = 26) : r(r)
    {
        size = 0;
        root = new trieNode(r);
    }
    trie( string *S, int S_size,int r = 26) : r(r)
    {
        size = 0;
        root = new trieNode(r);
        for (int i = 0; i < S_size; i++)
        {
            insert(S[i]);
        }
    }
    ~trie()
    {
        if (root)
        {
            delete root;
            root = nullptr;
        }
    }
    int hash_char_to_int(char c)
    {
        return c - 'a';
    }
    bool insert(string word)
    {
        trieNode *cur = root;
        for (int i = 0; i < word.length()-1; i++)
        {
            if(!cur->children[hash_char_to_int(word[i])]){
                cur->children[hash_char_to_int(word[i])]=new trieNode(r);
            }
            cur=cur->children[hash_char_to_int(word[i])];
        }
        if (cur->belong_to_S[hash_char_to_int(word[word.length() - 1])])
        { // 这里为了更通用，不减'a'而是采用转换函数
            return false;
        }
        cur->belong_to_S[hash_char_to_int(word[word.length() - 1])] = true;
        size++;
        return true;
    }
    bool find(string word)
    {
        trieNode *cur = root;
        for (int i = 0; i < word.length()-1; i++)
        {
            if(!cur->children[hash_char_to_int(word[i])]){
                return false;
            }
            cur=cur->children[hash_char_to_int(word[i])];
        }
        if (cur->belong_to_S[hash_char_to_int(word[word.length() - 1])])
        { // 这里为了更通用，不减'a'而是采用转换函数
            return true;
        }
        return false;
    }
    bool remove(string word)
    {
        trieNode *cur = root;
        for (int i = 0; i < word.length()-1; i++)
        {
            if(!cur->children[hash_char_to_int(word[i])]){
                return false;
            }
            cur=cur->children[hash_char_to_int(word[i])];
        }
        if (cur->belong_to_S[hash_char_to_int(word[word.length() - 1])])
        { // 这里为了更通用，不减'a'而是采用转换函数
            cur->belong_to_S[hash_char_to_int(word[word.length() - 1])] = false;
            size--;
            //或许应该存一下该Node的父Node指针，本身存储的string数和它的子trie们存储的string数，在清零后删除该Node，并向上检查父Node
            return true;
        }
        return false;
    }
};
void test_trie()
{
    std::string words[6] = {"apple", "app", "apricot", "banana", "berry", "bana"};
    trie t(words, 6,26);
    std::cout<<t.size<<std::endl;
    // 测试插入
    std::string insertWords[] = {"apricot",  "orange"};
    std::cout << "Inserting words:" << std::endl;
    for (int i=0;i<2;i++)
    {
        std::cout << "Inserting \"" << insertWords[i] << "\": " << (t.insert(insertWords[i]) ? "Success" : "Already exists") << std::endl;
    }
    // 测试查找
    std::cout << "\nFinding words:" << std::endl;
    std::string testWords[7] = {"apple", "app", "apricot", "banana", "berry", "orange", "not exist"};
    for (const auto &word : testWords)
    {
        std::cout << "Finding \"" << word << "\": " << (t.find(word) ? "Found" : "Not found") << std::endl;
    }

    // 测试移除
    std::cout << "\nRemoving words:" << std::endl;
    for (const auto &word : testWords)
    {
        std::cout << "Removing \"" << word << "\": " << (t.remove(word) ? "Removed" : "Not found") << std::endl;
    }

    // 再次测试查找，验证移除是否成功
    std::cout << "\nFinding removed words:" << std::endl;
    for (const auto &word : testWords)
    {
        std::cout << "Finding \"" << word << "\": " << (t.find(word) ? "Found" : "Not found") << std::endl;
    }
}