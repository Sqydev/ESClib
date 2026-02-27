#include "../tester.h"

void RunSignalTester(int signo, const char* signame, int (*disableFn)(int), int (*enableFn)(int), const char* typeDesc) {
	InitTui(0);

	disableFn(signo);
	printf("TESTER: Disabled %s (%s type) waiting 5 sec\n", signame, typeDesc);
	fflush(stdout);

	ESleep(5, 0, 0);

	enableFn(signo);
	printf("TESTER: Enabled %s (%s type) waiting 5 sec\n", signame, typeDesc);
	fflush(stdout);

	ESleep(5, 0, 0);

	printf("TESTER: Ending test\n");
	fflush(stdout);
}
