/**
 * A LRU cache
 */
#include <iostream>
#include <cassert>
#include <vector>

template<typename K, typename V>
class LRUCache
{
public:
    LRUCache( size_t elements )
        : m_maxElements( elements )
    {
    }

    ~LRUCache()
    {
    }

    bool has( const K& key ) const
    {
        return ( m_elements.find(key) != m_elements.end() );
    }

    V    get( const K& key )
    {
        return V();
    }

    bool get( const K& key, V& value )
    {
        std::map<K,V>::iterator itr = m_elements.find(key);

        if ( key == m_elements.end() )
        {
            // not found
            return false;
        }
        else
        {
            // update LRU

            // return the value
            value = itr->second;
            return true;
        }
    }

    bool put( const K& key, const V& value )
    {
        return false;
    }
    
    bool remove( const K& key )
    {
        return false;
    }

    void clear()
    {
        return false;
    }

    size_t entries() const     { return m_elements;    }
    size_t maxEntries() const  { return m_maxElements; }

private:
    std::list<K>  m_uselist;
    std::map<K,V> m_elements;
    size_t m_maxElements;
};
