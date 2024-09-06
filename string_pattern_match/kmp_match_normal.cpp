#include <iostream>
#include <vector>
#include "string.h"
using namespace std;
class kmp_match_normal
{
public:
    int* buildNext(char*P)//O(m)-O(m)
    {
        size_t m=strlen(P),j=0;
        int * N=new int[m+1];
        int t = N[0] = -1;
        while(j<m){
            if(t<0 || P[j]==P[t]){
                t++;j++;
                N[j]=((j<m&&P[t]==P[j])?N[t]:t);
            }else{
                t=N[t];
            }
        }
        return N;
    }
    vector<int> match_bruteforce(char* P, char* T)
    {
        size_t n=strlen(T),i=0;
        size_t m=strlen(P),j=0;
        if(m==0)
            return {};
        vector<int> out;
        while(i<n){
            if(T[i]==P[j]){
                i++;j++;
                if(j==m){
                    out.push_back(i-m);
                    i=i-j+1;j=0;
                }
            }
            else{
                i=i-j+1;j=0;
            }
        }
        return out;
    }
    vector<int> match(char* P, char* T)//O(n)-O(n)
    {
        int*next =buildNext(P);

        int n=(int)strlen(T),i=0;
        int m=(int)strlen(P),j=0;
        cout<<"--next table--"<<endl;
        for(int i=0;i<m+1;i++){
            cout<<i<<" "<<next[i]<<endl;
        }
        cout<<endl;

        if(m==0)
            return {};
        vector<int> out;
        while(i<n){
            if(j<0 || T[i]==P[j]){
                i++;j++;
                if(j==m){
                    out.push_back(i-m);
                    j=next[j];
                }
            }
            else{
                j=next[j];
            }
        }
        delete[] next;
        // for (int i = 0; i < out.size(); i++)
        // {
        //     cout << out[i] << " ";
        // }
        // cout << endl;
        return out;
    }
};
int main()
{
    char* T = (char*)"0001000010";
    char* P = (char*)"000010";
    kmp_match_normal match;
    vector<int> out = match.match(P, T);
    for (int i = 0; i < out.size(); i++)
    {
        cout << out[i] << " ";
    }
    return 0;
}