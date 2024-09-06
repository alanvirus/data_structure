//时间复杂度：set/clear/test三个接口本身O(1)，调用的expand平摊O(1)
//空间复杂度: O(n)，但是单元类型变为了Rank，且Bitmap的最大规模无法超过Rank的表示范围，比如Rank为四字节整数，那么Bitmap规模在2^31-1内（注意负数用来标记被删校验环）
//使用校验环省略将bitmap初始化为0的步骤
#include<cstring>
#include <stdio.h>
typedef int Rank;
class Bitmap{
    private:
        Rank N;
        Rank* F;
        Rank* T;//栈
        Rank top;
    protected:
        inline bool valid(Rank r){
            return (0<=r) && (r<top);
        }
        inline bool erased(Rank r){
            return valid(F[r])&&(T[F[r]]+1+r==0);
        }
    public:
        Bitmap(int n=8){
            F=new Rank[N=n];
            T=new Rank[N];
            top=0;
        }
        ~Bitmap(){
            delete[] F;
            F=nullptr;
            delete[] T;
            T=nullptr;
        }
        // void set(int i){
        //     if(test(i))return;
        //     F[i]=top++;
        //     T[F[i]]=i;
        // }
        inline void set(int i){
            expand(i);
            if(test(i))return;
            if(!erased(i))F[i]=top++;
            T[F[i]]=i;
        }
        //clear实现，仅将T[F[i]]置为-1，其余不变，那么T的长度无法控制在n内，而是会正比于操作次数
        //clear后仍保留i信息，让T[F[i]]与i绑定，实现T与F双射
        //感觉还可以clear后置为-1，新set时检查如果位置为-1就使用，这样T[F[i]]不与i绑定，新的i也能抢过来
        inline void clear(int i){
            expand(i);
            if(test(i)){
                T[F[i]]=-1-i;
            }
        }
        inline bool test(int i){
            expand(i);
            return valid(F[i])&&(T[F[i]]==i);
        } 
        void expand(int i){//加倍扩容
            if(i>=N){
                int oldN=N;
                Rank* oldF=F;
                Rank* oldT=T;
                F=new Rank[N=2*i];
                T=new Rank[N];
                memcpy_s(F,N*sizeof(Rank),oldF,oldN*sizeof(Rank));
                memcpy_s(T,N*sizeof(Rank),oldT,top*sizeof(Rank));
                delete[] oldF;
                delete[] oldT;
            }
        }
};