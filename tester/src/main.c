#include "../../include/esclib.h"
#include "../../src/private/signal_handling.h"

#include "unistd.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

void SignalHandlingTester() {
	InitTui(0, 0, 0, TUI_DYNAMIC);

	printf("TESTER: SIGINT should trigger CloseTui() now\n");
	fflush(stdout);

	while(1) {
		SignalsStep();

		usleep(16667);
	}


	printf("TESTER: Ending test\n");
	fflush(stdout);
}

void DisableSignalTester() {
	InitTui(0, 0, 0, TUI_DYNAMIC);

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

void EnableSignalTester() {
	InitTui(0, 0, 0, TUI_DYNAMIC);

	DisableSignal(SIGINT);

	printf("TESTER: Enabled: SIGINT, and none other for now\n");
	fflush(stdout);

	EnableSignal(SIGINT);

	printf("TESTER: Enabled SIGINT(result should be like in SignalHandlingTester())\n");
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

int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("You have to give 1 and only 1 argument\n");
		fflush(stdout);

		return EXIT_FAILURE;
	}

	if(strcmp(argv[1], "SHT") == 0) {
		SignalHandlingTester();
	}
	else if(strcmp(argv[1], "DST") == 0) {
		DisableSignalTester();
	}
	else if(strcmp(argv[1], "EST") == 0) {
		EnableSignalTester();
	}
	else {
		printf("Wrong argument\n");
		printf("Correct arguments:\n");
		printf("1. SHT: SignalHandlingTester()\n");
		printf("2. DST: DisableSignalTester()\n");
		printf("3. EST: EnableSignalTester()\n");
		fflush(stdout);

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
