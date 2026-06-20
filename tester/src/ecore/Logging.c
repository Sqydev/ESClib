#include "../../../include/esclib.h"

void LoggingTester() {
	InitTui(60, TUI_DYNAMIC);

	InitLoggin("./log", LOG_DEBUG);

	TraceLog(LOG_INFO, "Log from tester:).");

	CloseLoggin();

	CloseTui();
}
