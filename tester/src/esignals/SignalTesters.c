#include "../tester.h"
#include "../../../include/esclib.h"

void SignalTester_SIGINT(void) {
	RunSignalTester(
		SIGINT,
		"SIGINT",
		DisableSignal,
		EnableSignal,
		"standard"
	);
}

void SignalTester_SIGWINCH(void) {
	RunSignalTester(
		SIGWINCH,
		"SIGWINCH",
		DisableSignal,
		EnableSignal,
		"standard"
	);
}

void SignalESClibTasksTester_SIGINT(void) {
	RunSignalTester(
		SIGINT,
		"SIGINT",
		DisableSignalESClibTasks,
		EnableSignalESClibTasks,
		"ESClib"
	);
}

void SignalESClibTasksTester_SIGWINCH(void) {
	RunSignalTester(
		SIGWINCH,
		"SIGWINCH",
		DisableSignalESClibTasks,
		EnableSignalESClibTasks,
		"ESClib"
	);
}

void SignalCustomTasksTester_SIGINT(void) {
	RunSignalTester(
		SIGINT,
		"SIGINT",
		DisableSignalCustomTasks,
		EnableSignalCustomTasks,
		"custom"
	);
}

void SignalCustomTasksTester_SIGWINCH(void) {
	RunSignalTester(
		SIGWINCH,
		"SIGWINCH",
		DisableSignalCustomTasks,
		EnableSignalCustomTasks,
		"custom"
	);
}

void SignalBuildInTasksTester_SIGINT(void) {
	RunSignalTester(
		SIGINT,
		"SIGINT",
		DisableSignalBuildInTasks,
		EnableSignalBuildInTasks,
		"built-in"
	);
}

void SignalBuildInTasksTester_SIGWINCH(void) {
	RunSignalTester(
		SIGWINCH,
		"SIGWINCH",
		DisableSignalBuildInTasks,
		EnableSignalBuildInTasks,
		"built-in"
	);
}
