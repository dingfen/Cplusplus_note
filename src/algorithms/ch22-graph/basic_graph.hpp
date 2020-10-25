/*
 * 图基本算法 BFS DFS
 * 邻接矩阵 邻接表 表示 
 */ 

#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef struct Edges
{
    int weight;
    int node;
    
    friend ostream & operator<<(ostream & out, const struct Edges &e)
    {
        out<<e.node<<" "<<e.weight<<endl;
        return out;
    }
} Edges;

// 无向图 邻接列表 表示
typedef map<int, vector<Edges>> undirectedGraph;

// 有向图 邻接列表 表示
typedef map<int, vector<Edges>> directedGraph;

undirectedGraph undirectedGraphCreate()
{
    int a,b,w;
    undirectedGraph G;
    while (cin>>a>>b>>w) {
        if(a == -1 && b == -1)
            break;
        
        Edges e;
        e.weight = w;
        e.node = b;
        G[a].push_back(e);
        e.node = a;
        G[b].push_back(e);
    }
    return G;
};

void show(undirectedGraph & G)
{
    for(int i=1;i<G.size();i++) {
        for(int j=0;j<G[i].size();j++) {
            cout<<i<<" "<<G[i][j].node<<" "<<G[i][j].weight<<endl;
        }
    }
}

directedGraph directedGraphCreate()
{
    int a,b,w;
    directedGraph G;
    while (cin>>a>>b>>w) {
        if(a == -1 && b == -1)
            break;

        Edges e;
        e.weight = w;
        e.node = b;
        G[a].push_back(e);
    }
    return G;
};

