#include"List.h"
#include<iostream>
void print(int& j){
    std::cout<<j<<" ";
}
template<typename T>
struct Increase{
    virtual void operator() (T&e) {e++;}
};
template<typename T> 
void increase ( List<T> &L ) 
{   Increase<T> myIncrease;
    L.traverse ( myIncrease);  }
int main(){
    List<int> L;
    L.insertAsFirst(43);
    L.insertAsFirst(17);
    L.insertAsFirst(11);
    L.insertAsFirst(35);
    L.insertAsFirst(33);
    L.insertAsFirst(19);
    L.insertAsFirst(3);
    L.insertAsFirst(27);
    L.insertAsFirst(1);
    L.insertAsFirst(257);
    L.sort(L.first(),L.size());
    // increase(L);
    L.traverse(print);
}