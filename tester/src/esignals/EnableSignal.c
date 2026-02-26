#include "../tester.h"

#include <time.h>

void EnableSignalTester(void) {
	InitTui(0);

	DisableSignal(SIGINT);

	printf("TESTER: Disabled: SIGINT, and none other for now\n");
	fflush(stdout);

	EnableSignal(SIGINT);

	printf("TESTER: Enabled SIGINT(result should be like in SignalHandlingTester())\n");
	fflush(stdout);

	printf("TESTER: Doing while loop until getchar returns: e\n");
	fflush(stdout);


	while(getchar() != 'e') {
		SignalsStep();

		struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = 16667 * 1000;
		nanosleep(&ts, NULL);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}

void EnableSignalESClibTasksTester(void) {
	InitTui(0);

	DisableSignalESClibTasks(SIGINT);

	printf("TESTER: Disabled: SIGINT's, and one other for now ESClib actions\n");
	fflush(stdout);

	EnableSignalESClibTasks(SIGINT);

	printf("TESTER: SIGINT's, and one other for now ESClib actions(result should be like in SignalHandlingTester())\n");
	fflush(stdout);

	printf("TESTER: Doing while loop until getchar returns: e\n");
	fflush(stdout);


	while(getchar() != 'e') {
		SignalsStep();

		struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = 16667 * 1000;
		nanosleep(&ts, NULL);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}

void EnableSignalCustomTasksTester(void) {
	InitTui(0);

	DisableSignalCustomTasks(SIGINT);

	printf("TESTER: Disabled: SIGINT's, and one other for now custom actions\n");
	fflush(stdout);

	EnableSignalCustomTasks(SIGINT);

	printf("TESTER: SIGINT's, and one other for now custom actions(result should be like in SignalHandlingTester())\n");
	fflush(stdout);

	printf("TESTER: Doing while loop until getchar returns: e\n");
	fflush(stdout);


	while(getchar() != 'e') {
		SignalsStep();

		struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = 16667 * 1000;
		nanosleep(&ts, NULL);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}

void EnableSignalBuildInTasksTester(void) {
	InitTui(0);

	DisableSignalBuildInTasks(SIGINT);

	printf("TESTER: Disabled: SIGINT's, and one other for now build in actions\n");
	fflush(stdout);

	EnableSignalBuildInTasks(SIGINT);

	printf("TESTER: SIGINT's, and one other for now build in actions(result should be like in SignalHandlingTester())\n");
	fflush(stdout);

	printf("TESTER: Doing while loop until getchar returns: e\n");
	fflush(stdout);


	while(getchar() != 'e') {
		SignalsStep();

		struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = 16667 * 1000;
		nanosleep(&ts, NULL);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}
