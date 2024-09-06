#include <iostream>
#include <vector>
using namespace std;
#define kMaxChar 256
class bm_match
{
public:
    vector<int> bc;
    vector<int> surfix;
    vector<bool> prefix;
    void create_bc_vector(string &P)
    {
        bc = vector<int>(kMaxChar);
        fill(bc.begin(), bc.end(), -1);
        for (int i = 0; i < P.size(); i++)
        {
            bc[P[i]] = i;
        }
    }
    void create_surfix_and_prefix(string &P)
    {
        surfix = vector<int>(P.size());
        prefix = vector<bool>(P.size());
        fill(surfix.begin(), surfix.end(), -1);
        fill(prefix.begin(), prefix.end(), false);
        int len_of_P = P.size();
        int i; // 当前检查prefix的尾字符在P中的位置
        for (i = 0; i < len_of_P - 1; i++)
        {
            int j = i;
            int k = 0; // 当前检查prefix长度
            while (j >= 0 && P[j] == P[len_of_P - 1 - k])
            {
                j--;
                k++;
                surfix[k] = j + 1;
            }
            if (j == -1)
            {
                prefix[k] = true;
            }
        }
    }
    vector<int> search(string &T, string &P)
    {
        int len_of_T = T.size();
        int len_of_P = P.size();
        if (len_of_P == 0)
        {
            return {};
        }
        create_bc_vector(P);
        create_surfix_and_prefix(P);
        vector<int> out;
        for (int i = 0; i < len_of_T - len_of_P + 1;)
        {
            int j;
            for (j = len_of_P - 1; j >= 0; j--)
            {
                if (P[j] != T[i + j])
                {
                    int stride_by_bc = j - bc[T[i + j]];
                    int stride_by_prefix = 0;
                    int surfix_nearest_pos = surfix[len_of_P - 1 - j];
                    if (j < len_of_P - 1) // 存在好后缀
                    {
                        stride_by_prefix = j + 1 - surfix_nearest_pos;
                        if (surfix_nearest_pos == -1)
                        {
                            int len_of_prefix = len_of_P - 2 - j;
                            while (len_of_prefix > 0 && !prefix[len_of_prefix])
                            {
                                len_of_prefix--;
                            }
                            stride_by_prefix = len_of_P - len_of_prefix;
                        }
                    }
                    i += max(stride_by_bc, stride_by_prefix);
                    break;
                }
            }
            if (j < 0)
            {
                out.push_back(i);
                int len_of_prefix = len_of_P - 1;
                while (len_of_prefix > 0 && !prefix[len_of_prefix])
                {
                    len_of_prefix--;
                }
                i+=len_of_P-len_of_prefix;
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
//     bm_match match;
//     vector<int> out = match.search(T, P);
//     for (int i = 0; i < out.size(); i++)
//     {
//         cout << out[i] << " ";
//     }
//     cout << endl
//          << "end" << endl;
//     return 0;
// }