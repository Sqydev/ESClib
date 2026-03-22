#include "../../../include/esclib.h"

#include <stdio.h>
#include <unistd.h>

void TextTest() {
	InitTui(165);

	printf("Drawing this in patern:\n");
	printf("@@@@@@@@@\n");
	printf("@#  #  #@\n");
	printf("@       @\n");
	printf("@#     #@\n");
	printf("@       @\n");
	printf("@#  #  #@\n");
	printf("@@@@@@@@@\n");
	fflush(stdout);

	sleep(1);

	while(1) {
		BeginDrawing();

		ClearTui((Color){ 0,0,0,0 }, (Color){ 255, 0, 0, 0});

		DrawChar("q", 0, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("ą", GetTuiDimensions().x, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("→", 0, GetTuiDimensions().y, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetTuiDimensions().x, GetTuiDimensions().y, (Color){ 255, 255, 255, 1 });
		DrawChar("ą", 0, GetTuiDimensions().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("q", GetTuiDimensions().x / 2, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("→", GetTuiDimensions().x, GetTuiDimensions().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("f", GetTuiDimensions().x / 2, GetTuiDimensions().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetTuiDimensions().x / 2, GetTuiDimensions().y, (Color){ 255, 255, 255, 1 });

		EndDrawing();
	}

	CloseTui();
}
