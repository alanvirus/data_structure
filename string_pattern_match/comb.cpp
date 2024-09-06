//m个空位中放置K个1，打印出所有放置方法
#include <iostream>
using namespace std;
void print(int *p, int m, int K)
{
    int i = 1;
    int j = 1;
    while (j <= m)
    {
        if (i > K || p[i] > j)
        {
            cout << "0";
            j++;
        }
        else
        {
            cout << "1";
            j++;
            i++;
        }
    }
    cout<<endl;
}
int main()
{
    int count=1;
    int m = 15;
    int K = 5;
    int *p = new int[K + 1];
    for (int i = 1; i <= K; i++)
    {
        p[i] = i;
    }
    print(p, m, K);
    int a = K;
    while (a >= 1)
    {
        if (p[a] == m - K + a)
        {
            a--;
            continue;
        }
        else
        {
            p[a]++;
            if (a < K && p[a + 1] - p[a] > 1)
            {
                for (int i = a + 1; i <= K; i++)
                {
                    p[i] = p[a] + i - a;
                }
                a=K;
            }
            print(p, m, K);
            count++;
            continue;
        }
    }
    cout<<"total num:"<<count<<endl;
    return 0;
}