#include <iostream>
#include <vector>

int main() {
    // 创建一个 vector<int>
    std::vector<int> vec1 = {1, 2, 3, 4, 5};

    // 使用引用
    std::vector<int>& vecRef1 = vec1;

    // 使用裸指针指向相同的 vector<int>
    std::vector<int>* vecPtr2 = &vec1;
    std::cout <<&vec1<<std::endl;
    std::cout <<&vecRef1<<std::endl;
    // // 修改通过引用
    // vecRef1.push_back(6);

    // // 打印两个引用和指针的 vector 数据，验证它们共享相同的内存
    // std::cout << "vecRef1: ";
    // for (int elem : vecRef1) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "vecPtr2: ";
    // for (int elem : *vecPtr2) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;

    return 0;
}
