#include <iostream>
using namespace std;
int main(){
    int r=2;
    int j=2;
    while(r-->0){}//这两个表达式效果一样
    while(0<j--){}
    cout<<r<<" "<<j;
    //    -1      -1
}