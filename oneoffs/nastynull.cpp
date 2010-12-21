/**
 * References in C++ are assumed to be non-null, however through some
 * trickery we can force the language to take a null reference. This does
 * not mean we should start treating refs like pointers (null check), but
 * it merely demonstrates that we can subvert the system fi we're not
 * careful.
 *
 * Just make sure whenever you cast from T* to T& that the address isn't
 * null and you'll be OK.
 */
#include <iostream>

void printValue( const int& v )
{
    std::cout << "The value is: " << v << std::endl;
}

int main()
{
    int * someValue = new int( 42 );
    printValue( *someValue );       // this works!

    int * evilValue = NULL;
    printValue( *evilValue );       // hahahahaha SEGFAULT ON THIS SUCKERS
}
