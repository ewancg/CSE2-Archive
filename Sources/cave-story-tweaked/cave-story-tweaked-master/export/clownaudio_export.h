
#ifndef CLOWNAUDIO_EXPORT_H
#define CLOWNAUDIO_EXPORT_H

#ifdef CLOWNAUDIO_STATIC_DEFINE
#  define CLOWNAUDIO_EXPORT
#  define CLOWNAUDIO_NO_EXPORT
#else
#  ifndef CLOWNAUDIO_EXPORT
#    ifdef clownaudio_EXPORTS
        /* We are building this library */
#      define CLOWNAUDIO_EXPORT 
#    else
        /* We are using this library */
#      define CLOWNAUDIO_EXPORT 
#    endif
#  endif

#  ifndef CLOWNAUDIO_NO_EXPORT
#    define CLOWNAUDIO_NO_EXPORT 
#  endif
#endif

#ifndef CLOWNAUDIO_DEPRECATED
#  define CLOWNAUDIO_DEPRECATED __declspec(deprecated)
#endif

#ifndef CLOWNAUDIO_DEPRECATED_EXPORT
#  define CLOWNAUDIO_DEPRECATED_EXPORT CLOWNAUDIO_EXPORT CLOWNAUDIO_DEPRECATED
#endif

#ifndef CLOWNAUDIO_DEPRECATED_NO_EXPORT
#  define CLOWNAUDIO_DEPRECATED_NO_EXPORT CLOWNAUDIO_NO_EXPORT CLOWNAUDIO_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CLOWNAUDIO_NO_DEPRECATED
#    define CLOWNAUDIO_NO_DEPRECATED
#  endif
#endif

#endif /* CLOWNAUDIO_EXPORT_H */
