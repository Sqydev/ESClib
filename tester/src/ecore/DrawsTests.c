#include "../../../include/esclib.h"

#include "../../../src/private/common_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void EndDrawingTest() {
	InitTui(60, TUI_DYNAMIC);

	int times = 10;
	SBCell filler;
	filler.bgColor = TRUEBLACK;
	filler.fgColor = (Color){ 2, 3, 100, 255, true };
	filler.Char[0] = 'q';
	filler.Char[1] = 0;
	filler.Char[2] = 0;
	filler.Char[3] = 0;
	filler.CharLen = 1;

	BeginFrame();

	ClearTui(TERMBLACK, TERMWHITE);

	for(int i = 0; i < times; i++) {
		WriteToBackbuff(filler, i, 0);
	}

	EndFrame();

	sleep(5);

	printf("TESTER: Ending EndFrameTest\n");

	CloseTui();
}

void RenderLoopStressTest() {
	printf("It's real epilepsy EPILEPSY WARNING\n");
	printf("Write 63 to continue: ");
	fflush(stdout);

	int a = 0;
	scanf("%d", &a);

	if(a != 63) { return; }

	printf("Write fps: ");
	fflush(stdout);

	scanf("%d", &a);

	InitTui(a, TUI_DYNAMIC);

	while(1) {
		BeginFrame();

		for(size_t i = 0; i < GetBackbuffCellCount(); i++) {
			WriteToBackbuff(
				(SBCell){
        			.bgColor = (Color){ rand() % 255, rand() % 255, rand() % 255, 255, true },
        			.fgColor = (Color){ rand() % 255, rand() % 255, rand() % 255, 255, true },
					.Char[0] = (rand() % (126 - 32 + 1)) + 32,
					.CharLen = 1,
    			}, i, 0);
		}

		EndFrame();
	}

	CloseTui();
}
