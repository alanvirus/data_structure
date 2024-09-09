//模式：先处理base case递归基（非递归地）

//线性递归：自己调用自己一次
int sum(int A[],int n){
    if(n<1)              
        return 0;
    else
        return A[n-1]+sum(A,n-1);
}
//复杂度分析：
//递归跟踪(recursion trace):将每一个递归实例表示为一个方框，注明参数；如果A实例调用了B实例，连一条A到B的有向边
//递推方程(recurrence equation):确定递推方程和边界条件