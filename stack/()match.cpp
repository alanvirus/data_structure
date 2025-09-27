#include<iostream>
#include <cstring>
#include"Stack.h"
bool paren(const char exp[],int lo ,int hi){
    Stack<char> S;
    for(int i=lo;i<=hi;i++){
        switch(exp[i]){
            case '(': case '[':case '{':S.push(exp[i]);break;
            case ')': if((S.empty()||'('!=S.pop()))return false; break;
            case ']': if((S.empty()||'['!=S.pop()))return false; break;
            case '}': if((S.empty()||'{'!=S.pop()))return false; break;
            default: break;
        }
    }
    return S.empty();
}
int main(){
    char exp[]="[){(){}}]{}[{({[]})}]";
    std::cout<<paren(exp,0, std::strlen(exp) -1);
    std::cout<<std::strlen(exp)<<' ';
    std::cout<<sizeof(exp)/sizeof(exp[0]);
}