#ifndef PX_PLATFORM_H
#define PX_PLATFORM_H

#ifdef _WIN32
#   ifdef __LIBPX__
#      define PXAPI __declspec(dllexport)
#   else
#      define PXAPI __declspec(dllimport)
#   endif
#elif __unix__
#    define PXAPI __attribute__((visibility("default")))
#endif


#endif // PLATFORM_H
