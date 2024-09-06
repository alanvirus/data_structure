#include <iostream>
#include <vector>
#include "string.h"
using namespace std;
#define kMaxChar 256
class bm_match_normal
{
public:
    int* buildBC(char*P) //O(m+kMaxchar)
    {
        int* bc = new int[kMaxChar];
        for(int i=0;i<kMaxChar;i++){
            bc[i]=-1;
        }
        for (int i = 0; i < (int)strlen(P); i++)
        {
            bc[P[i]] = i;
        }
        return bc;
    }
    int* buildGS(char* P){ //看起来GS可以采用与next相同的改进办法，如果P[j]与前缀的前一个字符相等，则应向前寻找
        int m= (int)strlen(P);
        int *GS=new int[m+1];//GS[i]代表后缀(i,m-1]与其最近前缀的距离（不能是后缀本身GS[i]>0）,GS[-1]放在GS[m]处
        GS[m-1]=1;
        int i=m-2;
        int j=m-2;
        while(i>=0){//进入循环表示要确定GS[i],
            if(j<0||P[j]==P[i+1]){
                GS[i]=i+1-j;
                i--;j--;
            }else{
                while(1){
                    j--;
                    int k;
                    for(k=max(0,-j);k<m-1-i;k++){
                        if(P[j+k]!=P[i+1+k])
                            break;
                    }
                    if(k==m-1-i)
                        break;
                }
            }
        }
        GS[m]=i+1-j;
        return GS;
    }
    vector<int> match_bruteforce(char* P, char* T)
    {
        size_t n=strlen(T),i;
        size_t m=strlen(P),j;
        if(m==0)
            return {};
        vector<int> out;
        for(i=0;i<n-m+1;i++){
            for(j=0;j<m;j++){
                if(P[j]!=T[i+j])
                    break;
            }
            if(j==m){
                out.push_back(i);
            }
        }
        return out;
    }
    vector<int> match(char*T, char*P)
    {
        int n=(int)strlen(T);
        int m=(int)strlen(P);
        int* bc=buildBC(P);
        int* GS=buildGS(P);
        cout<<"--goodsuffix table--"<<endl;
        for(int i=0;i<m+1;i++){
            cout<<i<<" "<<GS[i]<<endl;
        }
        cout<<endl;
        vector<int> out;
        for (int i = 0; i < n-m + 1;)
        {
            int j=m-1;
            while(P[j] == T[i + j]){
                if(0>--j)break;//这里的0可以改进，否则aaaaaaaaaa与aaa匹配会退化为nm
            }
            if (j < 0)
            {
                out.push_back(i);
                i+=GS[m];
            }else{
                i += max(j - bc[T[i + j]], GS[j]);
            }
        }
        delete[] bc;
        delete[] GS;
        return out;
    }
};
int main()
{
    char* T = (char*)"baabaabaa";
    char* P = (char*)"baabaa";
    bm_match_normal match;
    vector<int> out = match.match(T, P);
    for (int i = 0; i < out.size(); i++)
    {
        cout << out[i] << " ";
    }
    return 0;
}