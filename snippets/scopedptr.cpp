#include <iostream>
#include <boost/scoped_ptr.hpp>

using boost::scoped_ptr;

class Foo
{
public:
    Foo()
        : name("default")
    {
        std::cout << "Foo:ctor, name=default" << std::endl;
    }

    Foo( const std::string& name )
        : name( name )
    {
        std::cout << "Foo:ctor2, name=" << name << std::endl;
    }

    ~Foo()
    {
        std::cout << "Foo:dtor, name=" << name << std::endl;
    }

    Foo( const Foo& f )
        : name( f.name )
    {
        std::cout << "Foo:copyctor, name=" << name << std::endl;
    }

    Foo& operator = ( const Foo& f )
    {
        name = f.name;
        std::cout << "Food:assign, name=" << name << std::endl;
        return *this;
    }

private:
    std::string name;
};

class Bar
{
public:
    Bar()
        : m_ptr( new Foo("alice") )
    {
        std::cout << "Bar constructor start" << std::endl;
        m_ptr.reset( new Foo("bob") );
        std::cout << "Bar constructor stop" << std::endl;
    }

    ~Bar()
    {
        std::cout << "Bar destructor" << std::endl;
    }

    void test()
    {
        std::cout << "Test func start" << std::endl;
        scoped_ptr<Foo> temp( new Foo("charlie") );
        m_ptr.swap( temp );
        std::cout << "Test func stop" << std::endl;
    }

private:
    scoped_ptr<Foo> m_ptr;
};

int main( int argc, char* argv[] )
{
    scoped_ptr<int> a( new int( 42 ) );
    std::cout << "a: " << *a << std::endl;

    std::cout << "===============" << std::endl;
    std::cout << "- Dynamic allocate bar" << std::endl;
    Bar *pBar = new Bar();

    std::cout << "- test func on bar" << std::endl;
    pBar->test();

    std::cout << "- destroying bar" << std::endl;
    delete pBar;

    
}
