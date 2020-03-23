#include <iostream>

using std::cout;
using std::endl;

class Animal
{
public:
    Animal() {};
    ~Animal() {};

    virtual void name() = 0;
};

class Dog : public Animal {
private:
    std::string _name;

public:
    Dog():_name("dog") {};
    ~Dog() {};

    void name() {
        cout << "I am a " << _name << endl;
    };
};

class Cat : public Animal {
private:
    std::string _name;
public:
    Cat():_name("cat") {};
    ~Cat() {};

    void name() {
        cout << "I am a " << _name << endl;
    }
};



int main(int argc, char const *argv[])
{
    Dog d;
    Cat c;
    Animal *pA = &d;
    pA->name();
    pA = &c;
    pA->name();
    return 0;
}
