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

#endif
