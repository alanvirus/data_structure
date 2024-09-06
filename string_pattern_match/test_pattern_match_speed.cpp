#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "match_brute_force.cpp"
#include "match_with_rolling_hash_v2.cpp"
#include "bm_match.cpp"
#include "kmp_match.cpp"

using namespace std;
using namespace std::chrono;

// Function prototypes
// vector<int> search_brute_force(string& T, string& P);
// vector<int> search_rolling_hash_v1(string& T, string& P);
// vector<int> search_rolling_hash_v2(string& T, string& P);

// Generate a random string of a given length
string generate_random_string(int length)
{
    string str(length, ' ');
    for (int i = 0; i < length; ++i)
    {
        str[i] = 'a' + rand() % 26;  // Random lowercase letter
    }
    str[length - 1] = 'a';
    str[length - 2] = 'a';
    str[length - 3] = 'a';
    str[length - 4] = 'a';
    str[length - 5] = 'a';
    return str;
}

// Test performance of different search algorithms
void test_performance(string &T, string &P, int iterations)
{

    auto start = high_resolution_clock::now();
    auto end = high_resolution_clock::now();

    start = high_resolution_clock::now();
    rolling_hash_v2 match1;
    vector<int> out1;
    for (int i = 0; i < iterations; ++i)
    {
        out1=match1.search(T, P);
    }
    end = high_resolution_clock::now();
    auto duration3 = duration_cast<nanoseconds>(end - start);
    cout << "Rabin-Karp v2: " << duration3.count()<< " nanoseconds  " << endl;

    // start = high_resolution_clock::now();
    // brute_force_match match2;
    // vector<int> out2;
    // for (int i = 0; i < iterations; ++i)
    // {
    //     out2=match2.search(T, P);
    // }
    // end = high_resolution_clock::now();
    // auto duration1 = duration_cast<nanoseconds>(end - start);
    // cout << "Brute Force:   " << duration1.count() << " nanoseconds " << endl;

    start = high_resolution_clock::now();
    bm_match match3;
    vector<int> out3;
    for (int i = 0; i < iterations; ++i)
    {
        out3=match3.search(T, P);
    }
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<nanoseconds>(end - start);
    cout << "boyal moore:   " << duration2.count()<< " nanoseconds  " << endl;

    start = high_resolution_clock::now();
    kmp_match match4;
    vector<int> out4;
    for (int i = 0; i < iterations; ++i)
    {
        out4=match4.match(T, P);
    }
    end = high_resolution_clock::now();
    auto duration4 = duration_cast<nanoseconds>(end - start);
    cout << "kmp:           " << duration4.count()<< " nanoseconds  " << endl;

    bool areEqual = std::equal(out1.begin(), out1.end(), out4.begin());
    bool areEqual2 = out1.size()==out4.size();
    std::cout << "equal: " << (areEqual&&areEqual2) << std::endl; // 输出 true
    areEqual = std::equal(out1.begin(), out1.end(), out3.begin());
    areEqual2 = out1.size()==out3.size();
    std::cout << "equal: " << (areEqual&&areEqual2) << std::endl; // 输出 true
    // areEqual = std::equal(out1.begin(), out1.end(), out2.begin());
    // std::cout << "equal: " << areEqual << std::endl; // 输出 true
}

int main()
{
    srand(time(nullptr)); // Seed the random number generator
    int iterations = 1;  // Number of iterations for each search
    for (int length = 100000; length <= 100000000; length *= 10)
    {
        string T = generate_random_string(length);
        // for(int j=0;j<1000;j++){
        //     T+="abcdef";
        // }
        string P = "aaaaa";
        // string T(length,'a');
        // T+='c';
        // string P(100,'a'); // Pattern remains constant for this example
        // P+='c';
        cout << endl<<"Testing with text length: " << length << endl;
        test_performance(T, P, iterations);
    }

    return 0;
}