#ifndef SCOTT_COMMON_STATIC_ASSERT_H
#define SCOTT_COMMON_STATIC_ASSERT_H

#define static_assert_x(predicate,file) SASSERT_LINE(predicate,__LINE__,file)
#define static_assert(predicate,msg) SASSERT_LINE(predicate,__LINE__,__FILE__)

#define SASSERT_PASTE(a,b) a##b
#define ASSERT_LINE(predicate,line,file) \
 typedef char SASSERT_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];

#endif
