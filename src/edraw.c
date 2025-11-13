#include "../include/esclib.h"
#include "PrivateErrorProtocols.h"

#include <string.h>

void DrawChar(const char character, int posX, int posY, fontStyle Style, color Color) {
	vector2 cursorPositionBuff = GetCursorPosition();
	color foregroundColorBuff = GetForegroundColor();
	char foregroundStyleBuff = GetForegroundStyle();
	
	SetCursorPosition(posX, posY);

	SetForegroundColor(Color);
	SetForegroundStyle(Style);

	WriteToBackBuffor(&character, 1);

	SetCursorPosition(cursorPositionBuff.x, cursorPositionBuff.y);

	SetForegroundColor(foregroundColorBuff);
	SetForegroundStyle(foregroundStyleBuff);
}
void DrawCharV(const char character, vector2 position, fontStyle Style, color Color) {
	DrawChar(character, position.x, position.y, Style, Color);
}

void DrawTextRaw(const char* text, int posX, int posY, fontStyle Style, color Color, size_t lenght) {
	for(int i = 0; i < (int)lenght; i++) {
		DrawChar(text[i], posX + i, posY, Style, Color);
	}
}
void DrawText(const char* text, int posX, int posY, fontStyle Style, color Color) {
	size_t lenght = strlen(text);
	DrawTextRaw(text, posX, posY, Style, Color, lenght);
}
void DrawTextV(const char* text, vector2 position, fontStyle Style, color Color) {
	DrawText(text, position.x, position.y, Style, Color);
}
