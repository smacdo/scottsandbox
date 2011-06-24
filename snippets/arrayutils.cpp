

template<typename T>
std::string dumpContainer( const T& container, const char * sep=", " )
{
    std::stringstream ss;

    typename T::const_iterator itr = container.begin();
    typename T::const_iterator beg = container.begin();
    typename T::const_iterator end = container.end();

    for ( ; itr != end; ++itr )
    {
        if ( itr != beg )
        {
            ss << sep;
        }

        ss << *itr;
    }

    return ss.str();
}

template<typename T, typename U>
size_t count( const T& container, const U& value )
{
    typename T::const_iterator itr = container.begin();
    typename T::const_iterator end = container.end();
    size_t times = 0;

    for ( ; itr != end; ++itr )
    {
        if ( *itr == value )
        {
            times++;
        }
    }

    return times;
}

template<typename T>
bool isInContainer( const T& container, const U& value )
{
    return count( container, value ) > 0;
}

template<typename T>
void deletePointerContainer( T& container )
{
    typename T::iterator itr = container.begin();
    typename T::iterator end = container.end();

    for ( ; itr != end; ++itr )
    {
        delete *itr;

        // also assign value to null to prevent it from being accessed
        *itr = NULL;
    }
}
