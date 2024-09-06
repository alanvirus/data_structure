#include <iostream>
#include <vector>
#include <algorithm> // for std::swap

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // Swap elements at index 1 and index 3
    std::swap(vec[1], vec[3]);

    // Print the vector after swap
    for (int elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}
