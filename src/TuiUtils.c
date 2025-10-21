#include "../include/tuiutils.h"
#include "PrivateErrorProtocols.h"

#include <stdio.h>
#include <stdbool.h>


bool AlternateBuffModeActive = false;

void SwitchAlternateBuffMode() {
	if(!AlternateBuffModeActive) {
		printf("\033[?1049h");
		AlternateBuffModeActive = true;
	}
	else {
		printf("\033[?1049l");
		AlternateBuffModeActive = false;
	}
}

void Clean() {

}
