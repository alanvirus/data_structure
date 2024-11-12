#include"Stack.h"
#include<iostream>
static char digit[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void convert(Stack<char>& S,long long n,int base){
    Stack<char> tmp;
    while(n>0){
        int yushu=n%base;
        tmp.push(yushu);
        n=n/base;
    }
    while(!tmp.empty()){
        S.push(tmp.pop());
        std::cout<<S.top();
    }
}

int main(){
    Stack<char> tmp;
    convert(tmp,12345,8);
}