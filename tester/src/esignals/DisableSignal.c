#include "../tester.h"

void DisableSignalTester(void) {
	InitTui(0);

	DisableSignal(SIGINT);

	printf("TESTER: Disabled: SIGINT, and none other for now\n");
	fflush(stdout);

	printf("TESTER: Doing while loop until getchar returns: e\n");
	fflush(stdout);


	while(getchar() != 'e') {
		SignalsStep();

		usleep(16667);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}

void DisableSignalESClibTasksTester(void) {
	InitTui(0);

	DisableSignalESClibTasks(SIGINT);

	printf("TESTER: Disabled: SIGINT's, and one other for now ESClib actions\n");
	fflush(stdout);

	printf("TESTER: Doing while loop until getchar returns: e\n");
	fflush(stdout);


	while(getchar() != 'e') {
		SignalsStep();

		usleep(16667);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}

void DisableSignalCustomTasksTester(void) {
	InitTui(0);

	DisableSignalCustomTasks(SIGINT);

	printf("TESTER: Disabled: SIGINT's, and one other for now custom actions\n");
	fflush(stdout);

	printf("TESTER: Doing while loop until getchar returns: e\n");
	fflush(stdout);


	while(getchar() != 'e') {
		SignalsStep();

		usleep(16667);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}

void DisableSignalBuildInTasksTester(void) {
	InitTui(0);

	DisableSignalBuildInTasks(SIGINT);

	printf("TESTER: Disabled: SIGINT's, and one other for now build in actions\n");
	fflush(stdout);

	printf("TESTER: Doing while loop until getchar returns: e\n");
	fflush(stdout);


	while(getchar() != 'e') {
		SignalsStep();

		usleep(16667);
	}

	printf("TESTER: Ending test\n");
	fflush(stdout);
}
