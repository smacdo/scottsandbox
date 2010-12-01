#include <iostream>

void f( float a, float b )
{
    std::cout << "f(float " << a << ",float " << b << ")" << std::endl;
}

void f( double a, double b )
{
    std::cout << "f(double " << a << ",double " << b << ")" << std::endl;
}

template<typename T>
void f( const T& a, const T& b )
{
    std::cout << "f(T " << a << ", T " << b << ")" << std::endl;
}

int main( int, char** )
{
    float fa = 2.0, fb = 4.0;
    double da = 2.0, db = 4.0;
    int ia = 2, ib = 2;

    std::cout << "FF" << std::endl;
    f(fa,fb);

    std::cout << "DD" << std::endl;
    f(da,db);
    
    std::cout << "II" << std::endl;
    f(ia, ib);

    std::cout << "FD" << std::endl;
    f(fa,db);

    std::cout << "DF" << std::endl;
    f(da,fb);

    std::cout << "ID" << std::endl;
    f(ia,db);

    std::cout << "IF" << std::endl;
    f(ia,fb);

    std::cout << "FI" << std::endl;
    f(fa,ib);
}
