#include "../../include/tuiutils.h"

#include <stdio.h>
#include <stdbool.h>


bool AlternateBuffModeActive = false;

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

void TuiSwitchBuffMode() {
	if(!AlternateBuffModeActive) {
		printf("\033[?1049h");
		fflush(stdout);
		TuiClearScreen();
		AlternateBuffModeActive = true;
	}
	else {
		printf("\033[?1049l");
		fflush(stdout);
		AlternateBuffModeActive = false;
	}
}

