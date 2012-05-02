/*
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
#ifndef SCOTT_COMMON_ASSERT_H
#define SCOTT_COMMON_ASSERT_H

// Check for presence of the NDEBUG macro. If it exists, the built in
// assertion will be disabled. There's no good way to go about manually
// disabling the builtin assertion macro, so we must rely on NDEBUG turning
// it off.
//
// Long story short: This file's custom assert cannot coexist with builtin
// assert. Turn it off
#ifndef NDEBUG
#   error "Builtin assert(x) enabled. Disable it with NDEBUG for custom asserts"
#endif

// If the compiler's debug mode is enabled, then turn on assertions.
// TODO: Make this more robust
#if defined(_DEBUG)
#   define ASSERTS_ENABLED  1
#   define ASSERTS_VERIFY   1
#endif

//
// Custom assertion handling
//
#ifdef ASSERTS_ENABLED
#   include <app/platform.h>        // need to pull in assertion callback
#   define scott_assert(msg,cond)           \
    do                                      \
    {                                       \
        if ( !(cond) )                      \
        {                                   \
            if ( App::raiseAssertion(msg,#cond,__FILE__,__LINE__) == \
                 App::EAssertion_Halt )     \
                app_break;                  \
        }                                   \
    } while( 0 )
#   define assert2(expr,msg) scott_assert(msg,expr)
#   define assert_null(var) scott_assert("Pointer was expected to be null",#var##" == NULL")
#   define assert_notNull(var) scott_assert("Pointer was expected to be non-null",#var##" != NULL")

#   ifdef ASSERTS_VERIFY        // only enabled for full sanity checks
#       define verify(expression)  scott_assert(NULL,expression)
#   else
#       define verify(expression)  do { (void)sizeof(x); } while(0)
#   endif
#   ifdef assert
#       undef assert
#   endif
#   define assert(x) scott_assert(NULL,x)
#else
#   ifndef assert
#       define assert(x)          do { (void)sizeof(x); } while(0)
#   endif
#   define assert2(x,m)       do { (void)sizeof(x); } while(0)
#   define assert_null(x)     do { (void)sizeof(x); } while(0)
#   define assert_notNull(x)  do { (void)sizeof(x); } while(0)
#   define verify(x)          do { (void)sizeof(x); } while(0)
#endif

#endif
