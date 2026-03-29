/*
* Copyright (c) 2025-present Wojciech Kaptur ( _Sqyd_ / Sqydev )
* Github: https://github.com/Sqydev
* GPG Fingerprint: 6DC2516B0DFDA9C59661650722F7B8A777F33B56
* 
* This software is provided "as-is", without any express or implied warranty. In no event
* will the authors be held liable for any damages arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose, including commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
* 
* 1. Non-Misrepresentation: The origin of this software must not be misrepresented; 
*    you must not claim that you wrote the original software. An acknowledgment in 
*    product documentation is appreciated but not required.
* 
* 2. Source-Level Copyleft: Any altered versions (forks) of this software's source code, 
*    or files containing significant portions of this code, must be distributed under 
*    these same license terms. Such modified source code must be made publicly available 
*    to any recipient, even if used over a network (SaaS).
* 
* 3. Proprietary Integration: This software may be integrated into, linked with, or 
*    used as a component of proprietary and closed-source products. In such cases, 
*    the surrounding proprietary application code does not need to be disclosed, 
*    provided that the original or modified source code of THIS software remains 
*    available under the terms of Section 2.
* 
* 4. Persistent Metadata: All original credits, including those in the source code headers 
*    and binary metadata (e.g., ELF .comment section, PE StringFileInfo, or equivalent), 
*    must not be removed. You may add your own credits to forks, provided the original 
*    authorship remains clearly identified.
* 
* 5. Notice Retention: This license notice may not be removed or altered from any 
*    source or binary distribution.
*/

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

// NOTE: I think it makes c++ work
#ifdef __cplusplus
extern "C" {
#endif

#ifndef RLAPI
    #define RLAPI
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#ifndef PI
	#define PI 3.14159265358979323846f
#endif

#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif

#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	bool trueColor;
} Color;

typedef struct {
	char Char[4];
	char CharLen;

	Color fgColor;
	Color bgColor;
} SBCell;

typedef struct {
	float x;
	float y;
} Vecrot2;

typedef struct {
	int x;
	int y;
} Vector2i;

typedef struct {
	double x;
	double y;
} Vector2d;

typedef struct {
	long x;
	long y;
} Vector2l;

typedef struct {
	float x;
	float y;
	float width;
	float height;
} Rectangle;

typedef struct {
	int x;
	int y;
	int width;
	int height;
} Rectanglei;

typedef struct {
	double x;
	double y;
	double width;
	double height;
} Rectangled;

typedef struct {
	long x;
	long y;
	long width;
	long height;
} Rectanglel;

typedef enum {
	TUI_STATIC,
	TUI_DYNAMIC
} TuiType;

// ECORE

RLAPI void InitTui(int targetFps, TuiType type);
RLAPI void CloseTui(void);

RLAPI void BeginDrawing(void);
RLAPI void EndDrawing(void);

RLAPI void ClearTui(Color BgColor, Color FgColor);

// ESIGNALS

RLAPI int EnableSignal(int signal);
RLAPI int DisableSignal(int signal);
RLAPI int EnableSignalESClibTasks(int signal);
RLAPI int DisableSignalESClibTasks(int signal);
RLAPI int EnableSignalCustomTasks(int signal);
RLAPI int DisableSignalCustomTasks(int signal);
RLAPI int EnableSignalBuildInTasks(int signal);
RLAPI int DisableSignalBuildInTasks(int signal);

// Add Custom Task To Signal Tasks. If index < 0 Than It Will Add Task To The End. Will Return Index Of Added Task And -1 if failed
RLAPI int AddSignalTask(int signal, void (*taskFunction)(void), int index);
// Remove Custom Signal Task From Signal. If index < 0 Than It Will Remove Task From The End. Will Return 0 If Removed Succesfuly And -1 Of Failed
RLAPI int RemoveSignalTask(int signal, int index);
// Compress Signal Tasks. By This I Mean If CustomTask = 1,NULL,...,6 Than CompressSignalTasks() Will Make It 1,6 To Reduce Memory Usadge :). If from < 0 Than It Will Compress From The Start And If to < 0 Than It Will Compress To The End Will Return New TaskCount If Compresson Was Succesful And -1 If Failed
RLAPI int CompressSignalTasks(int signal, int from, int to);

// ESETS

RLAPI void SetTargetFps(int targetFps);
RLAPI void SetTuiDimentions(int x, int y);

// EGETS

// Returns last index of tui buff. So GetLastTuiIndex().x for TUI with GetTuiDimensions().x == 10 is == 9
RLAPI Vector2i GetLastTuiIndex(void);
// Same as GetLastTuiIndex() but returns pointer to the variable so you don't have to check for resize or smf
RLAPI Vector2i* GetLastTuiIndexPtr(void);
// Returns the number of places in TUI. DO NOT confuse with GetLastTuiIndex() becouse 1 is not first place but secound(arrays)
RLAPI Vector2i GetTuiDimensions(void);
// Same as GetTuiDimensions() but returns pointer to the variable so you don't have to check for resize or smf
RLAPI Vector2i* GetTuiDimensionsPtr(void);
// Same as GetTuiDimensions() but in pixels
RLAPI Vector2i GetTuiDimensionsInPixels(void);
// Same as GetTuiDimensionsInPixels() but returns pointer to the variable so you don't have to check for resize or smf
RLAPI Vector2i* GetTuiDimensionsPtrInPixels(void);

// Returns last index of screen. So GetLastTerminalIndex().x for terminal with GetTerminalDimensions().x == 10 is == 9
RLAPI Vector2i GetLastTerminalIndex(void);
// Same as GetLastTerminalIndex() but returns pointer to the variable so you don't have to check for resize or smf
RLAPI Vector2i* GetLastTerminalIndexPtr(void);
// Returns the number of places in terminal. DO NOT confuse with GetLastTerminalIndex() becouse 1 is not first place but secound(arrays)
RLAPI Vector2i GetTerminalDimensions(void);
// Same as GetTerminalDimensions() but returns pointer to the variable so you don't have to check for resize or smf
RLAPI Vector2i* GetTerminalDimensionsPtr(void);
// Same as GetTerminalDimensions() but in pixels
RLAPI Vector2i GetTerminalDimensionsInPixels(void);
// Same as GetTerminalDimensionsInPixels() but returns pointer to the variable so you don't have to check for resize or smf
RLAPI Vector2i* GetTerminalDimensionsPtrInPixels(void);
// Same as GetTerminalDimensions() but it acualy checks
Vector2i GetTerminalDimensionsForReal(void);
// Same as GetTerminalDimensionsInPixels() but it accualy checks
Vector2i GetTerminalDimensionsInPixelsForReal(void);

RLAPI size_t GetBackbuffSize(void);
RLAPI size_t GetBackbuffCellCount(void);
RLAPI size_t GetCharbuffSize(void);
RLAPI size_t GetCharbuffCellCount(void);

// Will Return Pointer To DATA.SignalData.SIGNAL.customTasks. If Failed Than It Will Return NULL
RLAPI void (**GetCustomSignalTasks(int signal))(void);
// Get Custom Signal Tasks Count. If Failed Than It Will Return -1
RLAPI int GetCustomSignalTasksCount(int signal);
// Get Custom Signal Tasks Size In Bytes I Think. If Failed Than It Will Return -1
RLAPI size_t GetCustomSignalTasksSize(int signal);

RLAPI double GetTime(void);

// EUTILS

RLAPI void ESleep(unsigned long sec, unsigned long ms, unsigned long ns);
RLAPI int GetCharWidth(const char* character);

// ETEXT

RLAPI void DrawCharV(const char* character, Vector2i pos, Color color);
RLAPI void DrawChar(const char* character, int x, int y, Color color);
RLAPI void DrawCharExV(const char* character, Vector2i pos, Color* fg, Color* bg);
RLAPI void DrawCharEx(const char* character, int x, int y, Color* fg, Color* bg);

RLAPI void DrawTextV(const char* text, Vector2i pos, Color color);
RLAPI void DrawText(const char* text, int x, int y, Color color);
RLAPI void DrawTextExV(const char* text, Vector2i pos, Color* fg, Color* bg);
RLAPI void DrawTextEx(const char* text, int x, int y, Color* fg, Color* bg);
RLAPI void DrawTextProV(const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spaceing, float angle);
RLAPI void DrawTextPro(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, float angle);

RLAPI void DrawTextfV(const char* text, Vector2i pos, Color color, ...);
RLAPI void DrawTextf(const char* text, int x, int y, Color color, ...);
RLAPI void DrawTextfExV(const char* text, Vector2i pos, Color* fg, Color* bg, ...);
RLAPI void DrawTextfEx(const char* text, int x, int y, Color* fg, Color* bg, ...);
RLAPI void DrawTextfProV(const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spaceing, double angle, ...);
RLAPI void DrawTextfPro(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, double angle, ...);

// ESHAPES

RLAPI void DrawRectangleRec(Rectanglei rec, Color color);
RLAPI void DrawRectangleV(Vector2i pos, Vector2i dimms, Color color);
RLAPI void DrawRectangle(int x, int y, int width, int height, Color color);
RLAPI void DrawRectangleExRec(char* character, Rectanglei rec, Color* fg, Color* bg);
RLAPI void DrawRectangleExV(char* character, Vector2i pos, Vector2i dimms, Color* fg, Color* bg);
RLAPI void DrawRectangleEx(char* character, int x, int y, int width, int height, Color* fg, Color* bg);
RLAPI void DrawRectangleProRec(char* character, Rectanglei rec, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness);
RLAPI void DrawRectangleProV(char* character, Vector2i pos, Vector2i dimms, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness);
RLAPI void DrawRectanglePro(char* character, int x, int y, int width, int height, int originX, int originY, Color* fg, Color* bg, double rotation, float roundness);

#ifdef __cplusplus
}
#endif

#endif
