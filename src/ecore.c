#include "../include/esclib.h"

#include "./private/coredata.h"
#include "./private/signal_handling.h"
#include "./private/common_utils.h"
#include "unistd.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// NOTE: Create CoreData(Defined in ./private/coredata.h)
CoreData DATA;

void InitTui(int width, int height, int targetFps, TuiType tuiType) {
	atexit(CloseTui);

	SignalsSetup(&DATA);

	(void)width;
	(void)height;
	(void)targetFps;
	(void)tuiType;
}

void CloseTui(void) {
	printf("This is testing message from CloseTui() :)");
	fflush(stdout);
}

int EnableSignal(int signal) {
	switch(signal) {
		case SIGINT:
			DATA.SignalData.SIG_INT.enabled = true;
			break;
		
		default:
			UniWrite(STDERR_FILENO, "ERROR: Couldn't enable signal becouse signal is NOT supported", 62);
			errno = EINVAL;
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int DisableSignal(int signal) {
	switch(signal) {
		case SIGINT:
			DATA.SignalData.SIG_INT.enabled = false;
			break;
		
		default:
			UniWrite(STDERR_FILENO, "ERROR: Couldn't disable signal becouse signal is NOT supported", 62);
			errno = EINVAL;
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
