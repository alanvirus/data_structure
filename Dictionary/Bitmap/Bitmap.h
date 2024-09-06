//时间复杂度：set/clear/test三个接口本身O(1)，调用的expand平摊O(1)
//空间复杂度: O(n)
//可以视为hash(x)=x,entry<int,bool>的完美哈希
#include<cstring>
#include <stdio.h>
class Bitmap{
    private:
        int N;
        char* M;
    protected:
        void init(int n) {
            M=new char[N=(n+7)/8];
            memset(M,0,N);
        }
    public:
        Bitmap(int n=8){
            init(n);
        }
        Bitmap(char* file,int n=8){
            init(n);
            FILE* fp = fopen(file, "r");
            fread(M, sizeof(char), N, fp);
            fclose(fp);
        }
        ~Bitmap(){
            delete[] M;
            M=nullptr;
        }
        void set(int i){
            expand(i);
            M[i>>3]|=(0x80>>(i&0x07));
        }
        void clear(int i){
            expand(i);
            M[i>>3]&=~(0x80>>(i&0x07));
        }
        bool test(int i){
            expand(i);
            return M[i>>3]&(0x80>>(i&0x07));
        } 
        void dump(char* file){
            FILE* fp = fopen(file, "w");
            fwrite(M, sizeof(char), N, fp);
            fclose(fp);
        }
        void expand(int k){//加倍扩容
            if(k>=8*N){
                int oldN=N;
                char* oldM=M;
                init(2*k);
                memcpy_s(M,N,oldM,oldN);
                delete[] oldM;
            }
        }
};