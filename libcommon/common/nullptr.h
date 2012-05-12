#ifndef SCOTT_COMMON_UTIL_NULLPTR_H
#define SCOTT_COMMON_UTIL_NULLPTR_H

#include <memory>

/**
 * Nullptr is a very useful utility class that allows a programmer
 * to assign to a shared_ptr. You can accomplish this with an empty
 * constructor or a empty reset(), but using this class makes the
 * assignment more obvious.
 *
 * eg,
 * std::shared_ptr<int> p = NullPtr();
 */
class NullPtr
{
    /**
     * Conversion operator returning a null shared_ptr
     */
    template<typename T> operator std::shared_ptr<T>()
    {
        return std::shared_ptr<T>();
    }
};

#endif
