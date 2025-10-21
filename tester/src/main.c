#include "../../include/tuiutils.h"

#include <stdio.h>
#include <unistd.h>

void AltBuffModeSwitchTest() {
	printf("Normal mode\n\n");

	usleep(1000000);

	SwitchAlternateBuffMode();

	printf("Alt Mode\n\n");

	usleep(1000000);

	SwitchAlternateBuffMode();
}

int main() {
	AltBuffModeSwitchTest();
}
