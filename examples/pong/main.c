#include "../../include/esclib.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

#define PLAYER_HEIGHT 10
#define SPEED 1

int main() {
	InitTui(60, TUI_DYNAMIC);
	HideCursor();

	int playerPos;

	Vector2 ballPos;
	Vector2d ballDir;

	double dtc;

reset:

	srand(time(NULL));

	playerPos = (GetLastTuiIndex().y / 2) - (PLAYER_HEIGHT / 2);

	ballPos = (Vector2){ (float)GetLastTuiIndex().x / 2, (float)GetLastTuiIndex().y / 2 };
	ballDir = EDir(((double)rand() / RAND_MAX) * 2.0 * PI);

	dtc = 0;

	while(1) {
		BeginFrame();
		ClearTui(TERMBLACK, TERMWHITE);

		dtc += GetDeltaTime();

		if(IsKeyPressed('w') && playerPos > 0) { playerPos--; }
		if(IsKeyPressed('s') && playerPos < GetLastTuiIndex().y - PLAYER_HEIGHT) { playerPos++; }

		if(dtc > 0.03 * SPEED) {
			ballPos.x += ballDir.x;
			ballPos.y += ballDir.y;

			dtc -= 0.03 * SPEED;
		}

		if(ballPos.x >= GetLastTuiIndex().x) { ballDir.x = -ballDir.x; ballPos.x += ballDir.x; }
		if(ballPos.y >= GetLastTuiIndex().y || ballPos.y <= 0) { ballDir.y = -ballDir.y; ballPos.y += ballDir.y; }
		if((ballPos.y >= playerPos && ballPos.y <= playerPos + PLAYER_HEIGHT) && ballPos.x < 0) { ballDir.x = -ballDir.x; ballPos.x += ballDir.x; }
		if(ballPos.x < 0) {
			goto reset;
		}

		DrawLine(0, playerPos, 0, playerPos + PLAYER_HEIGHT, TERMWHITE, 1);

		DrawCharEx(" ", roundf(ballPos.x), roundf(ballPos.y), &TERMWHITE, &TERMWHITE);

		EndFrame();
	}

	CloseTui();
}
