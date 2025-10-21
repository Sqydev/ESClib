#include "../include/tuiutils.h"
#include "PrivateErrorProtocols.h"

#include <stdio.h>
#include <stdbool.h>


bool AlternateBuffModeActive = false;

void ClearScreen() {
	printf("\033[2J\033[H");
	fflush(stdout);
}

void ClearLine() {
	printf("\r\033[2K");
	fflush(stdout);
}

void ClearChar() {
	printf("\033[D\033[K");
	fflush(stdout);
}

void SwitchAlternateBuffMode() {
	if(!AlternateBuffModeActive) {
		printf("\033[?1049h");
		fflush(stdout);
		ClearScreen();
		AlternateBuffModeActive = true;
	}
	else {
		printf("\033[?1049l");
		fflush(stdout);
		AlternateBuffModeActive = false;
	}
}

void SetCursorPos(int x, int y) {
	printf("\033[%d;%dH", y, x);
	fflush(stdout);
}

void MoveCursorDirectional(int up, int down, int left, int right) {
	if (up > 0) printf("\033[%dA", up);
	if (down > 0) printf("\033[%dB", down);
	if (right > 0) printf("\033[%dC", right);
	if (left > 0) printf("\033[%dD", left);
	fflush(stdout);
}

void MoveCursor(int x, int y) {
	if (y < 0) printf("\033[%dA", -y);
	if (y > 0) printf("\033[%dB", y);
	if (x > 0) printf("\033[%dC", x);
	if (x < 0) printf("\033[%dD", -x);
	fflush(stdout);
}
