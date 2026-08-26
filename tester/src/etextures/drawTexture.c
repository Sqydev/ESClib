#include "../../../include/esclib.h"

#include <stdio.h>

void DrawTextureTester(void) {
	InitTui(60, TUI_DYNAMIC);

	HideCursor();

	Texture toby[5];
	toby[0] = LoadTexture("./tester/src/etextures/GREENGASTER.png", TEXTURE_MONO);
	toby[1] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_8COLOR);
	toby[2] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_16COLOR);
	toby[3] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_256COLOR);
	toby[4] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_TRUECOLOR);

	size_t tobysCnt = 1;
	double rot = 0;
	size_t enumer = 0;

	while(!IsKeyPressed(KEY_ESCAPE)) {
		BeginFrame();
		ClearTui(TERMBLACK, TERMWHITE);

		if(IsKeyPressed(KEY_ENTER)) { tobysCnt = (tobysCnt + 1) % 5; }
		if(IsKeyPressed(KEY_TAB)) { enumer = (enumer + 1) % 3; }
		if(IsKeyPressed(' ')) { rot += PI / 180; }

		DrawTexturePro(&toby[tobysCnt], " ", (Rectangle){ 100, 1, 100, 100 }, WHOLETEXTURE(toby[tobysCnt]), toby[tobysCnt].width / 2, toby[tobysCnt].height / 2, rot, BLANK, false, true, enumer, true);

		DrawTextf("%d, %d", 0, 0, TERMWHITE, GetTuiDimensions().x, GetTuiDimensions().y);

		EndFrame();
	}

	for(size_t i = 0; i < 5; i++) {
		FreeTexture(&toby[i]);
	}

	CloseTui();
}
