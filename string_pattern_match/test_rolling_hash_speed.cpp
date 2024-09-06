#include <iostream>
#include <string>
#include <chrono>
#include <cmath>

const long long prime = 101;
// const long long q= pow(10,9)+7;
// Function to calculate the rolling hash with pow function.
long long rolling_hash(const std::string &T, int start_pos, int len) {
    long long hash = 0;
    for (int i = 0; i < len; i++) {
        hash += T[start_pos + i] * std::pow(prime, len - 1 - i);
        // hash %=q;
    }
    return hash;
}

// Function to calculate the rolling hash without pow function.
long long rolling_hash_v2(const std::string &T, int start_pos, int len) {
    long long hash = 0;
    for (int i = 0; i < len; i++) {
        hash = hash * prime + T[start_pos + i];
        // hash %=q;
    }
    return hash;
}

// Test function to measure the time difference.
void test_speed_difference(const std::string &T, int len, int iterations) {
    auto start_pos = 0; // Starting position for the substring.
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Measure time for rolling_hash
    for (int i = 0; i < iterations; ++i) {
        // std::cout <<rolling_hash(T, start_pos, len)<<std::endl;
        rolling_hash_v2(T, start_pos, len);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_hash = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    start_time = std::chrono::high_resolution_clock::now();
    
    // Measure time for rolling_hash_v2
    for (int i = 0; i < iterations; ++i) {
        // std::cout <<rolling_hash_v2(T, start_pos, len)<<std::endl;
        rolling_hash(T, start_pos, len);
    }
    end_time = std::chrono::high_resolution_clock::now();
    auto duration_hash_v2 = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    
    std::cout << "Time taken by rolling_hash: " << duration_hash << " microseconds" << std::endl;
    std::cout << "Time taken by rolling_hash_v2: " << duration_hash_v2 << " microseconds" << std::endl;
}

int main() {
    const int iterations = 10000; // Number of times to call the functions.
    const int len = 1000; // Length of the substring.
    std::string T(len, 'a'); // Create a string of length 1000 filled with 'a'.
    test_speed_difference(T, len, iterations);
    
    return 0;
}