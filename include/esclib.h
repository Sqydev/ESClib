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

// If trueColor == true, rgb is rgb if trueColor == false than the color == r + g + b. WARNING: Terminals have stupid thing that in trueColor if fg == 0, 0, 255 and bg == 255, 0, 0 than the terminal blends the fg to 255, 0, 255. If anyone knows how to turn this off than PLEASE tell me, but for now just keep that fact in mind
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
} Vector2;

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
	int x;
	int y;
	int width;
	int height;
} Rectangle;

typedef struct {
	float x;
	float y;
	float width;
	float height;
} Rectanglef;

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

typedef struct {
	int centerX;
	int centerY;
	int radius;
} Circle;

typedef struct {
	float centerX;
	float centerY;
	float radius;
} Circlef;

typedef struct {
	double centerX;
	double centerY;
	double radius;
} Circled;

typedef struct {
	long centerX;
	long centerY;
	long radius;
} Circlel;

typedef enum {
	TUI_STATIC,
	TUI_DYNAMIC
} TuiType;

#define TERMWHITE (Color){ 255, 0, 0, 0 }
#define TERMBLACK (Color){ 232, 0, 0, 0 }

#define WHITE (Color){ 255, 0, 0, 0 }
#define BLACK (Color){ 232, 0, 0, 0 }

#define TRUEWHITE (Color){ 255, 255, 255, 1 }
#define TRUEBLACK (Color){ 0, 0, 0, 1 }

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
RLAPI Vector2i GetTerminalDimensionsForReal(void);
// Same as GetTerminalDimensionsInPixels() but it accualy checks
RLAPI Vector2i GetTerminalDimensionsInPixelsForReal(void);

// Returns proportions of the cells
RLAPI Vector2i GetCellProportions(void);
// Return size of cells in pixels
RLAPI Vector2i GetCellSizeInPixels(void);
// Same as GetCellSizeProportions() but with ptr
RLAPI Vector2i* GetCellProportionsPtr(void);
// Same as GetCellSizeInPixels() but with ptr
RLAPI Vector2i* GetCellSizeInPixelsPtr(void);
// GetCellSizeProportions() for reals
RLAPI Vector2i GetCellProportionsForReal(void);
// GetCellSizeInPixels() for reals
RLAPI Vector2i GetCellSizeInPixelsForReal(void);

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

// Like. If you have a and b. And if you wanna get angle that a in going. Use this
RLAPI double CalculateAngleOfAGoingToB(Vector2 a, Vector2 b);

// Get sin() FAST. It checks for common angles and if it's f.e PI than it will return 0 without accualy calculating sin(). Will return -2 if error
RLAPI double ESin(double angle);
// Get cos() FAST. It checks for common angles and if it's f.e PI than it will return 1 without accualy calculating cos(). Will return -2 if error
RLAPI double ECos(double angle);
// Get tan() FAST. It checks for common angles and if it's f.e PI than it will return 0 without accualy calculating tan(). Will return -2 if error
RLAPI double ETan(double angle);
// Calculate dir vetor. Nice utility to have. Will return -2, -2 if error
RLAPI Vector2d EDir(double angle);

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

RLAPI void DrawLineV(Vector2i pointA, Vector2i pointB, Color color, int thickness);
RLAPI void DrawLine(int pointAX, int pointAY, int pointBX, int pointBY, Color color, int thickness);
RLAPI void DrawLineExV(char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineEx(char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineProV(char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness, bool aspectRatiofied);
RLAPI void DrawLinePro(char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness, bool aspectRatiofied);

RLAPI void DrawRectangleRec(Rectangle rec, Color color);
RLAPI void DrawRectangleV(Vector2i pos, Vector2i dimms, Color color);
RLAPI void DrawRectangle(int x, int y, int width, int height, Color color);
RLAPI void DrawRectangleExRec(char* character, Rectangle rec, Color* fg, Color* bg, bool lines);
RLAPI void DrawRectangleExV(char* character, Vector2i pos, Vector2i dimms, Color* fg, Color* bg, bool lines);
RLAPI void DrawRectangleEx(char* character, int x, int y, int width, int height, Color* fg, Color* bg, bool lines);
RLAPI void DrawRectangleProRec(char* character, Rectangle rec, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, bool aspectRatiofied);
RLAPI void DrawRectangleProV(char* character, Vector2i pos, Vector2i dimms, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, bool aspectRatiofied);
RLAPI void DrawRectanglePro(char* character, int x, int y, int width, int height, int originX, int originY, Color* fg, Color* bg, double rotation, float roundness, bool lines, bool aspectRatiofied);

RLAPI void DrawCircleCir(Circle circle, Color color);
RLAPI void DrawCircleV(Vector2i centerPos, int radius, Color color);
RLAPI void DrawCircle(int centerX, int centerY, int radius, Color color);
RLAPI void DrawCircleExCir(char* character, Circle circle, Color* fg, Color* bg, bool lines);
RLAPI void DrawCircleExV(char* character, Vector2i centerPos, int radius, Color* fg, Color* bg, bool lines);
RLAPI void DrawCircleEx(char* character, int centerX, int centerY, int radius, Color* fg, Color* bg, bool lines);
RLAPI void DrawCircleProCir(char* character, Circle circle, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, bool aspectRatiofied);
RLAPI void DrawCircleProV(char* character, Vector2i centerPos, int radius, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, bool aspectRatiofied);
RLAPI void DrawCirclePro(char* character, int centerX, int centerY, int radius, double startAngle, double endAngle, Color* fg, Color* bg, bool lines, bool aspectRatiofied);

/*
 * To make.
    void DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom);
    void DrawRectangleGradientH(int posX, int posY, int width, int height, Color left, Color right);
    void DrawRectangleLines(int posX, int posY, int width, int height, Color color);

	void DrawCircleLines(int centerX, int centerY, float radius, Color color); 
	void DrawCircleGradient(int centerX, int centerY, float radius, Color inner, Color outer);
	
	void DrawLineGradient(int pointAX, int pointAY, int pointBX, int pointBY, Color color, int thickness);
*/

#ifdef __cplusplus
}
#endif

#endif
