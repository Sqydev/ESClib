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
