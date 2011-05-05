/**
 * Basic utility functions that can be required all over the place. This
 * is not the place for more "specialized utility" (oxymoron?) functions
 * like string utilities.
 *
 * Keep it simple and easy to include
 */
#ifndef SCOTT_COMMON_UTILS_H
#define SCOTT_COMMON_UTILS_H

template<typename ContainerT>
deletePtrContainer( ContainerT& input )
{
    typename ContainerT::iterator itr;

    for ( itr = input.begin(); itr != itr.end(); ++itr )
    {
        delete *itr;
    }

    target.clear();
}

#endif
