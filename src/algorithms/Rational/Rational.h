#ifndef RATIONAL__H_
#define RATIONAL__H_

#include <iostream>

class Rational
{
private:
    int numerator;   // 分子
    int denominator; // 分母
public:
    Rational(int numerator = 0, int denominator = 1):
        numerator(numerator), denominator(denominator) {};
    
    int getnumerator() const {return numerator;};
    int getdenominator() const {return denominator;};

    // 获取 double类型的值
    double value() const {return (double)numerator/denominator;};
    // 化简分数
    void simplify()
    {
        int big, small;
        if(numerator > denominator) {
            big = numerator;
            small = denominator;
        }
        else {
            big = denominator;
            small = numerator;
        }
        int b = big % small;
        while(b != 0) {
            big = small;
            small = b;
            b = big % small;
        }
        numerator /= small;
        denominator /= small;
    }

    friend std::ostream & operator<<(std::ostream & os, const Rational & r)
    {
        os << r.getnumerator()<<" / "<< r.getdenominator()<<std::endl;
        return os;
    }
    ~Rational() {};  
};


const Rational operator+(const Rational & lhs, const Rational & rhs)
{
    Rational ret(lhs.getnumerator()*rhs.getdenominator()+lhs.getdenominator()*rhs.getnumerator(),
        lhs.getdenominator()*rhs.getdenominator());
    ret.simplify();
    return ret;
}

const Rational operator-(const Rational & lhs, const Rational & rhs)
{
    Rational ret(lhs.getnumerator()*rhs.getdenominator()-lhs.getdenominator()*rhs.getnumerator(),
        lhs.getdenominator()*rhs.getdenominator());
    ret.simplify();
    return ret;
}

const Rational operator*(const Rational & lhs, const Rational & rhs)
{
    Rational ret(lhs.getnumerator()*rhs.getnumerator(), lhs.getdenominator()*rhs.getdenominator());
    ret.simplify();
    return ret;
}

const Rational operator/(const Rational & lhs, const Rational & rhs)
{
    Rational ret(lhs.getnumerator()*rhs.getdenominator(), lhs.getdenominator()*rhs.getnumerator());
    ret.simplify();
    return ret;
}

bool operator<(const Rational & lhs, const Rational & rhs)
{
    int lvalue = lhs.getnumerator()*rhs.getdenominator();
    int rvalue = rhs.getnumerator()*lhs.getdenominator();
    return lvalue < rvalue;
}

bool operator>(const Rational & lhs, const Rational & rhs)
{
    int lvalue = lhs.getnumerator()*rhs.getdenominator();
    int rvalue = rhs.getnumerator()*lhs.getdenominator();
    return lvalue > rvalue;
}

bool operator==(const Rational & lhs, const Rational & rhs)
{
    int lvalue = lhs.getnumerator()*rhs.getdenominator();
    int rvalue = rhs.getnumerator()*lhs.getdenominator();
    return lvalue == rvalue;
}

bool operator!=(const Rational & lhs, const Rational & rhs)
{
    int lvalue = lhs.getnumerator()*rhs.getdenominator();
    int rvalue = rhs.getnumerator()*lhs.getdenominator();
    return lvalue != rvalue;
}
#endif