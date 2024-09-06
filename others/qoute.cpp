#include<iostream>
using namespace std;
int main(){
    //Test show that the variable and all its qoute can be seen as the same ref to some mem
    int data=16;
    int *ptr=&data;
    int & data_qoute=data;
    int *& ptr_qoute= ptr;
    int*&ptr_qoute2= ptr_qoute;
    int * ptr_else=ptr_qoute2;
    cout<<*ptr<<endl;
    cout<<data_qoute<<endl;
    cout<<*ptr_qoute<<endl;
    cout<<*ptr_qoute2<<endl;
    cout<<*ptr_else<<endl;
    cout<<ptr<<endl;
    cout<<ptr_qoute<<endl;
    cout<<ptr_qoute2<<endl;
    cout<<ptr_else<<endl;
    int *ptr2=new int(12);
    cout<<*ptr2<<" "<<ptr2<<endl;
    ptr=ptr2;

    cout<<*ptr<<endl;
    cout<<data_qoute<<endl;
    cout<<*ptr_qoute<<endl;
    cout<<*ptr_qoute2<<endl;
    cout<<*ptr_else<<endl;
    cout<<ptr<<endl;
    cout<<ptr_qoute<<endl;
    cout<<ptr_qoute2<<endl;
    cout<<ptr_else<<endl;
}