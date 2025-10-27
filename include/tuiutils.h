#ifndef TUIUTILS_H
#define TUIUTILS_H

// Screen

void TuiClearScreen();

void TuiClearLine();

void TuiClearChar();

void TuiSwitchBuffMode();


// Cursor

void TuiCursorPosSet(int x, int y);

void TuiCursorMoveDirectional(int up, int down, int left, int right);

void TuiCursorMove(int x, int y);


// Input

void EnableRawMode();

void DisableRawMode();

int GetKey();



// Macros


// KEYS(by chatgbt becouse I'm not crazy enough to write this myself)

// =====================
// BASIC / CONTROL KEYS
// =====================
#define KEY_NULL           -1
#define KEY_ESC            27
#define KEY_ENTER          13
#define KEY_TAB            9
#define KEY_BACKSPACE      8
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
