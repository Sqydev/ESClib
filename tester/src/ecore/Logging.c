#include "../../../include/esclib.h"

void LoggingTester() {
	InitTui(60, TUI_DYNAMIC);

	InitLoggin("./log");

	TraceLog("Log from tester:).");

	CloseLoggin();

	CloseTui();
}
