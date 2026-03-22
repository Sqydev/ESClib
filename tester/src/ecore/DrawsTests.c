#include "../../../include/esclib.h"

#include "../../../src/private/common_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void EndDrawingTest() {
	InitTui(60);

	int times = 10;
	SBCell filler;
	filler.bgColor = (Color){ 0, 0, 0, 0};
	filler.fgColor = (Color){ 2, 3, 100, 0};
	filler.Char[0] = 'q';
	filler.Char[1] = 0;
	filler.Char[2] = 0;
	filler.Char[3] = 0;
	filler.CharLen = 1;

	BeginDrawing();

	ClearBackground((Color){ 0,0,0,0 }, (Color){ 255,255,255,1 });

	for(int i = 0; i < times; i++) {
		WriteToBackbuff(filler, i, 0);
	}

	EndDrawing();

	sleep(5);

	printf("TESTER: Ending EndDrawingTest\n");

	CloseTui();
}

void RenderLoopStressTest() {
	InitTui(165);

	printf("It's real epilepsy EPILEPSY WARNING");
	fflush(stdout);

	sleep(5);

	while(1) {
		BeginDrawing();

		for(size_t i = 0; i < GetBackbuffCellCount(); i++) {
			WriteToBackbuff(
				(SBCell){
        			.bgColor = (Color){ rand() % 255, rand() % 255, rand() % 255, 1 },
        			.fgColor = (Color){ rand() % 255, rand() % 255, rand() % 255, 1 },
					.Char[0] = (rand() % (126 - 32 + 1)) + 32,
					.CharLen = 1,
    			}, i, 0);
		}

		EndDrawing();
	}

	CloseTui();
}
