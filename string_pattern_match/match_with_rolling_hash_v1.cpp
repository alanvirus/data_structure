// best O(n) worst O(mn) 滚动哈希 该版本在P较长时错误，请使用v2版本
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
using namespace std;
#define prime 101
#define Q 1000000007
// long long rolling_hash(string &T, int start_pos, int len)
// {
//     long long hash = 0;
//     for (int i = 0; i < len; i++)
//     {
//         hash += T[start_pos + i] * pow(prime, i);
//     }
//     return hash;
// }
class rolling_hash_v1
{
public:
    long long pow_prime_len;
    long long pow(int p, int len)
    {
        long long t = 1;
        for (int i = 0; i < len - 1; i++)
            t = (t * p) % Q;
        return t;
    }
    long long rolling_hash(string &T, int start_pos, int len)
    {
        long long hash = 0;
        for (int i = 0; i < len; i++)
        {
            hash = hash * prime + T[start_pos + len - i - 1];
            hash %= Q;
        }
        return hash;
    }
    long long roll_hash_to_next(string &T, int old_start_pos, int old_hash, int len)
    {
        old_hash = (( (old_hash -T[old_start_pos]) /prime + T[old_start_pos + len]* pow_prime_len) % Q +Q)%Q;//除法与模不兼容
        return old_hash;
    }
    vector<int> search(string &T, string &P)
    {
        int len_of_T = T.size();
        int len_of_P = P.size();
        assert(len_of_P <= len_of_T);
        if (len_of_P == 0)
        {
            return {};
        }
        pow_prime_len=pow(prime,len_of_P);
        long long hash_of_T = rolling_hash(T, 0, len_of_P);
        long long hash_of_P = rolling_hash(P, 0, len_of_P);
        vector<int> out;
        for (int i = 0; i < len_of_T - len_of_P + 1; i++)
        {
            int match = 0;
            if (hash_of_T == hash_of_P)
            {
                for (int j = 0; j < len_of_P; j++)
                {
                    if (T[i + j] != P[j])
                    {
                        break;
                    }
                    if (j == len_of_P - 1)
                    {
                        match = 1;
                    }
                }
            }
            if (match)
            {
                out.push_back(i);
            }
            if (i < len_of_T - len_of_P)
                hash_of_T = roll_hash_to_next(T, i, hash_of_T, len_of_P);
        }
        for (int i = 0; i < out.size(); i++)
        {
            cout << out[i] << " ";
        }
        cout << endl;
        return out;
    }
};
int main()
{
    string T="ssaaaaa";
    string P="aaaaa";
    rolling_hash_v1 hash;
    vector<int> out = hash.search(T, P);
    for (int i = 0; i < out.size(); i++)
    {
        cout << out[i] << " ";
    }
    cout << endl;
    return 0;
}