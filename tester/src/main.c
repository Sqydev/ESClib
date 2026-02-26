#include "./tester.h"

int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("You have to give 1 and only 1 argument\n");
		fflush(stdout);

		return EXIT_FAILURE;
	}

	if(strcmp(argv[1], "SHT") == 0) {
		SignalHandlingTester();
	}
	else if(strcmp(argv[1], "EST") == 0) {
		EnableSignalTester();
	}
	else if(strcmp(argv[1], "DST") == 0) {
		DisableSignalTester();
	}
	else if(strcmp(argv[1], "ESETT") == 0) {
		EnableSignalESClibTasksTester();
	}
	else if(strcmp(argv[1], "DSETT") == 0) {
		DisableSignalESClibTasksTester();
	}
	else if(strcmp(argv[1], "ESCTT") == 0) {
		EnableSignalCustomTasksTester();
	}
	else if(strcmp(argv[1], "DSCTT") == 0) {
		DisableSignalCustomTasksTester();
	}
	else if(strcmp(argv[1], "ESBTT") == 0) {
		EnableSignalBuildInTasksTester();
	}
	else if(strcmp(argv[1], "DSBTT") == 0) {
		DisableSignalBuildInTasksTester();
	}
	else if(strcmp(argv[1], "ICTT") == 0) {
		InitCloseTuiTester();
	}
	else {
		printf("Wrong argument\n");
		printf("Correct arguments:\n");
		printf("1. SHT: SignalHandlingTester()\n");

		printf("2. EST: EnableSignalTester()\n");
		printf("3. DST: DisableSignalTester()\n");

		printf("4. ESETT: EnableSignalESClibTasksTester()\n");
		printf("5. DSETT: DisableSignalESClibTasksTester()\n");

		printf("6. ESCTT: EnableSignalCustomTasksTester()\n");
		printf("7. DSCTT: DisableSignalCustomTasksTester()\n");

		printf("8. ESBTT: EnableSignalBuildInTasksTester()\n");
		printf("9. DSBTT: DisableSignalBuildInTasksTester()\n");

		printf("10. ITT: InitTuiTester()\n");
		fflush(stdout);

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
