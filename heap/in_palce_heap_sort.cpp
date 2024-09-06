#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <algorithm>
//in_place heap sort 优势在于空间复杂度，不需要额外辅助空间，时间复杂度为理论最优值O(nlogn),但是和默认sort相比可见常系数不是最优
using namespace std;
#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) ((2 * (i)) + 1)
#define RIGHT(i) ((2 * (i)) + 2)
template <typename T>
class MinHeap//based on vector
{
    vector<T> *harr;
    int size;
public:
    void traverse()
    {
        for (int i = 0; i < size; i++)
            cout << (*harr)[i] << " ";
        cout << endl;
    }
    MinHeap(vector<T> &vec)//vec不pop_back,每次del时swap(0,size-1),n次del后实现就地排序
    { // 不占用额外空间，除了key不是val时需要储存key
        harr = &vec;
        size=harr->size();
        Floyd();
    }
    // ~MinHeap()//排完之后可不能删了vec内容
    // {
    //     delete harr;
    // }
    // void clear()
    // {
    //     harr->clear();
    // }
    T delMin()
    {
        if(size<=0){
            cout << "no elem"<< endl;
            return (T)NULL;
        }
        T elem=(*harr)[0];
        (*harr)[0] = (*harr)[size-1]; 
        (*harr)[size-1]=elem;
        size-=1;
        // harr->pop_back(); 
        MinHeapify(0); //向下调整,即便此时空了也能用
        return elem;
    }
    void Floyd(){
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
            if (l < size && (*harr)[l] > (*harr)[i]) //实为maxheap
                smallest = l;
            if (r < size && (*harr)[r] > (*harr)[smallest])
                smallest = r;
            if (smallest == i)
                break;
            swap((*harr)[i], (*harr)[smallest]);
            i = smallest;
        }
    }
};
template <typename T>
void heapSort(vector<T> &vec)
{
    MinHeap<T> heap(vec);
    for (int i = 0; i < vec.size()-1; i++)
    {
        heap.delMin();
    }
}
int main() {
    ofstream results("sort_times.csv");
    results << "size,heapSort,std::sort\n";
    int i=0;
    for (int n = 10000; n <= 5000000; n *= 2) {
        i++;
        vector<int> vec1(n);
        srand(time(0));
        for (int &val : vec1)
            val = rand();
        vector<int> vec2 = vec1;

        auto start = chrono::high_resolution_clock::now();
        heapSort(vec1);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;
        double heapSortTime = duration.count();

        auto start2 = chrono::high_resolution_clock::now();
        sort(vec2.begin(), vec2.end());
        auto end2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration2 = end2 - start2;
        double sortTime = duration2.count();

        results << i << "," << heapSortTime << "," << sortTime << "\n";
    }

    results.close();
    cout << "Results written to sort_times.csv" << endl;

    return 0;
}