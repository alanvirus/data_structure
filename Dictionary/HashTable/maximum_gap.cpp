//n个无序点形成的n-1个线段最大长度
//找到全局hi和lo，则max_gap>=(hi-lo)/(n-1),
//采用hash(x)=(n-1)*(x-lo)/(hi-lo),则最大间隙的左右端点位于不同的bucket
//构造ht，每个桶存局部hi和lo，最后遍历找最大值
//如果为浮点数组有没有可能出现最大间隙的两个端点本来不在一个桶，但是因为计算精度问题被分到一个桶的情况?  或许可以仍选取整数端点建桶，浮点数向下取整后hash，只需保证桶长小于(hi-lo)/(n-1)，如果(hi-lo)/(n-1)<=1,就放大
#include<iostream>
#include "../Bitmap/Bitmap.h"
int maximum_gap(int* x,int n){
    int min=x[0];
    int max=x[0];
    for(int i=1;i<n;i++){
        if(x[i]>max)max=x[i];
        if(x[i]<min)min=x[i];
    }
    int *hi=new int[n];
    int *lo=new int[n];
    Bitmap b(n);
    for(int i=0;i<n;i++){
        int p=(n-1)*(x[i]-min)/(max-min);
        if(b.test(p)){
            if(x[i]>hi[p])hi[p]=x[i];
            if(x[i]<lo[p])lo[p]=x[i];
        }else{
            b.set(p);
            hi[p]=lo[p]=x[i];
        }
    }
    int last_hi=hi[0];
    int max_gap=0;
    for(int i=1;i<n;i++){
        if(b.test(i)){
            if(lo[i]-last_hi>max_gap)max_gap=lo[i]-last_hi;
            last_hi=hi[i];
        }
    }
    return max_gap;
}
int main(){
    int arr[] = { 3, 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,9, 15,1, 20,-3};
    int length = sizeof(arr) / sizeof(arr[0]);

    std::cout << maximum_gap(arr,length) << std::endl;

    return 0;
}