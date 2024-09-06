//字符串（向量）去重，保留最靠左的
//按照书中方法，先全部set一遍，然后遍历bitmap表输出1的可以得到排好序的去重向量
#include "Bitmap_no_init.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
int main()
{
    int N = 100;
    char *L = new char[N];
    // 可打印ASCII字符[32,127)
    int min = 32;
    int max = 64;
    std::srand(std::time(nullptr));
    for (int i = 0; i < N; ++i)
    {
        L[i] = std::rand() % (max - min + 1) + min;
    }
    for (int i = 0; i < N; ++i)
    {
        std::cout << L[i];
    }
    std::cout << std::endl;
    char *newL = new char[N];
    int newN = 0;
    Bitmap bitmap(max - min + 1);
    for (int i = 0; i < N; ++i)
    {
        if (!bitmap.test(L[i] - min))
        {
            bitmap.set(L[i] - min);
            newL[newN++] = L[i];
        }
    }
    for (int i = 0; i < newN; ++i)
    {
        std::cout << newL[i];
    }
    std::cout << std::endl;
    return 0;
}