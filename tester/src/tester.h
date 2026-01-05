#include "../../include/esclib.h"
#include "../../src/private/signal_handling.h"

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#if defined (__linux__) || defined (__APPLE__)
	#include <unistd.h>
#endif

int main(int argc, char* argv[]);
void SignalHandlingTester(void);

void EnableSignalTester(void);
void DisableSignalTester(void);

void EnableSignalESClibTasksTester(void);
void DisableSignalESClibTasksTester(void);

void EnableSignalCustomTasksTester(void);
void DisableSignalCustomTasksTester(void);

void EnableSignalBuildInTasksTester(void);
void DisableSignalBuildInTasksTester(void);
