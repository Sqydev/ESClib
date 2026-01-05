#include "../tester.h"

void SignalHandlingTester(void) {
	InitTui(0, 0, 0, TUI_DYNAMIC);

	printf("TESTER: SIGINT, and no more signals for now should trigger CloseTui() now\n");
	fflush(stdout);

	while(1) {
		SignalsStep();

		usleep(16667);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}
