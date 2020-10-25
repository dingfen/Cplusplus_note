#include "basic_graph.hpp"
#include <iostream>

int S[100] = {0};

void DFS(undirectedGraph & G, int begin)
{
    cout<<begin<<endl;
    S[begin] = 1;
    for(int i=0;i<G[begin].size();i++) {
        if(S[G[begin][i].node] == 0) {
            DFS(G, G[begin][i].node);
            S[G[begin][i].node] = 1;
        }
    }
}

int main(int argc, char const *argv[])
{
    undirectedGraph G;
    int begin;
    G = undirectedGraphCreate();
    cout<<"Begin DFS point: "<<endl;
    cin >> begin;
    DFS(G, begin);
    return 0;
}
