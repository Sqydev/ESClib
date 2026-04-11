#include "../../../include/esclib.h"

#include <stdio.h>
#include <unistd.h>

void CharTest() {
	InitTui(165, TUI_DYNAMIC, true);

	printf("Drawing this in patern:\n");
	printf("@@@@@@@@@\n");
	printf("@#  #  #@\n");
	printf("@   ##  @\n");
	printf("@#  #  #@\n");
	printf("@    #  @\n");
	printf("@#  #  #@\n");
	printf("@@@@@@@@@\n");
	fflush(stdout);

	sleep(1);

	while(1) {
		BeginDrawing();

		ClearTui((Color){ 0,0,0,0 }, (Color){ 255, 0, 0, 0});

		DrawChar("q", 0, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("ą", GetLastTuiIndex().x, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("→", 0, GetLastTuiIndex().y, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetLastTuiIndex().x, GetLastTuiIndex().y, (Color){ 255, 255, 255, 1 });
		DrawChar("ą", 0, GetLastTuiIndex().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("q", GetLastTuiIndex().x / 2, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("→", GetLastTuiIndex().x, GetLastTuiIndex().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("⠟", GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetLastTuiIndex().x / 2, GetLastTuiIndex().y, (Color){ 255, 255, 255, 1 });

		// NOTE: If it writes on something else. DO NOT panic. It's just wierd division. yk / 2.2. It only tests if it's overrided by one another
		DrawChar("🔥", GetLastTuiIndex().x / 2, GetLastTuiIndex().y - (GetLastTuiIndex().y / 2.2), (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetLastTuiIndex().x / 2 + 1, GetLastTuiIndex().y - (GetLastTuiIndex().y / 2.2), (Color){ 255, 255, 255, 1 });
		DrawChar("A", GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 2.2, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetLastTuiIndex().x / 2 + 1, GetLastTuiIndex().y / 2.2, (Color){ 255, 255, 255, 1 });

		EndDrawing();
	}

	CloseTui();
}

void TextTest() {
	InitTui(165, TUI_DYNAMIC, true);

	while(1) {
		BeginDrawing();

		ClearTui((Color){ 0,0,0,0 }, (Color){ 255, 0, 0, 0});

		DrawText("To jest OGIE🔥ń", 0, 0, (Color){ 255, 255, 255, 1 });
		DrawTextPro("To jest kurczaczki rozprzesterzenny OGIE🔥ń", 0, 1, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 1, 0);
		DrawTextPro("A to jest vertivcal OGIE🔥ń", 0, 3, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, PI / 2);

		DrawTextPro("A to jest PI/3 OGIE🔥ń", 5, 5, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, PI / 3);

		DrawTextPro("A to jest 45 * DEG2RAD OGIE🔥ń", 20, 8, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, 45 * DEG2RAD);

		DrawTextPro("(Ukryte w połowie)A to jest 45 * DEG2RAD z spaceingiem i BG fioletowym OGIE🔥ń", 30, 8, 0, 0, &(Color){ 255, 255, 255, 1 }, &(Color){ 255, 0, 255, 1 }, 1, 45 * DEG2RAD);

		DrawTextPro("A to jest ODWROTNY OGIE🔥ń", GetLastTuiIndex().x, 8, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, -PI);

		DrawTextPro("A to jest origin 5, 0 z rotacją 45 stopni OGIE🔥ń", 20, 8, 5, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, 45 * DEG2RAD);

		EndDrawing();
	}

	CloseTui();
}

void TextTestf() {
	InitTui(165, TUI_DYNAMIC, true);

	while(1) {
		BeginDrawing();

		ClearTui((Color){ 0,0,0,0 }, (Color){ 255, 0, 0, 0});

		DrawTextf("To jest OGIE🔥ń", 0, 0, (Color){ 255, 255, 255, 1 });
		DrawTextfPro("To jest kurczaczki rozprzesterzenny OGIE🔥ń", 0, 1, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 1, 0);
		DrawTextfPro("A to jest vertivcal OGIE🔥ń", 0, 3, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, PI / 2);

		DrawTextfPro("A to jest PI/3 OGIE🔥ń", 5, 5, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, PI / 3);

		DrawTextfPro("A to jest 45 * DEG2RAD OGIE🔥ń", 20, 8, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, 45 * DEG2RAD);

		DrawTextfPro("(Ukryte w połowie)A to jest 45 * DEG2RAD z spaceingiem i BG fioletowym OGIE🔥ń", 30, 8, 0, 0, &(Color){ 255, 255, 255, 1 }, &(Color){ 255, 0, 255, 1 }, 1, 45 * DEG2RAD);

		DrawTextfPro("A to jest ODWROTNY OGIE🔥ń", GetLastTuiIndex().x, 8, 0, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, -PI);

		DrawTextfPro("A to jest origin 5, 0 z rotacją 45 stopni OGIE🔥ń", 20, 8, 5, 0, &(Color){ 255, 255, 255, 1 }, NULL, 0, 45 * DEG2RAD);

		DrawTextf("🔥 5 == %d, 3.1 == %f, q == q, qwe == qwe", 50, 20, (Color){ 255, 255, 255, 1 }, 5, 3.1f, 'q', "qwe");

		DrawTextf("$fg$bgCZARNE$bg $fgBIAŁE $spSPACEING$sp $anAN$anGLE", 50, 21, (Color){ 255, 255, 255, 1 }, (Color){ 0, 0, 0, 0 }, (Color){ 255, 255, 255, 1 }, (Color){ 0, 0, 0, 0 }, (Color){ 255, 255, 255, 1 }, 2, 0, 45 * DEG2RAD, 315 * DEG2RAD);

		EndDrawing();
	}

	CloseTui();
}
