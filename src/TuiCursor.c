#include "../include/tuiutils.h"
#include "PrivateErrorProtocols.h"

#include <stdio.h>

void TuiCursorPosSet(int x, int y) {
	printf("\033[%d;%dH", y, x);
	fflush(stdout);
}

void TuiCursorMoveDirectional(int up, int down, int left, int right) {
	if (up > 0) printf("\033[%dA", up);
	if (down > 0) printf("\033[%dB", down);
	if (right > 0) printf("\033[%dC", right);
	if (left > 0) printf("\033[%dD", left);
	fflush(stdout);
}

void TuiCursorMove(int x, int y) {
	if (y < 0) printf("\033[%dA", -y);
	if (y > 0) printf("\033[%dB", y);
	if (x > 0) printf("\033[%dC", x);
	if (x < 0) printf("\033[%dD", -x);
	fflush(stdout);
}
