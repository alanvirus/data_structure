int fib(int n){
    int f=0;int g=1;
    while(0<n--){
        g+=f; f=g-f;
    }
    return f;
}