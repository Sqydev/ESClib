#include "./tester.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*TesterFn)(void);

typedef struct {
	const char* name;
	TesterFn fn;
	const char* description;
} TesterEntry;

const TesterEntry testers[] = {
	{"ST_SIGINT", SignalTester_SIGINT, "SignalTester_SIGINT"},
	{"SET_SIGINT", SignalESClibTasksTester_SIGINT, "SignalESClibTasksTester_SIGINT"},
	{"SCT_SIGINT", SignalCustomTasksTester_SIGINT, "SignalCustomTasksTester_SIGINT"},
	{"SBT_SIGINT", SignalBuildInTasksTester_SIGINT, "SignalBuildInTasksTester_SIGINT"},

	{"ST_SIGWINCH", SignalTester_SIGWINCH, "SignalTester_SIGWINCH"},
	{"SET_SIGWINCH", SignalESClibTasksTester_SIGWINCH, "SignalESClibTasksTester_SIGWINCH"},
	{"SCT_SIGWINCH", SignalCustomTasksTester_SIGWINCH, "SignalCustomTasksTester_SIGWINCH"},
	{"SBT_SIGWINCH", SignalBuildInTasksTester_SIGWINCH, "SignalBuildInTasksTester_SIGWINCH"},

	{"BBD", BackbuffDump, "BackbuffDump"},

	{"EDT", EndDrawingTest, "EndDrawingTest"},

	{"RLST", RenderLoopStressTest, "RenderLoopStressTest"},

	{"CT", CharTest, "CharTest"},
	{"TT", TextTest, "TextTest"},
	{"TTf", TextTestf, "TextTestf"},

	{"STT", StaticTuiTester, "StaticTuiTester"},

	{"DRT", DrawRecTester, "DrawRecTester"},

	{"FUCT", FuckedUpColorsTest, "FuckedUpColorsTest"},

	{"FRR", FunnyRottatingRec, "FunnyRottatingRec"},

	{"DCT", DrawCircleTester, "DrawCircleTester"},

	{"DLT", DrawLineTester, "DrawLineTester"},

	{"IKDT", IsKeyDownTester, "IsKeyDownTester"},

	{"LT", LoggingTester, "LoggingTester"},

	{"BGT", PatternsGPUTest, "PatternsGPUTest"},

	{"DTT", DrawTriangleTester, "DrawTriangleTester"},

	{"MBT", MergBrileTester, "MergBrileTester"},

	{"SCP", SetCursorPosTester, "SetCursorPosTester"},

	{"WFKPT", WaitForKeyPressTester, "WaitForKeyPressTester"},

	{"WT", WRITETester, "WRITETester"},

	{"TL", TextureLoad, "TextureLoad"},

	{"DT", DisplayTheToby, "DisplayTheToby"},

	{"CBT", ColorBlendingTester, "ColorBlendingTester"},
};

void PrintUsage(const char* prog) {
	printf("Usage: %s <TEST>\n\n", prog);
	printf("Available tests:\n");

	for(size_t i = 0; i < sizeof(testers) / sizeof(testers[0]); i++) {
		printf("  %s -> %s\n", testers[i].name, testers[i].description);
	}
}

int main(int argc, char* argv[]) {
	if(argc != 2) {
		PrintUsage(argv[0]);
		return EXIT_FAILURE;
	}

	// NOTE: Sooo, I found out that if there are 10 elements in the array then sizeof(array) / sizeof(array[0]) will be 10 :0
	for(size_t i = 0; i < sizeof(testers) / sizeof(testers[0]); i++) {
		if(strcmp(argv[1], testers[i].name) == 0) {
			testers[i].fn();
			return EXIT_SUCCESS;
		}
	}
	
	PrintUsage(argv[0]);
	return EXIT_FAILURE;
}
