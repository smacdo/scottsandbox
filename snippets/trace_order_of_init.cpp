/**
 * Shows order of construction in C++
 */
#include <iostream>

int GCurrentScope = 0;

class DebugCounter
{
public:
    DebugCounter( const char * name, int initialValue=0 )
        : name( name ),
          value( initialValue )
    {
    }

private:
    const char * name;
    int value;
};

class Trace
{
public:
    Trace()
        : name("trace")
    {
        enter();
    }

    Trace( const Trace&, const std::string& name )
        : name(name)
    {
        enter();
    }

    Trace( const Trace&, const Trace&, const std::string& name )
        : name(name)
    {
        enter();
    }

    Trace( const std::string& name )
        : name(name)
    {
        enter();
    }

    operator int() const
    {
        return 42;
    }

    ~Trace()
    {
        leave();
    }

private:
    void enter()
    {
        start = clock();

        for ( int i = 0; i < GCurrentScope; ++i ) { std::cout << "="; }
        GCurrentScope++;

        std::cout << ">: " << name << std::endl;
    }

    void leave()
    {
        stop = clock();

        GCurrentScope--;
        for ( int i = 0; i < GCurrentScope; ++i ) { std::cout << "="; }

        std::cout << "<: " << name << std::endl;
    }

private:
    std::string name;
    clock_t     start;
    clock_t     stop;
};

Trace GTrace("global trace");

void multiargs_1()
{
    std::cout << "mutltiargs #1\n";
    Trace a( Trace("a_arg_1"), "a" );
}

void multiargs_2()
{
    std::cout << "multiargs #2\n";
    Trace b( Trace("b_arg_1"), Trace("b_arg_2"), "b" );
}

void multiargs_3()
{
    std::cout << "multiargs #3\n";
    Trace c( Trace( Trace( "c_a_arg1" ), Trace( "c_a_arg2" ), "c_a" ),
             Trace( Trace( "c_b_arg1" ), Trace( "c_b_arg2" ), "c_b" ),
             "c" );
}

void with_static()
{
    Trace f( "f" );
    static Trace s( Trace("s_arg1"), "s" );

    Trace t( "static_inner" );
}

void testswitch()
{
/*    Trace top("top_of_switch");

    switch( top )
    {
        Trace xxx("switch_before_cases");
        case 1
        {
            Trace("case_1");
            break;
        }

        case 2:
        {
            Trace("case_2");
            break;
        }

        case 42:
        {
            Trace("case_42");
            break;
        }

        default:
        {
            Trace("casea_default");
            break;
        }

        Trace("bottom of switch function");
    }

    Trace("bottom_of_switch_func");*/
}

int main( int argc, char* argv[] )
{
    multiargs_1();
    multiargs_2();
    multiargs_3();

    std::cout << "\n --- static scope #1 --- \n";
    std::cout << "    (Static 's' should only be created this time\n";
    with_static();
    std::cout << "\n --- static scope #2 --- \n";
    with_static();
    std::cout << "\n --- static scope #3 (last) --- \n";
    std::cout << "    (Static 's' should not destructed)\n";
    with_static();

    std::cout << "\n --- switch scope ---\n";
    testswitch();

    std::cout << "\n\n********** MAIN LAST LINE **********\n";
}
