
//
// Check if the compiler has defined any common debugging symbols.
// If it has, then we'll automatically turn on debugging mode in the
// application
//
#if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#   ifndef NDEBUG
#       define APP_DEBUG 1
#   endif
#endif
