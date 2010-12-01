#include <iostream>

template<typename T>
class ScopedPtr
{
public:
    explicit ScopedPtr( T* ptr )
        : m_ptr(ptr),
          m_name( "unnamed" )
    {
    }

    ScopedPtr( T* ptr, const std::string& name )
        : m_ptr(ptr),
          m_name( name )
    {
    }

    ~ScopedPtr()
    {
        std::cout << "Deleting: " << m_name << std::endl;
        delete m_ptr;
    }

    T& operator * () const
    {
        return *m_ptr;
    }

    T * get() const
    {
        return m_ptr;
    }

    T * operator ->() const
    {
        return m_ptr;
    }

private:
    ScopedPtr( const ScopedPtr<T>& ptr );
    ScopedPtr<T>& operator = ( const ScopedPtr<T>& rhs );

    T * m_ptr;
    std::string m_name;
};

class AwesomeClass
{
    public:
        int getIt() const { return 42; }
};

int printIt( int * val )
{
    std::cout << "Value of pointer is " << *val << std::endl;
    return *val;
}



int main( int argc, char* argv[] )
{
    ScopedPtr<int> ptr( new int(42) );
    printIt( ptr.get() );


    ScopedPtr<AwesomeClass> nptr( new AwesomeClass );
    nptr->getIt();

    std::cout << "Value of dereferenced int is: " << *ptr << std::endl;

    return 0;
}
