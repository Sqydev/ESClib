#include "../../include/esclib.h"

#include <stdio.h>
#include <stdbool.h>


bool AlternateBuffModeActive = false;

void ClearAndFillScreen(color Color) {
	TuiClearScreen();
	FillScreen(Color);
}

void FillScreen(color Color) {
	// GOD KNOWS HOW
}

void TuiClearScreen() {
	printf("\033[2J\033[H");
	fflush(stdout);
}

void TuiClearLine() {
	printf("\r\033[2K");
	fflush(stdout);
}

void TuiClearChar() {
	printf("\033[D\033[K");
	fflush(stdout);
}

void TuiEnableBuffMode() {
	printf("\033[?1049h");
	fflush(stdout);
}

void TuiDisableBuffMode() {
	printf("\033[?1049l");
	fflush(stdout);
}

void TuiSwitchBuffMode() {
	if(!AlternateBuffModeActive) {
		TuiEnableBuffMode();
		AlternateBuffModeActive = true;
	}
	else {
		TuiDisableBuffMode();
		AlternateBuffModeActive = false;
	}
}

