// 并查集的应用

#include <iostream>
#include <vector>

using namespace std;

#define NUM 1000

int Tree[NUM];

int getRoot(int x)
{
    if(Tree[x] == -1) return x;
    else {
        int tmp = getRoot(Tree[x]);
        Tree[x] = tmp;
        return tmp;
    }
}


int main(int argc, char const *argv[])
{
    int N;
    int M;
    int i;
    while (cin >> N >> M)
    {
        int a, b;
        if(N == 0) break;
        for(i = 0;i< N;i++)
            Tree[i] = -1;
        for (i = 0; i < M; i++)
        {
            cin >> a >> b;
            a = getRoot(a);
            b = getRoot(b);
            if(a != b) Tree[a] = b;
        }
        int ans = 0;
        for(i = 1;i <= N;i++) {
            if(Tree[i] == -1)
                ans++;
        }
        cout<<ans<<endl;
    }
    return 0;
}
