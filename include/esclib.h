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

#include <stddef.h>

typedef struct {
	float x;
	float y;
} Vecrot2;

typedef struct {
	int x;
	int y;
} Vector2i;

// ECORE

RLAPI void InitTui(int targetFps);
RLAPI void CloseTui(void);

RLAPI static inline size_t GetBackbuffSize(void);
RLAPI size_t WriteToBackbuff(const char* content);

RLAPI int EnableSignal(int signal);
RLAPI int DisableSignal(int signal);
RLAPI int EnableSignalESClibTasks(int signal);
RLAPI int DisableSignalESClibTasks(int signal);
RLAPI int EnableSignalCustomTasks(int signal);
RLAPI int DisableSignalCustomTasks(int signal);
RLAPI int EnableSignalBuildInTasks(int signal);
RLAPI int DisableSignalBuildInTasks(int signal);

// EGET
RLAPI Vector2i GetTuiDimmentions(void);
RLAPI Vector2i GetTuiDimmensionsInPixels(void);
RLAPI Vector2i* GetTuiDimmensionsPtr(void);
RLAPI Vector2i* GetTuiDimmensionsPtrInPixels(void);
RLAPI Vector2i GetTuiDimmentionsForReal(void);
RLAPI Vector2i GetTuiDimmensionsInPixelsForReal(void);

#endif
