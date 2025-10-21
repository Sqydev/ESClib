#ifndef TUIUTILS_H
#define TUIUTILS_H



void ClearScreen();

void ClearLine();

void ClearChar();

void SwitchAlternateBuffMode();

void SetCursorPos(int x, int y);

void MoveCursorDirectional(int up, int down, int left, int right);

void MoveCursor(int x, int y);



#endif
