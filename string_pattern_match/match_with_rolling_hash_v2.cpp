// best O(n) worst O(mn) 滚动哈希 将哈希计算公式更改为s[0]*pow(101,len-1)+....+s[len-1]*pow(101,0),这样滚动的时候除法变乘法
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;
#define R 101//应当取字符集大小
#define M 1000000007//大素数，表示散列表长度，由于不实际存储散列表，可以取得很大   指纹对比所需时间取决于M
#define DIGIT(S,i) ((S)[i]-'0')
typedef HashCode HashCode;
class rolling_hash_v2
{
public:
    HashCode Dm;
    HashCode pow(int p, int len)
    {
        HashCode t = 1;
        for (int i = 0; i < len-1; i++)
            t = (t * p) % M;
        return t;
    }
    HashCode rolling_hash(string &T, int start_pos, int m)
    {
        HashCode hash = 0;
        for (int i = 0; i < m; i++)
        {
            hash = (hash * R + T[start_pos + i]) % M;
        }
        return hash;
    }
    HashCode updateHash(HashCode old_hash, string &T, int old_start_pos, int len)
    {
        old_hash = (old_hash -T[old_start_pos] * Dm) %M;
        old_hash = (( old_hash *R + T[old_start_pos + len]) % M +M)%M;//%运算结果与被除数正负号一致，如果想要得到非负结果，可用%构造mod运算
        //或者if(0>old_hash) old_hash+=M;
        // old_hash = ( (old_hash -T[old_start_pos] * Dm) *R + T[old_start_pos + len]) % M ;
        return old_hash;
    }
    bool check1by1(string &P, string &T, int k){
        for (int j = 0; j < P.size(); j++)
        {
            if (T[k + j] != P[j])
            {
                return false;
            }
        }
        return true;
    }
    vector<int> search(string &P,string &T)
    {
        int len_of_T = T.size();
        int len_of_P = P.size();
        assert(len_of_P <= len_of_T);
        if (len_of_P == 0)
        {
            return {};
        }
        Dm=pow(R,len_of_P);
        HashCode hash_of_T = rolling_hash(T, 0, len_of_P);
        HashCode hash_of_P = rolling_hash(P, 0, len_of_P);
        vector<int> out;
        for (int i = 0; i < len_of_T - len_of_P + 1; i++)
        {
            if (hash_of_T == hash_of_P)
            {
                if(check1by1(P,T,i)){
                    out.push_back(i);
                }
            }
            if (i < len_of_T - len_of_P) //如果是location问题，可以在i>n-m时也返回i，使用！(n<m+i)判断是否detection
                hash_of_T = updateHash(hash_of_T, T, i, len_of_P);
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
//     rolling_hash_v2 match;
//     vector<int> out = match.search(P, T);
//     for (int i = 0; i < out.size(); i++)
//     {
//         cout << out[i] << " ";
//     }
//     cout << endl;
//     return 0;
// }