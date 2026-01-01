#ifndef ESCLIB_H
#define ESCLIB_H

#define ESCLIB_VERSION_MAJOR 0
#define ESCLIB_VERSION_MINOR 1
#define ESCLIB_VERSION_PATCH 0

// NOTE: Thoes library specyfiers
#if defined(_WIN32) || defined (_WIN64)
	#if defined(__TINYC__)
		#define __declspec(x) __attribute__((x))
	#endif
	#if defined(BUILD_LIBTYPE_SHARED)
		#define RLAPI __declspec(dllexport)
	#elif defined(USE_LIBTYPE_SHARED)
		#define RLAPI __declspec(dllimport)
	#endif
#else
	#if defined(BUILD_LIBTYPE_SHARED)
		#define RLAPI __attribute__((visibility("default")))
	#endif
#endif

#ifndef RLAPI
    #define RLAPI
#endif

typedef enum TuiType {
	TUI_STATIC,
	TUI_DYNAMIC,
} TuiType;

RLAPI void InitTui(int width, int height, int targetFps, TuiType tuiType);
RLAPI void CloseTui(void);

#endif
