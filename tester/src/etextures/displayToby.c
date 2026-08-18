#include "../../../include/esclib.h"

#include <stdio.h>

void DisplayTheToby(void) {
	InitTui(60, TUI_DYNAMIC);

	Texture toby[3];
	toby[0] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_8COLOR);
	toby[1] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_16COLOR);
	toby[2] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_256COLOR);

	size_t tobysCnt = 0;
	EscKey ch = '\0';

	do {
		BeginFrame();

		if(ch == KEY_ENTER) { tobysCnt = (tobysCnt + 1) % 3; }

		for(size_t y = 0; y < toby[tobysCnt].height; y++) {
			for(size_t x = 0; x < toby[tobysCnt].width; x++) {
				DrawCharEx(" ", x, y, NULL, &(Color){ toby[tobysCnt].data[(toby[tobysCnt].width * (y - 1)) + x], 0, 0, 255, false });
			}
		}

		EndFrame();

		ch = WaitForKeyPress();
	}
	while(ch != KEY_ESCAPE);

	for(size_t i = 0; i < 3; i++) {
		FreeTexture(&toby[i]);
	}

	CloseTui();
}
