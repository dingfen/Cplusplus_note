#include <iostream>

using std::cout;
using std::endl;

class Shape{
public:
    virtual void show() {
        cout << "I am Shape." <<endl;
    }

    void print() {
        cout << "It is Shape." <<endl;
    }
};

class Rectangle : public Shape {
public:
    void show() {
        cout << "I am Rectangle." <<endl;
    }

    void print() {
        cout << "It is Rectangle." <<endl;
    }
};

class Triangle : public Shape {
public:
    void show() {
        cout << "I am Triangle." <<endl;
    }
    
    void print() {
        cout << "It is Triangle." <<endl;
    }
};

class Circle : public Shape {
public:
    void show() {
        cout << "I am Circle." <<endl;
    }

    void print() {
        cout << "It is Circle." <<endl;
    }
};


int main(int argc, char const *argv[])
{
    Shape* spt[3];
    Circle c;
    Rectangle r;
    Triangle t;
    
    spt[0] = &c;
    spt[1] = &r;
    spt[2] = &t;
    for (int i = 0; i < 3; i++) {
        spt[i]->show();
        spt[i]->print();
    }
    return 0;
}
