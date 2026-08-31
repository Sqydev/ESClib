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

void BackbuffDump(void);

void EndDrawingTest(void);
void RenderLoopStressTest(void);

void CharTest(void);
void TextTest(void);
void TextTestf(void);

void LoggingTester(void);

void StaticTuiTester(void);

void DrawRecTester(void);

void FuckedUpColorsTest(void);

void FunnyRottatingRec(void);

void DrawCircleTester(void);

void DrawLineTester(void);

void IsKeyDownTester(void);

void PatternsGPUTest(void);

void DrawTriangleTester(void);

void MergBrileTester(void);

void SetCursorPosTester(void);

void WaitForKeyPressTester(void);

void WRITETester(void);

void TextureLoad(void);

void DisplayTheToby(void);

void ColorBlendingTester(void);

void DrawTextureTester(void);
