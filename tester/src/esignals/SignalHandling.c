#include "../tester.h"

#include <time.h>

void SignalHandlingTester(void) {
	InitTui(0);

	printf("TESTER: SIGINT, and no more signals for now should trigger CloseTui() now\n");
	fflush(stdout);

	while(1) {
		SignalsStep();

		struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = 16667 * 1000;
		nanosleep(&ts, NULL);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}
