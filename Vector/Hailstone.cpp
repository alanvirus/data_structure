#include <iostream>
#include "Vector.h"
template<typename T>
class print{
    public:
    void operator()(T& e){
        std::cout<<e<<" ";
    }
};
int hailstone(int n)//根据
{
    Vector<int> elem(100);
    while (1)
    {
        if (n == 1)
        {
            elem.insert(1);
            break;
        }
        if (elem.find(n)>=0)
        { // 成环了
            break;
        }
        // std::cout<<n<<" ";
        elem.insert(n);
        if (n % 2)
        {
            n = 3 * n + 1;
        }
        else
        {
            n >>= 1;
        }
    }
    print<int> print;
    elem.traverse(print);
    std::cout<<std::endl;
    return elem.size();
}
void test_hailstone()
{
    // std::cout << "out"<<hailstone(4) << " ";
    for (int i = 1; i < 10; i++)
        std::cout << hailstone(i) << std::endl;;
}