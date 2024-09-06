#include <iostream>
class node{
public:
    int a;
    node *b;
    node(int a,node *b):a(a),b(b){}
};
int main(){
    int a=1;
    node b(2,nullptr);
    node *nod=new node(a, &b);
    std::cout<<nod->a<<" "<<nod->b->a<<std::endl;
    delete nod;
    std::cout<<nod->a<<" "<<nod->b->a<<" "<<b.a;
}