#include<iostream>
using namespace std;
void placeQueens(int N){
    int* solu=new int[N];
    for(int i=0;i<N;i++){
        solu[i]=0;
    }
    int cur_r=0;
    int cur_c=0;
    int total=0;
    while(true){
        //在（cur_r,cur_c）放置王后是否冲突
        bool contradict=false;
        for(int i=0;i<cur_r;i++){
            if(solu[i]==cur_c||(solu[i]+i)==(cur_c+cur_r)||(solu[i]-i)==(cur_c-cur_r)){
                contradict=true;
                break;
            }
        }
        if(contradict){
            //尝试在同一行下一个位置放置，如果当前行已检查完，回溯至前一行
            cur_c++;
            while(cur_c==N){
                cur_r--;
                if(cur_r<0){
                    break;
                }else{
                    cur_c=solu[cur_r]+1;
                }
            }
            if(cur_r<0)break;
            // cout<<"contradict"<<cur_r<<" "<<cur_c<<endl;
        }else{
            //不冲突则入栈，从下一行的第0列开始放置；如果已到最后一行，总数+1，检查同一行下一位置
            solu[cur_r]=cur_c;
            cur_r++;
            if(cur_r<N){
                cur_c=0;
            }else{
                total++;
                cur_r--;
                cur_c++;
                while(cur_c==N){
                    cur_r--;
                    if(cur_r<0){
                        break;
                    }else{
                        cur_c=solu[cur_r]+1;
                    }
                }
                if(cur_r<0)break;
            }
            // cout<<"not contradict"<<cur_r<<" "<<cur_c<<endl;
        }
    }
    cout<<N<<" "<<total<<endl;
    delete[] solu;
}
int main(){
    for(int i=4;i<40;i++){
        placeQueens(i);
    }
}