/**
 * Singleton implementations. Demonstrates various ways of implementing
 * the singleton pattern in C++
 */
#include <iostream>

//===========================================================================
// Classic implementation. Can either return a reference (slightly safer
// than a pointer, but not as widely used), or a pointer to the singleton.
//
// Constructor is made private to prevent anyone besides the Singleton
// class from constructing it. (Important)
//===========================================================================
class Singleton1
{
public:
    // Returns a reference to the constructed singleton pointer. This is
    // slightly safer since the caller can be assured the pointer is not
    // null (though not promised, since patholigically a reference can be
    // set to null but only through programmer intervention)
    static Singleton1& instance()
    {
        if ( singleton == NULL )
        {
            singleton = new Singleton1();
        }

        return *singleton;
    }

    // Returns a pointer to the constructed singleton pointer.
    static Singleton1* instancePtr()
    {
        if ( singleton == NULL )
        {
            singleton = new Singleton1();
        }

        return singleton;
    }

    void print()
    {
        std::cout << "singleton1" << std::endl;
    }

private:
    // Constructor is private!
    Singleton1()
    {
    }

    // For sanity, declare copy ctor and assignment constructors if you
    // want to prevent them too

    // The singleton
    static Singleton1 *singleton;
};

Singleton1* Singleton1::singleton = NULL;

//===========================================================================
// Another implmentation of the singleton pattern. This one's a little
// more clever, but slightly more efficient. By declaring a static varible,
// the instance is constructed at application load time, avoiding the need
// to check for null every single time the singleton is requested.
//===========================================================================
class Singleton2
{
public:
    static Singleton2& instance()
    {
        static Singleton2 mine;
        return mine;
    }

    void print()
    {
        std::cout << "singleton2" << std::endl;
    }
};

//===========================================================================
// Singleton class inhereitance
//===========================================================================
template<typename T>
class SingletonBase
{
public:
    static T& instance()
    {
        static T t;
        return t;
    }

protected:
    SingletonBase() { }
    SingletonBase( const SingletonBase& sb );
    SingletonBase operator = ( const SingletonBase& sb );
};

class Singleton3 : public SingletonBase<Singleton3>
{
public:
    void print() const
    {
        std::cout << "singleton3" << std::endl;
    }
};

int main()
{
    Singleton1::instance().print();
    Singleton1::instancePtr()->print();
    Singleton2::instance().print();
    Singleton3::instance().print();
}
