#include <cstddef>
#include <vector>

template<typename T>
class ShuffleBag
{
public:
    ShuffleBag();
    ShuffleBag( const ShuffleBag& bag );

    void add( const T& object );
    void add( const T& object, size_t weight );

    template<typename Container>
    void addMany( const Container& input )
    {
        typename Container::const_iterator itr = input.begin();

        for ( itr = input.begin(); itr != input.end(); ++itr )
        {
            m_values.push_back( *itr );
        }
    }

    /**
     * Returns the number of elements stored in the shuffle bag
     */
    size_t count() const;

    /**
     * Forcibly re-shuffles the shuffle bag. This will restock the 
     * possibilities
     */
    void reshuffle();

    T takeOne();

    std::vector<T> takeMany( const size_t times );

private:
    std::vector<T> m_values;
    size_t m_cursor;
};

template<typename T>
ShuffleBag<T>::ShuffleBag()
    : m_values(),
      m_cursor( 0 )
{
}

template<typename T>
ShuffleBag<T>::ShuffleBag( const ShuffleBag<T>& bag )
    : m_values( bag.m_values ),
      m_cursor( 0 )
{
}

template<typename T>
void ShuffleBag<T>::add( const T& object )
{
    m_values.push_back( object );
}

template<typename T>
void ShuffleBag<T>::add( const T& object, size_t weight )
{
    // Add it many times
    for ( size_t i = 0; i < weight; ++i )
    {
        m_values.push_back( object );
    }
}

template<typename T>
void ShuffleBag<T>::reshuffle()
{
}

template<typename T>
size_t ShuffleBag<T>::count() const
{
    return m_values.size();
}


template<typename T>
T ShuffleBag<T>::takeOne()
{
    assert( m_values.size() > 0 );

    if ( m_cursor > m_values.size() )
    {
        reshuffle();
        m_cursor = 0;
    }

    return m_values[ m_cursor++ ];
}

template<typename T>
std::vector<T> ShuffleBag<T>::takeMany( size_t picks )
{
    typename std::vector<T> results;

    for ( size_t i = 0; i < picks; ++i )
    {
        results.push_back( takeOne() );
    }

    return results;
}
