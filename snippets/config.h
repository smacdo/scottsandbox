
/**
 * General debugging mode. This will be used to set up other options
 *
 *  0: "relase"     This is the mode where all optimizations turned on
 *  1: "debugrel"   Basic optimizations with debug symbols.
 *  2: "debug"      Standard developer mode
 *  3: "debugtrace" Developer mode with lots of extras turned on
 */
#ifndef APP_DEBUG_MODE
#define APP_DEBUG_MODE 2
#endif

#ifndef APP_PROFILING
#   if APP_DEBUG_MODE > 1
#       define APP_PROFILING 1
#   else
#       define APP_PROFILING 0
#   endif
#endif

/**
 * How are assertions handled?
 *  0: assertions are logged (if logging is turned on), but otherwise
 *     ignored
 *  1: Assertions trigger an exception
 *  2: Assertions trigger error and exit
 */
#ifndef APP_ASSERT_MODE
#   if APP_DEBUG_MODE == 0
#       define APP_ASSERT_MODE 1
#   else
#       define APP_ASSERT_MODE 2
#   endif
#endif

#ifndef APP_DOUBLE_PRECISION
#define APP_DOUBLE_PRECISION 0
#endif

#ifndef APP_LOGGING
#   if APP_DEBUG_MODE == 0
#       define APP_LOGGING 0
#       define APP_LOGGING_DEFAULT_LEVEL LOG_FATAL
#       define APP_LOGGING_DIR logs
#   else
#       define APP_LOGGING 1
#       define APP_LOGGING_DEFAULT_LEVEL LOG_INFO
#       define APP_LOGGING_DIR   logs
#   endif
#endif
