#include "../../../include/esclib.h"

#include <stdio.h>
#include <unistd.h>

void CharTest() {
	InitTui(165, TUI_DYNAMIC);

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
		DrawChar("⠟", GetTuiDimensions().x / 2, GetTuiDimensions().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetTuiDimensions().x / 2, GetTuiDimensions().y, (Color){ 255, 255, 255, 1 });

		EndDrawing();
	}

	CloseTui();
}

void TextTest() {
	InitTui(165, TUI_DYNAMIC);

	while(1) {
		BeginDrawing();

		ClearTui((Color){ 0,0,0,0 }, (Color){ 255, 0, 0, 0});

		DrawText("To jest OGIE🔥ń", 0, 0, (Color){ 255, 255, 255, 1 });
		DrawTextPro("To jest kurczaczki rozprzesterzenny OGIE🔥ń", 0, 1, &(Color){ 255, 255, 255, 1 }, NULL, 1, 0);
		DrawTextPro("A to jest vertivcal OGIE🔥ń", 0, 3, &(Color){ 255, 255, 255, 1 }, NULL, 0, PI / 2);

		DrawTextPro("A to jest PI/3 OGIE🔥ń", 5, 5, &(Color){ 255, 255, 255, 1 }, NULL, 0, PI / 3);

		EndDrawing();
	}

	CloseTui();
}
