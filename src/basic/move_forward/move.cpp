#include <string>
#include <iostream>

class Demo {
private:    
    std::string m_s;
public:
    Demo();
    ~Demo();

    void set(const std::string &);
    void set(const std::string &&);
};

Demo::Demo() : m_s("") {
}

Demo::~Demo() {
}

void Demo::set(const std::string &s) {
    std::cout << "lvalue reference use copy" << std::endl;
    m_s = s;
}

void Demo::set(const std::string &&s) {
    std::cout << "Rvalue reference use std::move" << std::endl;
    m_s = std::move(s);
}


int main(int argc, char const *argv[])
{
    std::string s("a very very long string");
    Demo d;
    d.set("a very long string");
    d.set(s);
    return 0;
}
