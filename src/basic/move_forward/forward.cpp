#include <string>
#include <iostream>

class Demo {
private:    
    std::string m_s;
public:
    Demo();
    ~Demo();

    template<typename T>
    void set(T &&);
};

Demo::Demo() : m_s("") {
}

Demo::~Demo() {
}

template<typename T>
void Demo::set(T && s) {
    std::cout << "lvalue reference use forward" << std::endl;
    m_s = std::forward<T>(s);
}


int main(int argc, char const *argv[])
{
    std::string s("a very very long string");
    Demo d;
    d.set("a very long string");
    d.set(s);
    return 0;
}
