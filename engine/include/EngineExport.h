
#ifndef ENGINE_EXPORT_H
#define ENGINE_EXPORT_H

#ifdef ENGINE_STATIC_DEFINE
#  define ENGINE_API
#  define ENGINE_NO_EXPORT
#else
#  ifndef ENGINE_API
#    ifdef engine_EXPORTS
        /* We are building this library */
#      define ENGINE_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define ENGINE_API __declspec(dllimport)
#    endif
#  endif

#  ifndef ENGINE_NO_EXPORT
#    define ENGINE_NO_EXPORT 
#  endif
#endif

#ifndef ENGINE_DEPRECATED
#  define ENGINE_DEPRECATED __declspec(deprecated)
#endif

#ifndef ENGINE_DEPRECATED_EXPORT
#  define ENGINE_DEPRECATED_EXPORT ENGINE_API ENGINE_DEPRECATED
#endif

#ifndef ENGINE_DEPRECATED_NO_EXPORT
#  define ENGINE_DEPRECATED_NO_EXPORT ENGINE_NO_EXPORT ENGINE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef ENGINE_NO_DEPRECATED
#    define ENGINE_NO_DEPRECATED
#  endif
#endif

#endif /* ENGINE_EXPORT_H */
