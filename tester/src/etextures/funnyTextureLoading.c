#include "../../../include/esclib.h"

#include <stdio.h>

void TextureLoad(void) {
	Texture toby[5];
	toby[0] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_MONO);
	toby[1] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_8COLOR);
	toby[2] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_16COLOR);
	toby[3] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_256COLOR);
	toby[4] = LoadTexture("./tester/src/etextures/texture.jpg", TEXTURE_TRUECOLOR);

	for(int i = 0; i < 5; i++) {
		printf("Data addr: %p\n", toby[i].data);
		printf("Id: %zu\n", toby[i].id);
		printf("Type: %d\n\n", toby[i].type);

		printf("Width: %zu\n", toby[i].width);
		printf("Height: %zu\n\n\n\n", toby[i].height);
		fflush(stdout);
	}

	for(int i = 0; i < 5; i++) {
		FreeTexture(&toby[i]);
	}
}
