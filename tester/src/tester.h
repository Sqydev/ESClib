#include "../../include/esclib.h"

#include "../../src/private/signals/signals_actions.h"

#include <stdio.h>

int main(int argc, char* argv[]);

void RunSignalTester(int signo, const char* signame, int (*disableFn)(int), int (*enableFn)(int), const char* backendDesc);

void SignalTester_SIGINT(void);
void SignalESClibTasksTester_SIGINT(void);
void SignalCustomTasksTester_SIGINT(void);
void SignalBuildInTasksTester_SIGINT(void);

void SignalTester_SIGWINCH(void);
void SignalESClibTasksTester_SIGWINCH(void);
void SignalCustomTasksTester_SIGWINCH(void);
void SignalBuildInTasksTester_SIGWINCH(void);

void BackbuffDump();

void EndDrawingTest();
void RenderLoopStressTest();

void TextTest();

void StaticTuiTester();
