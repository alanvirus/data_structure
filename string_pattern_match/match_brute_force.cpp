// worst O(mn)暴力匹配
#include <iostream>
#include <vector>
using namespace std;
class brute_force_match
{
public:
    vector<int> search(string &T, string &P)
    {
        int len_of_T = T.size();
        int len_of_P = P.size();
        if (len_of_P == 0)
        {
            return {};
        }
        vector<int> out;
        for (int i = 0; i < len_of_T - len_of_P + 1; i++)
        {
            int match = 1;
            for (int j = 0; j < len_of_P; j++)
            {
                if (T[i + j] != P[j])
                {
                    match = 0;
                    break;
                }
            }
            if (match)
            {
                out.push_back(i);
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
//     string T = "asdfsdfsdasddfdsasdas";
//     string P = "asd";
//     vector<int> out= search_brute_force(T, P);
//     for(int i=0;i<out.size();i++){
//         cout<<out[i]<<" ";
//     }
//     cout<<endl;
//     return 0;
// }