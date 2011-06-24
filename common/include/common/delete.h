#ifndef SCOTT_COMMON_DELETE_H
#define SCOTT_COMMON_DELETE_H

/**
 * Deletes an STL container of pointers. Sets each deleted pointer
 * value to zero to indicate it has been deleted
 */
template<class Container>
inline void DeletePointerContainer( Container& pointers )
{
    // intentionally complex - according to boost code, simplifying this
    // causes certain compilers to optimize this out and thereby break
    // the functionality of CheckedDelete
    typedef char type_must_be_complete[ sizeof(Container::value_type) ? 1 : -1 ];
    (void) sizeof( type_must_be_complete );

    // now delete every pointer in the container
    typename Container::iterator itr;

    for ( itr = pointers.begin(); itr != pointers.end(); ++itr )
    {
        // delete the actual pointer
        delete (*itr);

        // set value to zero so we cannot dereference it again
        *itr = 0;
    }
}

/**
 * CheckedDelete - identical to boost's boost::checked_delete. Ensures
 * that the pointer being deleted belongs to a fully defined class
 */
template<class T>
inline void CheckedDelete( T *ptr )
{
    // intentionally complex - according to boost code, simplifying this
    // causes certain compilers to optimize this out and thereby break
    // the functionality of CheckedDelete
    typedef char type_must_be_complete[ sizeof(T) ? 1 : -1 ];
    (void) sizeof( type_must_be_complete );

    delete ptr;
}

template<class T>
inline void CheckedArrayDelete( T *ptr )
{
    typedef char type_must_be_complete[ sizeof(T) ? 1 : -1 ];
    (void) sizeof( type_must_be_complete );

    delete[] ptr;
}

#endif
