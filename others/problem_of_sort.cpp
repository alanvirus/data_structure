#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
using namespace std;
class Solution {
public:
    vector<int> frequencySort(vector<int>& nums) {
        std::map<int, int> hm;
        for (int numb : nums)
            hm[numb] ++ ;
        sort(nums.begin(), nums.end(), [&](int a,int b){
            if(hm[a] == hm[b])
                return a > b;
            return hm[a] <  hm[b];
        });
        cout<<"before";
        return nums;
        cout<<"after";
        vector<int> arr;
        for(auto &[numb,freq] :  hm){
            while(freq > 0){
                arr.push_back(numb);
                freq--;
            }
        }
        std::sort(arr.begin(), arr.end(), [&](int a, int b){
            if( hm[a] == hm[b])
                return a > b;
            else
                return hm[a] <  hm[b];
        });
        return arr;
    }
};
int main(){
    Solution s;
    vector<int> vec={1,1,2,2,2,3};
    vector<int> out=s.frequencySort(vec);
    for (const auto &element : out) {
        std::cout << element << " ";
    }
}