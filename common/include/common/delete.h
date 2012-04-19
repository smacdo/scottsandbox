/**
 * Copyright 2012 Scott MacDonald
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SCOTT_COMMON_DELETE_H
#define SCOTT_COMMON_DELETE_H

#include <algorithm>
#include <vector>
#include <map>

/**
 * Deletes a pointer and then sets the address to NULL to prevent accidental
 * dereferences
 *
 * \param pointer The pointer to delete
 */
template<typename T>
void Delete( T& pointer )
{
#ifdef USE_BOOST
    boost::checked_delete( pointer );
#else
    CheckedDelete( pointer );
#endif
    pointer = NULL;
}

/**
 * Deletes an array pointer, and then sets the address to NULL to prevent
 * accidental dereferences
 *
 * \param arrayPointer The array pointer to delete
 */
template<typename T>
void DeleteArray( T& arrayPointer )
{
#ifdef USE_BOOST
    boost::checked_array_delete( arrayPointer );
#else
    CheckedArrayDelete( arrayPointer );
#endif
    arrayPointer = NULL;
}

/**
 * Deletes a generic container of pointer values by iterating through the
 * list, deleting each pointer element and then setting the size of the container
 * to zero.
 *
 * \param  container  A generic container holding pointers
 */
template<typename T>
size_t DeletePointerContainer( T& container )
{
    typename T::iterator itr;
    size_t count = 0;

    for ( itr = container.begin(); itr != container.end(); std::advance(itr,1) )
    {
        Delete<typename T::value_type>( *itr );
        count++;
    }

    // Also reset the container to have zero entries
    container.clear();

    // Make sure caller knows how many objects were destroyed
    return count;
}

/**
 * Deletes an STL container of vectors, and resizes the container to zero.
 *
 * \param  container  A STL vector holding pointers
 */
template<typename T>
size_t DeleteVectorPointers( std::vector<T>& container )
{
    // Record how many objects are in the container before destroying
    size_t count = container.size();

    // Now destroy every pointer in the container, and then reset the container
    // to be empty
    std::for_each( container.begin(), container.end(), Delete<T> );
    container.clear();

    return count;
}

/**
 * Deletes an STL map of containers, and resizes the container to zero.
 *
 * \param  container  A STL map holding pointers
 */
template<typename T, typename U>
size_t DeleteMapPointers( std::map<T,U>& container )
{
    typename std::map<T,U>::iterator itr;
    size_t count = 0;

    for ( itr = container.begin(); itr != container.end(); ++itr )
    {
        Delete<U>( itr->second );
        count++;
    }

    // Reset the container to be empty, ensuring we don't accidently reference a
    // null pointer
    container.clear();

    return count;
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
