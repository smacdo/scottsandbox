/**
 * Assignment operator called via pointer. Did this quick test to verify
 * that you can invoke a custom assignment operator via pointer derefence.
 */
#include <iostream>

class Foo
{
public:
    Foo( int bar_ )
        : bar(bar_)
    {
    }

    Foo& operator = ( const Foo& f )
    {
        std::cout << "operator = called (foo '" << bar
            << "' := " << f.bar << " --> " << f.bar << ")" << std::endl;
        bar = f.bar;

        return *this;
    }

    int bar;
};

int main( int , char* [] )
{
    // Base
    Foo* f = new Foo( 0 );

    // First, assign when both pointers
    Foo* a = new Foo( 2 );
    *f = *a;

    // Assign from local
    Foo b( 12 );
    *f = b;

    return 0;
}
