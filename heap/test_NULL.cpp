#include <iostream>

int returnNull() {
    return (int)NULL; // NULL 是一个空指针常量，不能正确转换为 int
}

int main() {
    int result = returnNull();
    std::cout << "Result你好: " << result << std::endl;
    return 0;
}
