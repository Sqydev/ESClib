#include "../../../include/esclib.h"

void LoggingTester() {
	InitTui(60, TUI_DYNAMIC, true);

	InitLoggin("./log");

	TraceLog("Log from tester:).");

	CloseLoggin();

	CloseTui();
}
