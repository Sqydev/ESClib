#include "../../../include/esclib.h"

#include "../../../src/private/common_utils.h"

#include <stdio.h>

void BackbuffDump() {
	InitTui(0);

	int times = 10;
	int filled = 0;
	SBCell filler;
	filler.bgColor = (Color){0,0,0,0};
	filler.fgColor = (Color){0,0,0,0};
	filler.Char[0] = 0;
	filler.Char[1] = 0;
	filler.Char[2] = 0;
	filler.Char[3] = 0;
	filler.CharLen = 4;

	printf("TESTER: Backbuff cell count: %lu\n", GetBackbuffCellCount());
	printf("TESTER: Backbuff sizeof: %lu\n", GetBackbuffSize());

	for(int i = 0; i < times; i++) {
		filled = WriteToBackbuff(&filler, 1);
	}

	printf("TESTER: Backbuff cell count: %lu\n", GetBackbuffCellCount());
	printf("TESTER: Backbuff sizeof: %lu\n", GetBackbuffSize());
	printf("TESTER: Filled(should be %d): %d\n", times, filled);
	
	printf("TESTER: Ending BackbuffDump\n");

	CloseTui();
}
