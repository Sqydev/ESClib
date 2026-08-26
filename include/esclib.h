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
#include <stdint.h>

#ifndef PI
	#define PI 3.14159265358979323846f
#endif

#ifndef DEG2RAD
	#define DEG2RAD (PI/180.0f)
#endif

#ifndef RAD2DEG
	#define RAD2DEG (180.0f/PI)
#endif

// If trueColor == true then it will do normal rgba. If trueColor == false then it will sum rgb and choose 8, 16 or 256 color pallete depending on the number and alpha will become 1 or 0. Also, beware because terminals are wierd with non-trueColor fg on bg
typedef struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	bool trueColor;
} Color;



typedef struct SBCell {
	char Char[4];
	char CharLen;

	Color fgColor;
	Color bgColor;
} SBCell;



typedef struct Vector2i {
	int x;
	int y;
} Vector2i;

typedef struct Vector2 {
	float x;
	float y;
} Vector2;

typedef struct Vector2d {
	double x;
	double y;
} Vector2d;

typedef struct Vector2l {
	long x;
	long y;
} Vector2l;



typedef struct Vector3i {
	int x;
	int y;
	int z;
} Vector3i;

typedef struct Vector3 {
	float x;
	float y;
	float z;
} Vector3;

typedef struct Vector3d {
	double x;
	double y;
	double z;
} Vector3d;

typedef struct Vector3l {
	long x;
	long y;
	long z;
} Vector3l;



typedef struct Vector4i {
	int x;
	int y;
	int z;
	int w;
} Vector4i;

typedef struct Vector4 {
	float x;
	float y;
	float z;
	float w;
} Vector4;

typedef struct Vector4d {
	double x;
	double y;
	double z;
	double w;
} Vector4d;

typedef struct Vector4l {
	long x;
	long y;
	long z;
	long w;
} Vector4l;



typedef struct Rectangle {
	int x;
	int y;
	int width;
	int height;
} Rectangle;

typedef struct Rectanglef {
	float x;
	float y;
	float width;
	float height;
} Rectanglef;

typedef struct Rectangled {
	double x;
	double y;
	double width;
	double height;
} Rectangled;

typedef struct Rectanglel {
	long x;
	long y;
	long width;
	long height;
} Rectanglel;



typedef struct Circle {
	int centerX;
	int centerY;
	int radius;
} Circle;

typedef struct Circlef {
	float centerX;
	float centerY;
	float radius;
} Circlef;

typedef struct Circled {
	double centerX;
	double centerY;
	double radius;
} Circled;

typedef struct Circlel {
	long centerX;
	long centerY;
	long radius;
} Circlel;



typedef struct Triangle {
	Vector2i A;
	Vector2i B;
	Vector2i C;
} Triangle;

typedef struct Trianglef {
	Vector2 A;
	Vector2 B;
	Vector2 C;
} Trianglef;

typedef struct Triangled {
	Vector2d A;
	Vector2d B;
	Vector2d C;
} Triangled;

typedef struct Trianglel {
	Vector2l A;
	Vector2l B;
	Vector2l C;
} Trianglel;



typedef struct Panel {
	int x;
	int y;
	int width;
	int height;
} Panel;



typedef enum TextureType {
	TEXTURE_MONO,
	TEXTURE_8COLOR,
    TEXTURE_16COLOR,
    TEXTURE_256COLOR,
    TEXTURE_TRUECOLOR,
} TextureType;


typedef enum ScalingAlgorithms {
	SCALEING_NEAREST_NEIGHBOR,
	SCALEING_BILINEAR,
	SCALEING_BICUBIC,
} ScalingAlgorithms;


typedef struct Texture {
	unsigned char* data;
	TextureType type;

	size_t id;
	size_t width;
	size_t height;
} Texture;
typedef Texture Texture2D;



typedef enum TuiType{
	TUI_STATIC,
	TUI_DYNAMIC
} TuiType;

typedef struct Kernel Kernel;



typedef int32_t EscKey;
typedef enum SpecialKeys {
	KEY_NULL = 0,

	KEY_ESCAPE    = 0x110001,
    KEY_ENTER     = 0x110002,
    KEY_TAB       = 0x110003,
    KEY_BACKSPACE = 0x110004,
    KEY_INSERT    = 0x110005,
    KEY_DELETE    = 0x110006,
    KEY_UP        = 0x110007,
    KEY_DOWN      = 0x110008,
    KEY_LEFT      = 0x110009,
    KEY_RIGHT     = 0x11000A,
    KEY_HOME      = 0x11000B,
    KEY_END       = 0x11000C,
    KEY_PAGE_UP   = 0x11000D,
    KEY_PAGE_DOWN = 0x11000E,
    KEY_F1        = 0x11000F,
	KEY_F2        = 0x11001A,
	KEY_F3        = 0x11001B,
	KEY_F4        = 0x11001C,
	KEY_F5        = 0x11001D,
	KEY_F6        = 0x11001E,
	KEY_F7        = 0x11001F,
	KEY_F8        = 0x11002A,
	KEY_F9        = 0x11002B,
	KEY_F10       = 0x11003C,
	KEY_F11       = 0x11004D,
	KEY_F12       = 0x11005E,
} SpecialKeys;



typedef enum LogLevel {
	LOG_ERROR = 3,
	LOG_WARNING = 2,
	LOG_INFO = 1,
	LOG_DEBUG = 0,
} LogLevel;



#define TERMWHITE (Color){ 15, 0, 0, true, false }
#define TERMBLACK (Color){ 0, 0, 0, true, false }
#define TERMBLANK (Color){ 0, 0, 0, false, false }

#define WHITE (Color){ 255, 0, 0, true, false }
#define BLACK (Color){ 232, 0, 0, true, false }
#define BLANK (Color){ 232, 0, 0, false, false }

#define TRUEWHITE (Color){ 255, 255, 255, 255, true }
#define TRUEBLACK (Color){ 0, 0, 0, 255, true }
#define TRUEBLANK (Color){ 0, 0, 0, 0, true }


#define WHOLETEXTURE(texture) (Rectangle){ 0, 0, texture.width, texture.height }


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

RLAPI void BeginFrame(void);
RLAPI void EndFrame(void);

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

RLAPI void BlendColors(Color* dst, Color src);

// ETEXT

RLAPI void DrawCharV(const char* character, Vector2i pos, Color color);
RLAPI void DrawChar(const char* character, int x, int y, Color color);
RLAPI void DrawCharExV(const char* character, Vector2i pos, Color* fg, Color* bg);
RLAPI void DrawCharEx(const char* character, int x, int y, Color* fg, Color* bg);
RLAPI void DrawCharCore(const char* character, int x, int y, Color* fg, Color* bg, bool isPanel, Panel panel);

RLAPI void DrawTextV(const char* text, Vector2i pos, Color color);
RLAPI void DrawText(const char* text, int x, int y, Color color);
RLAPI void DrawTextExV(const char* text, Vector2i pos, Color* fg, Color* bg);
RLAPI void DrawTextEx(const char* text, int x, int y, Color* fg, Color* bg);
RLAPI void DrawTextProV(const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spaceing, float angle);
RLAPI void DrawTextPro(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, float angle);
RLAPI void DrawTextCore(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, float angle, bool isPanel, Panel panel);

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
RLAPI void vaDrawTextfCore(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spacing, double angle, bool isPanel, Panel panel, va_list va);

// ESHAPES

RLAPI void DrawLineV(Vector2i pointA, Vector2i pointB, Color color, int thickness);
RLAPI void DrawLine(int pointAX, int pointAY, int pointBX, int pointBY, Color color, int thickness);
RLAPI void DrawLineExV(char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineEx(char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineProV(char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness);
RLAPI void DrawLinePro(char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness);
RLAPI void DrawLineCore(char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness, bool isPanel, Panel panel);

RLAPI void DrawRectangleRec(Rectangle rec, Color color);
RLAPI void DrawRectangleV(Vector2i pos, Vector2i dimms, Color color);
RLAPI void DrawRectangle(int x, int y, int width, int height, Color color);
RLAPI void DrawRectangleExRec(char* character, Rectangle rec, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawRectangleExV(char* character, Vector2i pos, Vector2i dimms, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawRectangleEx(char* character, int x, int y, int width, int height, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawRectangleProRec(char* character, Rectangle rec, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawRectangleProV(char* character, Vector2i pos, Vector2i dimms, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawRectanglePro(char* character, int posX, int posY, int width, int height, int originX, int originY, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawRectangleCore(char* character, int posX, int posY, int width, int height, int originX, int originY, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied, bool isPanel, Panel panel);

RLAPI void DrawCircleCir(Circle circle, Color color);
RLAPI void DrawCircleV(Vector2i centerPos, int radius, Color color);
RLAPI void DrawCircle(int centerX, int centerY, int radius, Color color);
RLAPI void DrawCircleExCir(char* character, Circle circle, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawCircleExV(char* character, Vector2i centerPos, int radius, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawCircleEx(char* character, int centerX, int centerY, int radius, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawCircleProCir(char* character, Circle circle, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawCircleProV(char* character, Vector2i centerPos, int radius, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawCirclePro(char* character, int centerX, int centerY, int radius, double startAngle, double endAngle, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawCircleCore(char* character, int centerX, int centerY, int radius, double startAngle, double endAngle, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied, bool isPanel, Panel panel);

RLAPI void DrawTriangleTri(Triangle triangle, Color color);
RLAPI void DrawTriangleV(Vector2i posA, Vector2i posB, Vector2i posC, Color color);
RLAPI void DrawTriangle(int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color color);
RLAPI void DrawTriangleExTri(char* character, Triangle triangle, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawTriangleExV(char* character, Vector2i posA, Vector2i posB, Vector2i posC, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawTriangleEx(char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color* fg, Color* bg, bool lines, int thicknessLines);
RLAPI void DrawTriangleProTri(char* character, Triangle triangle, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawTriangleProV(char* character, Vector2i posA, Vector2i posB, Vector2i posC, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawTrianglePro(char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int originX, int originY, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied);
RLAPI void DrawTriangleCore(char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int originX, int originY, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied, bool isPanel, Panel panel);

// EINPUT

RLAPI void PressKey(EscKey key);
RLAPI bool IsKeyPressed(EscKey key);
RLAPI EscKey GetKeyPressed(void);
RLAPI EscKey WaitForKeyPress(void);
RLAPI void WaitForKeyPressAndRegister(void);

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

RLAPI void DrawTextfVP(Panel panel, const char* text, Vector2i pos, Color color, ...);
RLAPI void DrawTextfP(Panel panel, const char* text, int x, int y, Color color, ...);
RLAPI void DrawTextfExVP(Panel panel, const char* text, Vector2i pos, Color* fg, Color* bg, ...);
RLAPI void DrawTextfExP(Panel panel, const char* text, int x, int y, Color* fg, Color* bg, ...);
RLAPI void DrawTextfProVP(Panel panel, const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spaceing, double angle, ...);
RLAPI void DrawTextfProP(Panel panel, const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, double angle, ...);

RLAPI void vaDrawTextfVP(Panel panel, const char* text, Vector2i pos, Color color, va_list va);
RLAPI void vaDrawTextfP(Panel panel, const char* text, int x, int y, Color color, va_list va);
RLAPI void vaDrawTextfExVP(Panel panel, const char* text, Vector2i pos, Color* fg, Color* bg, va_list va);
RLAPI void vaDrawTextfExP(Panel panel, const char* text, int x, int y, Color* fg, Color* bg, va_list va);
RLAPI void vaDrawTextfProVP(Panel panel, const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spacing, double angle, va_list va);
RLAPI void vaDrawTextfProP(Panel panel, const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spacing, double angle, va_list va);

// ECURSOR
RLAPI void SetCursorPositionV(Vector2i pos);
RLAPI void SetCursorPosition(int posX, int posY);

RLAPI void MoveCursorV(Vector2i pos);
RLAPI void MoveCursor(int posX, int posY);

RLAPI void HideCursor(void);
RLAPI void ShowCursor(void);

// ETEXTURES
RLAPI Texture LoadTexture(const char* path, TextureType type);
RLAPI void FreeTexture(Texture* texture);

RLAPI void DrawTexture(Texture* texture, Rectangle rec);
RLAPI void DrawTextureEx(Texture* texture, char* character, Rectangle rec, Color tint, bool affectFg, bool affectBg);
// If texture.type != true color then it will only do nearest neighbour
RLAPI void DrawTexturePro(Texture* texture, char* character, Rectangle rec, Rectangle textureSlice, int originX, int originY, double rotation, Color tint, bool affectFg, bool affectBg, ScalingAlgorithms scaling, bool aspectRatiofied);
// If texture.type != true color then it will only do nearest neighbour
RLAPI void DrawTextureCore(Texture* texture, char* character, Rectangle rec, Rectangle textureSlice, int originX, int originY, double rotation, Color tint, bool affectFg, bool affectBg, ScalingAlgorithms scaling, bool aspectRatiofied, bool isPanel, Panel panel);

#ifdef __cplusplus
}
#endif

#endif
