
template<typename T>
class ShuffleBag
{
public:
    void add( const T& object );
    void add( const T& object, size_t weight );

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
ShuffleBag::ShuffleBag()
    : m_values(),
      m_cursor( 0 )
{
}

template<typename T>
ShuffleBag::ShuffleBag( const ShuffleBag<T>& bag )
    : m_values( bag.m_values ),
      m_cursor( 0 )
{
}

template<typename T>
void ShuffleBag::add( const T& object )
{
    m_values.push_back( object );
}

template<typename T, typename Container>
void ShuffleBag::addMany( const Container& input )
{
    typedef Container::const_iterator itr = input.begin();

    for ( itr = input.begin(); itr != input.end(); ++itr )
    {
        m_values.push_back( *itr );
    }
}

template<typename T>
void ShuffleBag::add( const T& object, size_t weight )
{
    // Add it many times
    for ( size_t i = 0; i < weight; ++i )
    {
        m_values.push_back( object );
    }
}

template<typename T>
void ShuffleBag::reshuffle()
{
}

template<typename T>
size_t ShuffleBag::count()
{
    return m_values.size();
}


template<typename T>
T ShuffleBag::takeOne()
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
std::vector<T> ShuffleBag::takeMany( size_t picks )
{
    typedef std::vector<T> results;

    for ( size_t i = 0; i < picks; ++i )
    {
        results.push_back( takeOne() );
    }

    return results;
}
