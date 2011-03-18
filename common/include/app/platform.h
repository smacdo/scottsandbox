#ifndef SCOTT_COMMON_APP_PLATFORM_H
#define SCOTT_COMMON_APP_PLATFORM_H

#ifdef _WIN32
#   define WINDOWS 1
#   define PLATFORM_DETECTED 1
    // Windows 32 or 64?
#   ifdef _WIN64
#       define PLATFORM_NAME "Windows 64"
#   else
#       define PLATFORM_NAME "Windows 32"
#   endif
#endif

#ifdef __APPLE__
#   define MAC 1
#   define OSX 1
#   define POSIX 1
#   define PLATFORM_DETECTED 1

#   define PLATFORM_NAME "Mac OSX"
#endif

#ifdef __linux
#   define LINUX 1
#   define POSIX 1
#   define PLATFORM_DETECTED 1
#   define PLATFORM_NAME "Generic Linux"
#endif

#ifdef __ANDROID__
#   error Android support not configured yet
#   define PLATFORM_DETECTED 1
#   define PLATFORM_NAME "Android"
#endif

#ifdef macintosh
#   error Mac OS9 not supported
#   define PLATFORM_DETECTED 1
#   define PLATFORM_NAME "Mac OS 9"
#endif

#if PLATFORM_DETECTED != 1
#   error Your operating system was not detected. Cannot compile until fixed
#endif

#endif
