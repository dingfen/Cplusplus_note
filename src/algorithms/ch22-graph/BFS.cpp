/*
 * 广度优先搜索法
 */

#include <iostream>
#include "basic_graph.hpp"
#include <queue>

void BFS(undirectedGraph & G, int begin)
{
    int S[100] = {0};
    queue<Edges> q;

    for(int i=1;i<G.size();i++) {
        for(int j=0;j<G[i].size();j++) {
            S[G[i][j].node] = 1;
        }
    }
    Edges first;
    first.node = begin;
    first.weight = 0;
    q.push(first);
    S[begin] = 0;

    while (!q.empty())
    {
        Edges e = q.front();
        q.pop();
        cout<<e;
        for(int i=0;i<G[e.node].size();i++) {
            if(S[G[e.node][i].node] == 1) {
                q.push(G[e.node][i]);
                S[G[e.node][i].node] = 0;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    undirectedGraph G;
    G = undirectedGraphCreate();
    int e;
    cout<<"BFS start point."<<endl;
    cin>>e;
    BFS(G, e);
    return 0;
}

