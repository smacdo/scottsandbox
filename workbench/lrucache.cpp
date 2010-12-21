/**
 * Simple LRU cache class
 */
#include <iostream>

/**
 * LRU cache - description
 *
 * DOES *NOT* track pointer lifetime or delete
 */
template<typename Key, typename Value, size_t Size>
class LRUCache
{
    LRUCache()
        : m_index( 0 )
    {
    }

    /**
     * Resets the LRU cache. Removes all keys
     */
    void reset()
    {
        m_index = 0;
    }

    /**
     * Attempts to retrieve the given key from the LRU cache. If the
     * key exists in the cache, the corresponding value is returned. If
     * the key has expired or was never added, then NULL is returned.
     */
    Value* get( const Key& key )
    {
        return NULL;
    }

    /**
     * Checks if the cache contains the given key. True will be returned
     * if the key exists, false otherwise
     */
    bool contains( const Key& key )
    {
        return false;
    }

    /**
     * Removes the given key from the cache. Returns true if the key
     * existed and was removed, false otherwise.
     */
    bool remove( const Key& key )
    {
        return false;
    }

    /**
     * Returns the number of cache entries that are in this cache
     */
    size_t count() const
    {
        return 0;
    }

    /**
     * Returns the maximum number of entries that can be placed in the
     * cache
     */
    size_t maxEntries() const
    {
        return Size;
    }

private:
    struct CacheSlot
    {
        Key key;
        Value val;
    };

    size_t m_index;
    CacheSlot m_slots[Size];

    /**
     * Number of cache hits
     */
    size_t m_cacheHits;

    /**
     * Number of cache misses
     */
    size_t m_cacheMisses;

    /**
     * Number of elements removed from cache
     */
    size_t m_elementsAdded;
};
