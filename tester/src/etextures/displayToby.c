#include "../../../include/esclib.h"

#include <stdio.h>

void DisplayTheToby(void) {
	InitTui(60, TUI_DYNAMIC);

	Texture toby[4];
	toby[0] = LoadTexture("./tester/src/etextures/GREENGASTER.png", TEXTURE_MONO);
	toby[1] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_8COLOR);
	toby[2] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_16COLOR);
	toby[3] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_256COLOR);

	size_t tobysCnt = 1;
	EscKey ch = '\0';

	do {
		BeginFrame();

		if(ch == KEY_ENTER) { tobysCnt = (tobysCnt + 1) % 4; }

		for(size_t y = 0; y < toby[tobysCnt].height; y++) {
			for(size_t x = 0; x < toby[tobysCnt].width; x++) {
				size_t index = (y * toby[tobysCnt].width + x) * 2;
				
				DrawCharEx(" ", x, y, NULL, &(Color){ toby[tobysCnt].data[index], 0, 0, toby[tobysCnt].data[index + 1], false });
			}
		}

		EndFrame();

		ch = WaitForKeyPress();
	}
	while(ch != KEY_ESCAPE);

	for(size_t i = 0; i < 4; i++) {
		FreeTexture(&toby[i]);
	}

	CloseTui();
}
