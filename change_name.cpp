# include <iostream>
using namespace std;

// char A[501000];
// char B[501000];

// int suan(int n,int m){
//     if(suan(n-1,m-1)==suan(n,m)){
        
//     }
// }
const int INF=1e9;
int main() {
    int n,m,k;
    cin >> n >> m >> k;
    string A,B;
    cin >> A >> B;
    if(n-m>k||m-n>k)
        cout<<-1<<endl;
    // long long ans = suan(n,m);
    long long ans = 0;
    int cost[2*k+1][m];
    for(int i = 0;i<2*k+1;i++){
        cost[i][0] = i-k;
    }
    for(int i = 1;i<m;i++){
        for(int j = 0;j<2*k+1;j++){
            if(j-k+i-2>=0&&B[i-1]==A[j-k+i-2]){
                cost[i][j] = cost[i-1][j];
            }else {
                int a=((j+1)<(2*k+1))?cost[i-1][j+1]:INF;
                int b=(j-1)>=0?cost[i][j-1]:INF;
                cost[i][j] = min(a,b)+1;
            }
        }
    }
    ans = cost[m-1][n-m+k];
    if(ans>k){
        cout<<-1<<endl;
    }else{
        cout<<ans<<endl;
    }
    return 0;
}