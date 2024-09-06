#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>
using namespace std;
#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) ((2 * (i)) + 1)
#define RIGHT(i) ((2 * (i)) + 2)
template <typename T>
class MinHeap//based on vector
{
    vector<T> *harr;
public:
    void traverse()
    {
        for (int i = 0; i < harr->size(); i++)
            cout << (*harr)[i] << " ";
        cout << endl;
    }
    // Constructor1
    MinHeap()
    {
        harr = new vector<T>();
    }
    // Constructor2
    MinHeap(vector<T> &vec)//vec不pop_back,每次del时swap(0,size-1),n次del后实现就地排序
    { // 不占用额外空间，除了key不是val时需要储存key
        harr = &vec;
        Floyd();
    }
    ~MinHeap()
    {
        delete harr;
    }
    void clear()
    {
        harr->clear();
    }
    void insertKey(T k)
    {
        harr->push_back(k);
        UpHeapify(harr->size() - 1);
    }
    void insertKey(vector<T> &vec)
    {
        for (const T &elem : vec)
            harr->push_back(elem);
        Floyd();
    }
    void merge(MinHeap<T> &other) //O(m+n) //premature optimization!
    { // 使用方法为a.merge(b),这样会清空b，保留a
        for (const T &elem : *other.harr)
            harr->push_back(elem);
        other.clear();
        Floyd();
    }
    void merge2(MinHeap<T> &other) //O(logn+...+log(n+m)) //According to Jim Mischel,normally the average case of heap insertion is O(1),although the constant is somewhat higher than for heapify 
    { // 使用方法为a.merge(b),这样会清空b，保留a
        for (const T &elem : *other.harr)
            insertKey(elem);
        other.clear();
    }
    T getMin()
    {
        if (harr->size() <= 0)
        {
            cout << "no elem left" << endl;
            return (T)NULL;
        }
        return (*harr)[0];
    }
    T delMin()
    {
        return deleteKey(0);
    }
    T deleteKey(int i){
        if(i>=harr->size()){
            cout << "no elem at "<<i<< endl;
            return (T)NULL;
        }
        T elem=(*harr)[i];
        (*harr)[i] = (*harr)[harr->size()-1]; 
        harr->pop_back(); 
        MinHeapify(i); //向下调整,即便此时空了也能用
        return elem;
    }
    //     void MinHeap::deleteKey(int i) //一种稍微低效的实现方式，效果与上面完全相同，可证明
    // { 
    //     decreaseKey(i, INT_MIN); 
    //     extractMin(); 
    // } 
    void changeKey(int i, T new_val){
        if(i>=harr->size()){
            cout << "no elem at "<<i<< endl;
        }
        (*harr)[i] = new_val; 
        UpHeapify(i);
        MinHeapify(i);
    }
    void Floyd(){
        int size = harr->size();
        if (size <= 1)
            return;
        for (int i = PARENT(size - 1); i >= 0; i--)
        {
            MinHeapify(i);
        }
    }
    void MinHeapify(int i)
    { // 下溢,该函数支持i>=size的情况
        while (1)
        {
            int smallest = i;
            int l = LEFT(i);
            int r = RIGHT(i);
            if (l < harr->size() && (*harr)[l] < (*harr)[i])
                smallest = l;
            if (r < harr->size() && (*harr)[r] < (*harr)[smallest])
                smallest = r;
            if (smallest == i)
                break;
            swap((*harr)[i], (*harr)[smallest]);
            i = smallest;
        }
    }
    void UpHeapify(int i)
    { // 上溢
        while (i != 0 && (*harr)[PARENT(i)] > (*harr)[i])
        {
            swap((*harr)[i], (*harr)[PARENT(i)]);
            i = PARENT(i);
        }
    }
};
int main() {
    // Test 1: Basic operations
    cout << "Test 1: Basic operations" << endl;
    MinHeap<int> heap1;
    heap1.insertKey(10);
    heap1.insertKey(20);
    heap1.insertKey(5);
    heap1.insertKey(30);
    heap1.insertKey(3);
    heap1.insertKey(3);
    cout << "Heap after insertions: ";
    heap1.traverse();  // Expected: 3 5 10 30 20

    cout << "Minimum element: " << heap1.delMin() << endl;  // Expected: 3
    
    cout << "Heap after deleting min: ";
    heap1.traverse();  // Expected: 5 10 20 30

    heap1.changeKey(1, 2);
    cout << "Heap after changing key at index 1 to 2: ";
    heap1.traverse();  // Expected: 2 5 20 30

    // Test 2: Merging heaps
    cout << "Test 2: Merging heaps" << endl;
    MinHeap<int> heap2;
    heap2.insertKey(15);
    heap2.insertKey(8);
    heap2.insertKey(25);
    
    cout << "Heap2 before merge: ";
    heap2.traverse();  // Expected: 8 15 25

    heap1.merge(heap2);
    cout << "Heap1 after merging with Heap2: ";
    heap1.traverse();  // Expected: 2 5 20 30 8 15 25

    cout << "Heap2 after merge (should be empty): ";
    heap2.traverse();  // Expected: (empty)

    // Test 3: Insert from vector and merge with another heap
    cout << "Test 3: Insert from vector and merge with another heap" << endl;
    vector<int> vec = {4, 3, 5, 4};
    MinHeap<int> heap3(vec);
    
    cout << "Heap3 after creation from vector: ";
    heap3.traverse();  // Expected: 35 40 50 45

    heap1.merge2(heap3);
    cout << "Heap1 after merging with Heap3: ";
    heap1.traverse();  // Expected: 2 5 20 30 8 15 25 35 40 50 45

    cout << "Heap3 after merge2 (should be empty): ";
    heap3.traverse();  // Expected: (empty)

    return 0;
}