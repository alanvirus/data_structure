#include <iostream>
#include <vector>
using namespace std;
#define kMaxChar 256
class kmp_match
{
public:
    vector<int> next;
    void create_next(string &P)
    {
        next = vector<int>(P.size()); //[i]表示前i+1个元素构成的子串的最长公共前缀后缀长度
        next[0] = -1;
        int k = next[0];
        for (int i = 1; i < P.size(); i++)
        {
            while (k != -1 && P[k + 1] != P[i])
            {
                k = next[k];
            }
            if (P[k + 1] == P[i])
            {
                k++;
            }
            next[i] = k;
        }
    }
    vector<int> match(string &T, string &P)
    {
        int len_of_T = T.size();
        int len_of_P = P.size();
        if (len_of_P == 0)
        {
            return {};
        }
        create_next(P);
        vector<int> out;
        int j=0;
        for (int i = 0; i < len_of_T;)
        {
            while(j<len_of_P&&P[j]==T[i]){
                j++;
                i++;
            }
            if(j==len_of_P){
                out.push_back(i-len_of_P);
            }
            if(j==0){
                i++;
            }else{
                j=next[j-1]+1;
            }
        }
        // for (int i = 0; i < out.size(); i++)
        // {
        //     cout << out[i] << " ";
        // }
        // cout << endl;
        return out;
    }
};
// int main()
// {
//     string T = "sssdfsdfdsfdabcabaabcsdfsaaaaaaaaaaaa";
//     string P = "baabc";
//     kmp_match match;
//     vector<int> out = match.search(T, P);
//     for (int i = 0; i < out.size(); i++)
//     {
//         cout << out[i] << " ";
//     }
//     return 0;
// }