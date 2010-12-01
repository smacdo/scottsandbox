#ifndef SCOTT_SANDBOX_DEBUG_ASSERT_H
#define SCOTT_SANDBOX_DEBUG_ASSERT_H

// http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/

namespace app
{
    enum AssertBehavior
    {
        Halt,
        Continue
    };

    typedef AssertBehavior (*AssertFailHandler)( const char * cond,
                                                 const char * msg,
                                                 const char * file,
                                                 const char * func,
                                                 int line );

    Handler getAssertHandler();
    void setAssertHandler( AssertHandler handler );

    void reportAssertFail( const char * condition,
                           const char * file,
                           const char * func,
                           int line );
    AssertBehavior raiseAssertFail();
}

#define UNUSED(x) do { (void)sizeof(x); } while(0)
#define DEBUG_BREAK __debugbreak()
#define DO_NOTHING(x) \
    do \
    {  \
        (void)sizeof(x); \
        __pragma(warning(push)) \
        __pragma(warning(disable:4127)) \
    } while(0) \
    __pragma(warning(pop))

#define UNUSED_VAR(var) DO_NOTHING(var)
#define UNUSED_ARG(var) DO_NOTHING(var)

#ifdef APP_ENABLE_ASSERT
#define ASSERT(cond)                                                 \
    do                                                               \
    {                                                                \
        if (!(cond))                                                 \
        {                                                            \
            app::reportAssertFail(#cond,__FILE__,__func__,__LINE__); \
            if ( app::raiseAssertFail() == AssertBehavior::Fail )    \
            {                                                        \
                DEBUG_BREAK;                                         \
            }                                                        \
        }                                                            \
    __pragma(warning(push))                                          \
    __pragma(warning(disable:4127))                                  \
    } while(0)                                                       \
    __pragma(warning(pop))

// Always fail assertion with message
#define ASSERT_FAIL(msg)
{

}

#else
#define ASSERT(x) DO_NOTHING(x)
#endif

#endif
