#ifndef ESCLIB_H
#define ESCLIB_H

#include <stdbool.h>
#include <stddef.h>

// NOTE: Order: Structs, macros, itp THEN functions

typedef struct color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} color;

typedef struct vector2 {
	float x;
	float y;
} vector2;

typedef struct vector3 {
	float x;
	float y;
	float z;
} vector3;

typedef struct vector4 {
	float x;
	float y;
	float z;
	float w;
} vector4;

typedef struct intvector2 {
	int x;
	int y;
} intvector2;

typedef struct intvector3 {
	int x;
	int y;
	int z;
} intvector3;

typedef struct intvector4 {
	int x;
	int y;
	int z;
	int w;
} intvector4;

typedef enum {
    STYLE_NORMAL        = 0,
    STYLE_BOLD          = 1,
    STYLE_DIMM          = 2,
    STYLE_ITALIC        = 3,
    STYLE_UNDERLINE     = 4,
    STYLE_BACKGROUND    = 7,
    STYLE_HIDDEN        = 8,
    STYLE_STRIKETHROUGH = 9
} fontStyle;



// eCore
void InitTui(int fps, bool DisableSignals);
void CloseTui(void);
void SetTargetFps(int fps);

void BeginDrawing(void);
void EndDrawing(void);

bool TuiShouldClose(void);
bool IsAlternativeBufferOn(void);
bool IsRawModeOn(void);

int GetTuiWidth(void);
int GetTuiHeight(void);
vector2 GetCursorPosition(void);
vector2 GetLockedCursorPosition(void);
color GetBackgroundColor(void);
color GetForegroundColor(void);
fontStyle GetForegroundStyle(void);
int GetKey(void);
double GetTime(void);

void SetCursorPosition(float x, float y);
void SetLockedCursorPosition(float x, float y);
void MoveCursorDirectional(float up, float down, float left, float right);
void MoveCursor(float x, float y);

void ShowCursor(void);
void HideCursor(void);
void LockCursor(void);
void UnlockCursor(void);

void SetBackgroundColor(color Color);
void SetForegroundColor(color Color);
void SetForegroundStyle(fontStyle Style);
void ClearBackground(color Color);

void ClearScreen(void);
void ClearLine(void);
void ClearChar(void);

// Super extra fns
void WriteToBackBuffor(const char* to_add, size_t lenght);

// Pretty useless fns for normal use
void ToggleBufferMode(void);
void EnableBufferMode(void);
void DisableBufferMode(void);



// eShapes
void DrawChar(const char character, int posX, int posY, fontStyle Style, color Color);
void DrawCharV(const char character, vector2 position, fontStyle Style, color Color);

void DrawTextRaw(const char* text, int posX, int posY, fontStyle Style, color Color, size_t lenght);
void DrawText(const char* text, int posX, int posY, fontStyle Style, color Color);
void DrawTextV(const char* text, vector2 position, fontStyle Style, color Color);

void DrawTextGradientRaw(const char* text, int posX, int posY, fontStyle Style, color Left, color Right, size_t lenght);
void DrawTextGradient(const char* text, int posX, int posY, fontStyle Style, color Left, color Right);
void DrawTextGradientV(const char* text, vector2 position, fontStyle Style, color Left, color Right);

void DrawRectangleRaw(const char character, int posX, int posY, int width, int height, fontStyle Style, color Color);
void DrawRectangle(int posX, int posY, int width, int height, fontStyle Style, color Color);
void DrawRectangleV(int posX, int posY, int width, int height, fontStyle Style, color Color);

void DrawRectangleGradientVerticalRaw(const char character, int posX, int posY, int width, int height, fontStyle Style, color Top, color Bottom);
void DrawRectangleGradientVertical(int posX, int posY, int width, int height, fontStyle Style, color Top, color Bottom);
void DrawRectangleGradientVerticalV(int posX, int posY, int width, int height, fontStyle Style, color Top, color Bottom);

void DrawRectangleGradientHorizontalRaw(const char character, int posX, int posY, int width, int height, fontStyle Style, color Left, color Right);
void DrawRectangleGradientHorizontal(int posX, int posY, int width, int height, fontStyle Style, color Left, color Right);
void DrawRectangleGradientHorizontalV(int posX, int posY, int width, int height, fontStyle Style, color Left, color Right);



// KEYS(by chatgbt becouse I'm not crazy enough to write this myself)

// =====================
// BASIC / CONTROL KEYS
// =====================
#define KEY_NULL           -1
#define KEY_ENTER          13
#define KEY_ESC            27
#define KEY_TAB            9
#define KEY_BACKSPACE      127
#define KEY_SPACE          32

// =====================
// ARROWS & NAVIGATION
// =====================
#define KEY_UP             1001
#define KEY_DOWN           1002
#define KEY_LEFT           1003
#define KEY_RIGHT          1004
#define KEY_HOME           1005
#define KEY_END            1006
#define KEY_INSERT         1007
#define KEY_DELETE         1008
#define KEY_PAGE_UP        1009
#define KEY_PAGE_DOWN      1010

// =====================
// FUNCTION KEYS
// =====================
#define KEY_F1             1011
#define KEY_F2             1012
#define KEY_F3             1013
#define KEY_F4             1014
#define KEY_F5             1015
#define KEY_F6             1016
#define KEY_F7             1017
#define KEY_F8             1018
#define KEY_F9             1019
#define KEY_F10            1020
#define KEY_F11            1021
#define KEY_F12            1022
#define KEY_F13            1023
#define KEY_F14            1024
#define KEY_F15            1025
#define KEY_F16            1026
#define KEY_F17            1027
#define KEY_F18            1028
#define KEY_F19            1029
#define KEY_F20            1030

// =====================
// ALPHABET KEYS
// =====================
#define KEY_A              'a'
#define KEY_B              'b'
#define KEY_C              'c'
#define KEY_D              'd'
#define KEY_E              'e'
#define KEY_F              'f'
#define KEY_G              'g'
#define KEY_H              'h'
#define KEY_I              'i'
#define KEY_J              'j'
#define KEY_K              'k'
#define KEY_L              'l'
#define KEY_M              'm'
#define KEY_N              'n'
#define KEY_O              'o'
#define KEY_P              'p'
#define KEY_Q              'q'
#define KEY_R              'r'
#define KEY_S              's'
#define KEY_T              't'
#define KEY_U              'u'
#define KEY_V              'v'
#define KEY_W              'w'
#define KEY_X              'x'
#define KEY_Y              'y'
#define KEY_Z              'z'

// =====================
// UPPERCASE ALIAS
// =====================
#define KEY_CAP_A          'A'
#define KEY_CAP_B          'B'
#define KEY_CAP_C          'C'
#define KEY_CAP_D          'D'
#define KEY_CAP_E          'E'
#define KEY_CAP_F          'F'
#define KEY_CAP_G          'G'
#define KEY_CAP_H          'H'
#define KEY_CAP_I          'I'
#define KEY_CAP_J          'J'
#define KEY_CAP_K          'K'
#define KEY_CAP_L          'L'
#define KEY_CAP_M          'M'
#define KEY_CAP_N          'N'
#define KEY_CAP_O          'O'
#define KEY_CAP_P          'P'
#define KEY_CAP_Q          'Q'
#define KEY_CAP_R          'R'
#define KEY_CAP_S          'S'
#define KEY_CAP_T          'T'
#define KEY_CAP_U          'U'
#define KEY_CAP_V          'V'
#define KEY_CAP_W          'W'
#define KEY_CAP_X          'X'
#define KEY_CAP_Y          'Y'
#define KEY_CAP_Z          'Z'

// =====================
// NUMBER KEYS
// =====================
#define KEY_0              '0'
#define KEY_1              '1'
#define KEY_2              '2'
#define KEY_3              '3'
#define KEY_4              '4'
#define KEY_5              '5'
#define KEY_6              '6'
#define KEY_7              '7'
#define KEY_8              '8'
#define KEY_9              '9'

// =====================
// SYMBOLS
// =====================
#define KEY_MINUS          '-'
#define KEY_EQUAL          '='
#define KEY_COMMA          ','
#define KEY_PERIOD         '.'
#define KEY_SLASH          '/'
#define KEY_SEMICOLON      ';'
#define KEY_QUOTE          '\''
#define KEY_LEFT_BRACKET   '['
#define KEY_RIGHT_BRACKET  ']'
#define KEY_BACKSLASH      '\\'
#define KEY_GRAVE          '`'

// =====================
// MODIFIER KEYS (just placeholders)
// =====================
#define KEY_SHIFT          2001
#define KEY_CTRL           2002
#define KEY_ALT            2003
#define KEY_CAPS_LOCK      2004

#endif
