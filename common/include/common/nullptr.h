#ifndef SCOTT_SHAREDPTR_NULL_H
#define SCOTT_SHAREDPTR_NULL_H

#include <memory>

class NullPtr
{
    template<typename T> operator std::shared_ptr<T>()
    {
        return std::shared_ptr<T>();
    }
};

#endif
