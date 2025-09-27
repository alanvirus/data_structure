#include"Stack.h"
#include<iostream>
static char digit[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void convert(Stack<int>& S,long long n,int base){
    Stack<int> tmp;
    while(n>0){
        tmp.push(n%base);
        n=n/base;
    }
    while(!tmp.empty()){
        S.push(tmp.pop());
        std::cout<<S.top()<<std::endl;
    }
}
void convert2(Stack<int>& S,long long n,int base){
    if(n>0){
        convert2(S,n/base,base);
        S.push(n%base);
        std::cout<<S.top()<<std::endl;
    }
}
int main(){
    Stack<int> tmp;
    convert2(tmp,12345,8);
}