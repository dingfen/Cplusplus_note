#ifndef GRAPH__H
#define GRAPH__H

#include <string>
#include <vector>

typedef enum {WHITE, BLACK, GRAY} Color;

class vertex
{
private:
    int num;
    std::string name;
    Color color;
public:
    vertex(std::string name, int num):name(name),num(num) {};
    ~vertex() {};

    int getNum() {return num;};
    void setNum(int n) {num = n;};
    std::string getName() {return name;};
    Color getColor() {return color;};
};

class graph
{
private:
    std::vector<vertex> points;
    std::vector<std::vector<int>> edges;
public:
    graph() {};
    ~graph() {};
};

void CreateGraph(graph & G)
{
    
}

#endif