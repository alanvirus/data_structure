#include "Bitmap.h"
#include <iostream>
#include <cmath>
//如果不做任何优化，可以分析外循环是O(N)的
//由素数定理，内循环最多n/lnn次，耗时小于
//n/2+n/3+n/5+...n/(n/lnn)=O(nlogn)
//这里改进后好像没有渐进意义?
void Eratosthenes(int N,char* file)
{
    int sqrt_N = sqrt(N)+1;
    Bitmap bitmap(N);
    bitmap.set(0);
    bitmap.set(1);
    int p;
    for (int i = 2; i <= sqrt_N; ++i)//筛到大于根号N的第一个数就可以结束了
    {
        if (!bitmap.test(i))
        {
            p = i * i;//对于素数i，从i*i开始筛 46340^2还在int表示范围内，46341^2就超了
            while (p <= N)
            {
                bitmap.set(p);
                p+=i;//之前采用p=i*j;j++;加法变乘法更慢了
            }
        }
    }
    bitmap.dump(file);
}
int main(){
    int N=98;
    char* file=(char*)"prime";
    Eratosthenes(N,file);
    Bitmap b(file,N);
    for(int i=0;i<N;i++){
        if(!b.test(i)){
            std::cout<<i<<" ";
        }
    }
    return 0;
}