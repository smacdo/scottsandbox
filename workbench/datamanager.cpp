#include <string>
#include <memory>
#include <unordered_map>

#include <cassert>
#include <iostream>

template<typename T>
class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    /**
     * Event method that is called when a requested resource is missing.
     * This method allows a derieved class to implement custom delayed
     * loading abilities or error reporting.
     *
     * If the method returns true, the resource manager will attempt to
     * re-obtain the missing resource. If false, it will simple error out
     * and fail.
     */
    virtual bool onResourceMissing( const std::string& name );

public:
    /**
     * Looks up the named resource, and returns a shared pointer to
     * the resource.
     */
    std::shared_ptr<T> request( const std::string& name );

    /**
     * Queries the resource manager to see if the named resource is
     * currently loaded
     */
    bool has( const std::string& name ) const;

    /**
     * Adds the named resource
     */
    void add( const std::string& name, T* object );

    /**
     * Removes the named resource
     */
    void remove( const std::string& name );

    /**
     * Returns an instance to the resource manager
     */
    static ResourceManager<T>& instance();

    /**
     * Forcibly purges the resource manager of all allocated memory.
     */
    static void kill();

private:
    typedef std::shared_ptr<T> SharedPtrT;
    typedef std::unordered_map<std::string, SharedPtrT> ResourceMap;

    ResourceMap m_registry;

 //   static ResourceManager<T> m_self;
};


//template<typename T>
//ResourceManager<T> ResourceMananger<T>::m_self;

template<typename T>
ResourceManager<T>::ResourceManager()
{
}

template<typename T>
ResourceManager<T>::~ResourceManager()
{
    kill();
}

template<typename T>
std::shared_ptr<T> ResourceManager<T>::request( const std::string& name )
{
    typename ResourceMap::iterator itr;

    // Look it up
    itr = m_registry.find( name );

    // Does it exist?
    if ( itr != m_registry.end() )
    {
        return itr->second;
    }
    else
    {
        // Signal that it doesn't exist, and give the class a chance to
        // load it
        if (! this->onResourceMissing( name ) )
        {
            assert( false );
        }

        itr = m_registry.find( name );

        if ( itr == m_registry.end() )
        {
            assert( false );
        }
        else
        {
            return itr->second;
        }
    }
}

template<typename T>
void ResourceManager<T>::add( const std::string& name, T *obj )
{
    SharedPtrT ptr( obj );
    typename ResourceMap::value_type value( name, ptr );

    // Make sure it does not already exist
    if ( m_registry.find( name ) != m_registry.end() )
    {
        assert( false && "Object was already registered" );
    }
    
    m_registry.insert( value );
}

template<typename T>
void ResourceManager<T>::kill()
{
}

template<typename T>
bool ResourceManager<T>::onResourceMissing( const std::string& name )
{
    std::cout << name << " is missing" << std::endl;
    return false;
}


#include <googletest/googletest.h>

struct SimpleObject
{
    SimpleObject( int val )
        : value( val ),
          copies( 0 )
    {
    }

    SimpleObject( const SimpleObject& s )
        : value ( s.value ),
          copies( s.value + 1 )
    {
    }

    SimpleObject& operator = ( const SimpleObject& rhs )
    {
        value  = rhs.value;
        copies = rhs.copies + 1;
        return *this;
    }

    int value;
    int copies;
};

TEST(ResourceManager, SimpleSetAndGet)
{
    using std::shared_ptr;

    ResourceManager<SimpleObject> rm;
    SimpleObject *so = new SimpleObject( 42 );

    rm.add( "myobject", so );
    shared_ptr<SimpleObject> r = rm.request( "myobject" );

    EXPECT_EQ( r.get(), so );
    EXPECT_EQ( r->value, so->value );
    EXPECT_EQ( r->copies, so->copies );
}
