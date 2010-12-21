/**
 * Empty a vector and free reserved memory
 */
vector<T>().swap(v);        // swap the vector with a temporary empty

/**
 * Copy a map to vector
 */
std::map<A,B> map
std::vector< pair<A,B> > vec( map.begin(), map.end() );

// -or-
vec.assign( map.begin(), map.end() );


/**
 * Check if container is sorted
 */
std::adjacent_find( container.begin(),
                    container.end(),
                    greater<Container::value_type>() ) == container.end();

/**
 * Simple way of printing a container
 */
std::vector<T> container;
std::copy( std::istream_iterator<T>( std::cin ),
           std::istream_iterator<T>(),
           std::back_inserter( container ) )

// -or-
std::copy( container.begin(), container.end(),
           std::ostream_iterator<T>( std::cout, "\n" ) )

// maybe this works?
template<typename Container>
void print( const Container& c )
{
    std::copy( c.begin(), c.end(),
               std::ostream_iterator<Container::value_type>( std::cout, "\n" )
    );
}
}
