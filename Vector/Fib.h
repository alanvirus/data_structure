class Fib{
    private:
        int f,g;//fib(k-1),fib(k) 以速度(1+sqrt(5))/2加倍，很快就会溢出int
    public:
        Fib(int n){ //log_phi(n)
            f=1;g=0; //... 13 -8 5 -3 2 -1 1 0 1 1 2 3 5 8 13 ...
            while(g<n){
                next();
            }
        }
        int get(){
            return g;
        }
        int next(){
            g+=f;
            f=g-f;
            return g;
        }
        int prev(){
            f=g-f;
            g-=f;
            return g;
        }
};