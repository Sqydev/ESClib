#include "../../include/esclib.h"

#include <stdlib.h>

#include <signal.h>

int IsOpposite(int a, int b) {
	return (a == 0 && b == 2) || (a == 2 && b == 0) || (a == 1 && b == 3) || (a == 3 && b == 1);
}

const char* GetChar(int from, int to) {
	if(from == to) {
		return (from % 2 == 0) ? "═" : "║";
	}

	if(from == 0 && to == 1) return "╗";
	if(from == 1 && to == 2) return "╝";
	if(from == 2 && to == 3) return "╚";
	if(from == 3 && to == 0) return "╔";

	if(from == 1 && to == 0) return "╚";
	if(from == 2 && to == 1) return "╔";
	if(from == 3 && to == 2) return "╗";
	if(from == 0 && to == 3) return "╝";

	return "?";
}

void WINCHCLEAN() {
	ClearTui(BLACK, WHITE);
}

int main() {
	InitTui(60, TUI_DYNAMIC);

	HideCursor();

	Vector2i size = GetLastTuiIndex();
	Vector2i currPos = { size.x / 2, size.y / 2 };

	ClearTui(BLACK, WHITE);

	AddSignalTask(SIGWINCH, WINCHCLEAN, 0);

	srand(GetTime());

	int face = rand() % 4;

	float elapsedTime = 0.0f;
	float elapsedTimetwu = 0.0f;

	Color color = (Color){ 255, 255, 255, 1 };

	while(1) {
		BeginFrame();

		elapsedTime += GetDeltaTime();
		elapsedTimetwu += GetDeltaTime();

		if(elapsedTime >= 0.05f) {
			elapsedTime = 0.0f;

			int prevFace = face;

			int newFace = face;

			if(elapsedTimetwu > 0.5f) {
				newFace = rand() % 4;

				while(IsOpposite(prevFace, newFace) || (rand() % 3 == 0 && newFace == prevFace)) {
					newFace = rand() % 4;
				}

				color = (Color){ rand() % 256, rand() % 256, rand() % 256, 1 };

				elapsedTimetwu -= 0.5f;
			}

			face = newFace;

			DrawChar(GetChar(prevFace, face), currPos.x, currPos.y, color);

			Vector2i nextPos = currPos;

			switch (face) {
				case 0: nextPos.x += 1; break;
				case 1: nextPos.y += 1; break;
				case 2: nextPos.x -= 1; break;
				case 3: nextPos.y -= 1; break;
			}

			if(nextPos.x < 0 || nextPos.x >= size.x || nextPos.y < 0 || nextPos.y >= size.y) {
				face = (face + 2) % 4;
			} else {
				currPos = nextPos;
			}
		}

		EndFrame();
	}

	CloseTui();
}
