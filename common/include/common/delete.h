#ifndef SCOTT_COMMON_DELETE_H
#define SCOTT_COMMON_DELETE_H

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
