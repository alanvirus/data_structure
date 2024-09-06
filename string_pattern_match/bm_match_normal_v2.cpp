#include <iostream>
#include <vector>
#include "string.h"
using namespace std;
#define kMaxChar 256
//buildBC O(m+kMaxchar)
//buildSS O(m)
//buildGS O(m)
//match   O()
class bm_match_normal_v2
{
public:
    int* buildSS(char*P){
        int m= (int)strlen(P);
        int *ss=new int[m];
        ss[m-1]=m;
        int lo=m-1;
        int hi=m-1;
        for(int j=m-2;j>=0;j--){
            // if(lo<j){
            //     if(ss[m-1-hi+j]<j-lo){
            //         ss[j]=ss[m-1-hi+j];
            //     }else{
            //         hi=j;
            //         lo=lo;
            //         while(lo>=0 && P[lo]==P[m-1-hi+lo]){
            //             lo--;
            //         }
            //         ss[j]=hi-lo;
            //     }
            // }else{
            //     hi=j;
            //     lo=j;
            //     while(lo>=0 && P[lo]==P[m-1-hi+lo]){
            //         lo--;
            //     }
            //     ss[j]=hi-lo;
            // }
            //合并为下式：
            if(lo<j&&ss[m-1-hi+j]<j-lo){
                    ss[j]=ss[m-1-hi+j];
            }else{
                hi=j;
                lo=min(lo,j);
                while(lo>=0 && P[lo]==P[m-1-hi+lo]){
                    lo--;
                }
                ss[j]=hi-lo;
            }
        }
        return ss;
    } 
    int* buildGS_by_SS(char* P){ // GS[i]代表后缀(i,m-1]与其最近前缀（且前一个字符不同）的距离（不能是后缀本身GS[i]>0）,GS[-1]==GS[0]
        int* ss=buildSS(P);
        int m= (int)strlen(P);
        int* gs=new int[m];
        for(int j=0;j<m;j++){
            gs[j]=m;
        }
        int i=0;
        for(int j=m-2;j>=0;j--){//真前缀
            if(ss[j]==j+1){
                while(i<m-1-j){
                    gs[i++]=m-j-1;
                }
            }
        }
        for(int j=0;j<m-1;j++){//画家算法
            gs[m-1-ss[j]]=m-1-j;
        }
        delete[] ss;
        return gs;
    }
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
        if(m==0){
            return {};
        }
        int* bc=buildBC(P);
        int* GS=buildGS_by_SS(P);
        cout<<"--goodsuffix table--"<<endl;
        for(int i=0;i<m;i++){
            cout<<i<<" "<<GS[i]<<endl;
        }
        cout<<endl;

        vector<int> out;
        int cmp_lo=0;
        for (int i = 0; i < n-m + 1;)
        {
            int j=m-1;
            while(P[j] == T[i + j]){
                if(cmp_lo>--j)break;//这里的0可以改进，对于匹配失败的情况，总对比次数不超过O(n),但是如果每次均匹配成功有可能O(mn)，这时就需要利用Galil来降至O(n)//其实对于失败但匹配真前缀的情况也能优化，但没必要
            }
            if (j < cmp_lo)
            {
                out.push_back(i);
                i+=GS[0];
                cmp_lo=m-GS[0];
            }else{
                cmp_lo=0;
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
    char* T = (char*)"iceiceiceieeiceice_ceice";
    char* P = (char*)"iceice";
    bm_match_normal_v2 match;
    vector<int> out = match.match(T, P);
    for (int i = 0; i < out.size(); i++)
    {
        cout << out[i] << " ";
    }
    return 0;
}