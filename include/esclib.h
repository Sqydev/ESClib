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
*	you must not claim that you wrote the original software. An acknowledgment in 
*	product documentation is appreciated but not required.
* 
* 2. Source-Level Copyleft: Any altered versions (forks) of this software's source code, 
*	or files containing significant portions of this code, must be distributed under 
*	these same license terms. Such modified source code must be made publicly available 
*	to any recipient, even if used over a network (SaaS).
* 
* 3. Proprietary Integration: This software may be integrated into, linked with, or 
*	used as a component of proprietary and closed-source products. In such cases, 
*	the surrounding proprietary application code does not need to be disclosed, 
*	provided that the original or modified source code of THIS software remains 
*	available under the terms of Section 2.
* 
* 4. Persistent Metadata: All original credits, including those in the source code headers 
*	and binary metadata (e.g., ELF .comment section, PE StringFileInfo, or equivalent), 
*	must not be removed. You may add your own credits to forks, provided the original 
*	authorship remains clearly identified.
* 
* 5. Notice Retention: This license notice may not be removed or altered from any 
*	source or binary distribution.
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
#include <stdint.h>
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
	float x;
	float y;
	float z;
} Vector3;

typedef struct {
	int x;
	int y;
	int z;
} Vector3i;

typedef struct {
	double x;
	double y;
	double z;
} Vector3d;

typedef struct {
	long x;
	long y;
	long z;
} Vector3l;



typedef struct {
	float x;
	float y;
	float z;
	float w;
} Vector4;

typedef struct {
	int x;
	int y;
	int z;
	int w;
} Vector4i;

typedef struct {
	double x;
	double y;
	double z;
	double w;
} Vector4d;

typedef struct {
	long x;
	long y;
	long z;
	long w;
} Vector4l;



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



typedef struct {
	Vector2i A;
	Vector2i B;
	Vector2i C;
} Triangle;

typedef struct {
	Vector2 A;
	Vector2 B;
	Vector2 C;
} Trianglef;

typedef struct {
	Vector2d A;
	Vector2d B;
	Vector2d C;
} Triangled;

typedef struct {
	Vector2l A;
	Vector2l B;
	Vector2l C;
} Trianglel;



typedef struct {
	int x;
	int y;
	int width;
	int height;
} Panel;



typedef enum {
	TUI_STATIC,
	TUI_DYNAMIC
} TuiType;

typedef struct Kernel Kernel;

#define ESC_KEYMAX 139

typedef enum {
	KEY_NULL            = 0,
	KEY_APOSTROPHE      = 40,
	KEY_COMMA           = 51,
	KEY_MINUS           = 12,
	KEY_PERIOD          = 52,
	KEY_SLASH           = 53,
	KEY_ZERO            = 11,
	KEY_ONE             = 2,
	KEY_TWO             = 3,
	KEY_THREE           = 4,
	KEY_FOUR            = 5,
	KEY_FIVE            = 6,
	KEY_SIX             = 7,
	KEY_SEVEN           = 8,
	KEY_EIGHT           = 9,
	KEY_NINE            = 10,
	KEY_SEMICOLON       = 39,
	KEY_EQUAL           = 13,
	KEY_A               = 30,
	KEY_B               = 48,
	KEY_C               = 46,
	KEY_D               = 32,
	KEY_E               = 18,
	KEY_F               = 33,
	KEY_G               = 34,
	KEY_H               = 35,
	KEY_I               = 23,
	KEY_J               = 36,
	KEY_K               = 37,
	KEY_L               = 38,
	KEY_M               = 50,
	KEY_N               = 49,
	KEY_O               = 24,
	KEY_P               = 25,
	KEY_Q               = 16,
	KEY_R               = 19,
	KEY_S               = 31,
	KEY_T               = 20,
	KEY_U               = 22,
	KEY_V               = 47,
	KEY_W               = 17,
	KEY_X               = 45,
	KEY_Y               = 21,
	KEY_Z               = 44,
	KEY_LEFT_BRACKET    = 26,
	KEY_BACKSLASH       = 43,
	KEY_RIGHT_BRACKET   = 27,
	KEY_GRAVE           = 41,
	KEY_LEFT_BRACE      = 84,
    KEY_RIGHT_BRACE     = 85,
    KEY_DOT             = 86,

	KEY_SPACE           = 57,
	KEY_ESCAPE          = 1,
	KEY_ENTER           = 28,
	KEY_TAB             = 15,
	KEY_BACKSPACE       = 14,
	KEY_INSERT          = 110,
	KEY_DELETE          = 111,
	KEY_RIGHT           = 106,
	KEY_LEFT            = 105,
	KEY_DOWN            = 108,
	KEY_UP              = 103,
	KEY_PAGE_UP         = 104,
	KEY_PAGE_DOWN       = 109,
	KEY_HOME            = 102,
	KEY_END             = 107,
	KEY_CAPS_LOCK       = 58,
	KEY_SCROLL_LOCK     = 70,
	KEY_NUM_LOCK        = 69,
	KEY_PRINT_SCREEN    = 99,
	KEY_PAUSE           = 119,
	KEY_PAGEUP          = 101,
	KEY_PAGEDOWN        = 121,
	
	KEY_F1              = 59,
	KEY_F2              = 60,
	KEY_F3              = 61,
	KEY_F4              = 62,
	KEY_F5              = 63,
	KEY_F6              = 64,
	KEY_F7              = 65,
	KEY_F8              = 66,
	KEY_F9              = 67,
	KEY_F10             = 68,
	KEY_F11             = 87,
	KEY_F12             = 88,

	KEY_MOD_SHIFT       = 42,
	KEY_LEFT_CONTROL    = 29,
	KEY_LEFT_ALT        = 56,
	KEY_LEFT_SUPER      = 125,
	KEY_RIGHT_CONTROL   = 97,
	KEY_RIGHT_ALT       = 100,
	KEY_RIGHT_SUPER     = 126,
	KEY_MENU            = 139,

	KEY_0               = 82,
	KEY_1               = 79,
	KEY_2               = 80,
	KEY_3               = 81,
	KEY_4               = 75,
	KEY_5               = 76,
	KEY_6               = 77,
	KEY_7               = 71,
	KEY_8               = 72,
	KEY_9               = 73,
	KEY_DECIMAL         = 83,
	KEY_DIVIDE          = 98,
	KEY_MULTIPLY        = 55,
	KEY_SUBTRACT        = 74,
	KEY_ADD             = 78,
} KeyboardKey;



typedef enum {
	LOG_ERROR = 3,
	LOG_WARNING = 2,
	LOG_INFO = 1,
	LOG_DEBUG = 0,
} LogLevel;



#define TERMWHITE (Color){ 15, 0, 0, 0 }
#define TERMBLACK (Color){ 0, 0, 0, 0 }

#define WHITE (Color){ 255, 0, 0, 0 }
#define BLACK (Color){ 232, 0, 0, 0 }

#define TRUEWHITE (Color){ 255, 255, 255, 1 }
#define TRUEBLACK (Color){ 0, 0, 0, 1 }

// ECORE

RLAPI void InitTui(int targetFps, TuiType type);
RLAPI void CloseTui(void);

// Init logging to file of path(0 if succes -1 if falied, -2 if already inited)
RLAPI int InitLoggin(char* path, LogLevel logLevel);
// Log to inited log file
RLAPI void TraceLog(LogLevel logLevel, const char* message, ...);
// Close logging file
RLAPI void CloseLoggin();

// Will do panic thing and safley close the program with exitCode logging the message and doing added tasks(see AddPanicTask())
RLAPI void Panic(const char* message, int exitCode);
// Will add task to tasks that Panic() does. In order of first -> last(index = 0 -> index = last). Will return index of added task. If failed will return 0
RLAPI int AddPanicTask(void (*task)(void));
// Will return 0 if succesfull -1 if failed
RLAPI int RemovePanicTask(size_t index);

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

// Add Custom Task To Signal Tasks( tasks executed in order of first -> last(index = 0 -> index = last) ). If index < 0 Than It Will Add Task To The End. Will Return Index Of Added Task And -1 if failed
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

RLAPI double GetCurrentFrameTime(void);
RLAPI double GetPreviousFrameTime(void);
RLAPI double GetDeltaTime(void);
RLAPI double GetFrameTime(void);
RLAPI int GetTargetFps(void);
RLAPI double GetTargetTime(void);

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

RLAPI Vector2i GetCursorPos(void);
RLAPI bool IsCursorHidden(void);

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

RLAPI void vaDrawTextfV(const char* text, Vector2i pos, Color color, va_list va);
RLAPI void vaDrawTextf(const char* text, int x, int y, Color color, va_list va);
RLAPI void vaDrawTextfExV(const char* text, Vector2i pos, Color* fg, Color* bg, va_list va);
RLAPI void vaDrawTextfEx(const char* text, int x, int y, Color* fg, Color* bg, va_list va);
RLAPI void vaDrawTextfProV(const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spacing, double angle, va_list va);
RLAPI void vaDrawTextfPro(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spacing, double angle, va_list va);

// ESHAPES

RLAPI void DrawLineV(Vector2i pointA, Vector2i pointB, Color color, int thickness);
RLAPI void DrawLine(int pointAX, int pointAY, int pointBX, int pointBY, Color color, int thickness);
RLAPI void DrawLineExV(char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineEx(char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineProV(char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness);
RLAPI void DrawLinePro(char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness);

RLAPI void DrawRectangleRec(Rectangle rec, Color color);
RLAPI void DrawRectangleV(Vector2i pos, Vector2i dimms, Color color);
RLAPI void DrawRectangle(int x, int y, int width, int height, Color color);
RLAPI void DrawRectangleExRec(char* character, Rectangle rec, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawRectangleExV(char* character, Vector2i pos, Vector2i dimms, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawRectangleEx(char* character, int x, int y, int width, int height, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawRectangleProRec(char* character, Rectangle rec, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawRectangleProV(char* character, Vector2i pos, Vector2i dimms, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawRectanglePro(char* character, int posX, int posY, int width, int height, int originX, int originY, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied);

RLAPI void DrawCircleCir(Circle circle, Color color);
RLAPI void DrawCircleV(Vector2i centerPos, int radius, Color color);
RLAPI void DrawCircle(int centerX, int centerY, int radius, Color color);
RLAPI void DrawCircleExCir(char* character, Circle circle, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawCircleExV(char* character, Vector2i centerPos, int radius, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawCircleEx(char* character, int centerX, int centerY, int radius, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawCircleProCir(char* character, Circle circle, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawCircleProV(char* character, Vector2i centerPos, int radius, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawCirclePro(char* character, int centerX, int centerY, int radius, double startAngle, double endAngle, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied);

RLAPI void DrawTriangleTri(Triangle triangle, Color color);
RLAPI void DrawTriangleV(Vector2i posA, Vector2i posB, Vector2i posC, Color color);
RLAPI void DrawTriangle(int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color color);
RLAPI void DrawTriangleExTri(char* character, Triangle triangle, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawTriangleExV(char* character, Vector2i posA, Vector2i posB, Vector2i posC, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawTriangleEx(char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawTriangleProTri(char* character, Triangle triangle, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawTriangleProV(char* character, Vector2i posA, Vector2i posB, Vector2i posC, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawTrianglePro(char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int originX, int originY, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied);

// EINPUT

RLAPI void PressKey(int key);
RLAPI bool IsKeyPressed(int key);
RLAPI bool IsKeyDown(int key);

// OPENCL

// If You Want To Init Only OpenCL And Don't Need TUI You Can Use This
RLAPI void InitOpenCl(void);
RLAPI void CleanUpOpenCl(void);

// Compile Shader For Use, If Null Is Returned Than Shader Wasn't Compiled(User Propably Doesn't Have Your Lovely Grafiks API)(Out of the box ~/ handling)
RLAPI Kernel* CompileKernel(const char* path, const char* kernelName);
// Add Kernel Arg, Same As In OpenCL
RLAPI int AddKernelArgValue(Kernel* kernel, uint32_t index, size_t size, const void* data);
// Add Kernel Arg, Same As In OpenCL
RLAPI int AddKernelArgBuffer(Kernel* kernel, uint32_t index, size_t size, const void* data);

RLAPI int ReadKernelArg(Kernel* kernel, uint32_t index, size_t size, void* out);
// Run Kernel
RLAPI int RunKernel(Kernel* kernel, size_t instances, size_t workgroupSize);
// Wait For Kernel To Finish
RLAPI int WaitForKernel(void);
// Free The Kernel
RLAPI void DestroyKernel(Kernel* k);

// EBRAILLE

// IT WILL MALLOC
RLAPI char* MergeBraille(const char* merged, const char* merger);

// EPANELS
RLAPI void DrawCharVP(Panel panel, const char* character, Vector2i pos, Color color);
RLAPI void DrawCharP(Panel panel, const char* character, int x, int y, Color color);
RLAPI void DrawCharExVP(Panel panel, const char* character, Vector2i pos, Color* fg, Color* bg);
RLAPI void DrawCharExP(Panel panel, const char* character, int x, int y, Color* fg, Color* bg);

RLAPI void DrawTextVP(Panel panel, const char* text, Vector2i pos, Color color);
RLAPI void DrawTextP(Panel panel, const char* text, int x, int y, Color color);
RLAPI void DrawTextExVP(Panel panel, const char* text, Vector2i pos, Color* fg, Color* bg);
RLAPI void DrawTextExP(Panel panel, const char* text, int x, int y, Color* fg, Color* bg);
RLAPI void DrawTextProVP(Panel panel, const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spaceing, float angle);
RLAPI void DrawTextProP(Panel panel, const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, float angle);

RLAPI void DrawLineVP(Panel panel, Vector2i pointA, Vector2i pointB, Color color, int thickness);
RLAPI void DrawLineP(Panel panel, int pointAX, int pointAY, int pointBX, int pointBY, Color color, int thickness);
RLAPI void DrawLineExVP(Panel panel, char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineExP(Panel panel, char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineProVP(Panel panel, char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineProP(Panel panel, char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness);

RLAPI void DrawRectangleRecP(Panel panel, Rectangle rec, Color color);
RLAPI void DrawRectangleVP(Panel panel, Vector2i pos, Vector2i dimms, Color color);
RLAPI void DrawRectangleP(Panel panel, int x, int y, int width, int height, Color color);
RLAPI void DrawRectangleExRecP(Panel panel, char* character, Rectangle rec, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawRectangleExVP(Panel panel, char* character, Vector2i pos, Vector2i dimms, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawRectangleExP(Panel panel, char* character, int x, int y, int width, int height, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawRectangleProRecP(Panel panel, char* character, Rectangle rec, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawRectangleProVP(Panel panel, char* character, Vector2i pos, Vector2i dimms, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawRectangleProP(Panel panel, char* character, int posX, int posY, int width, int height, int originX, int originY, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied);

RLAPI void DrawCircleCirP(Panel panel, Circle circle, Color color);
RLAPI void DrawCircleVP(Panel panel, Vector2i centerPos, int radius, Color color);
RLAPI void DrawCircleP(Panel panel, int centerX, int centerY, int radius, Color color);
RLAPI void DrawCircleExCirP(Panel panel, char* character, Circle circle, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawCircleExVP(Panel panel, char* character, Vector2i centerPos, int radius, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawCircleExP(Panel panel, char* character, int centerX, int centerY, int radius, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawCircleProCirP(Panel panel, char* character, Circle circle, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawCircleProVP(Panel panel, char* character, Vector2i centerPos, int radius, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawCircleProP(Panel panel, char* character, int centerX, int centerY, int radius, double startAngle, double endAngle, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied);

RLAPI void DrawTriangleTriP(Panel panel, Triangle triangle, Color color);
RLAPI void DrawTriangleVP(Panel panel, Vector2i posA, Vector2i posB, Vector2i posC, Color color);
RLAPI void DrawTriangleP(Panel panel, int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color color);
RLAPI void DrawTriangleExTriP(Panel panel, char* character, Triangle triangle, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawTriangleExVP(Panel panel, char* character, Vector2i posA, Vector2i posB, Vector2i posC, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawTriangleExP(Panel panel, char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawTriangleProTriP(Panel panel, char* character, Triangle triangle, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawTriangleProVP(Panel panel, char* character, Vector2i posA, Vector2i posB, Vector2i posC, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawTriangleProP(Panel panel, char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int originX, int originY, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied);

// ECURSOR
RLAPI void SetCursorPositionV(Vector2i pos);
RLAPI void SetCursorPosition(int posX, int posY);

RLAPI void MoveCursorV(Vector2i pos);
RLAPI void MoveCursor(int posX, int posY);

RLAPI void HideCursor(void);
RLAPI void ShowCursor(void);

#ifdef __cplusplus
}
#endif

#endif
