#include<iostream>
#include"string.h"
#include"../Bitmap/Bitmap.h"
//该实现有问题，对于位置p，如果有两种分解且一种小于另一种，更大的一种未必能直接舍去
void FermatLagrange(int N){
    int*a1=new int[N];//记录四个整数中的最大值
    int*a2=new int[N];
    int*a3=new int[N];
    int*a4=new int[N];
    int*sum=new int[N];
    Bitmap b(N);
    int i=0;
    int i_square=0;
    while(i_square<N){
        a1[i_square]=i;a2[i_square]=a3[i_square]=a4[i_square]=0;
        sum[i_square]=a1[i_square]+a2[i_square]+a3[i_square]+a4[i_square];
        b.set(i_square);
        i_square+=(2*i+1);
        i++;
    }
    for(int i=0;i<N;i++){
        if(a4[i]){
            continue;
        }
        if(a3[i]){
            for(int j=1;j<=a3[i];j++){
                int p=i+j*j;
                if(p>=N)break;
                if(!b.test(p) || sum[p]>sum[i]+j){//这里先假设不会出现sum[i]和i同时相等的情况
                    a1[p]=a1[i];a2[p]=a2[i];a3[p]=a3[i];a4[p]=j;sum[p]=sum[i]+j;
                    b.set(p);
                }
            }
            continue;
        }
        if(a2[i]){
            for(int j=1;j<=a2[i];j++){
                int p=i+j*j;
                if(p>=N)break;
                if(!b.test(p) || sum[p]>sum[i]+j){//这里先假设不会出现sum[i]和i同时相等的情况
                    a1[p]=a1[i];a2[p]=a2[i];a3[p]=j;a4[p]=0;sum[p]=sum[i]+j;
                    b.set(p);
                }
            }
            continue;
        }
        if(a1[i]){
            for(int j=1;j<=a1[i];j++){
                int p=i+j*j;
                if(p>=N)break;
                if(!b.test(p) || sum[p]>sum[i]+j ){//会出现sum[i]和i同时相等的情况 9330/7710 若非零部分等长，取最低非零位更大的 ；若不等长，取0更多的
                    a1[p]=a1[i];a2[p]=j;a3[p]=0;a4[p]=0;sum[p]=sum[i]+j;
                    b.set(p);
                }
            }
            continue;
        }
    }
    for(int i=0;i<N;i++){
        std::cout<<a1[i]<<" "<<a2[i]<<" "<<a3[i]<<" "<<a4[i]<<std::endl;
    }
}
int main(){
    int N=101;
    FermatLagrange(N+1);
    return 0;
}