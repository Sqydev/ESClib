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

void DrawTextGradientRaw(const char* text, int posX, int posY, fontStyle Style, color Left, color Right, size_t lenght) {
	for(int i = 0; i < (int)lenght; i++) {
		// Sum alghorytm from THE internet

		float state_of_journey = (float)i / (float)(lenght - 1);

		color Color;

		Color.red= Left.red + (int)((Right.red- Left.red) * state_of_journey);
        Color.green = Left.green + (int)((Right.green - Left.green) * state_of_journey);
        Color.blue = Left.blue + (int)((Right.blue - Left.blue) * state_of_journey);

		DrawChar(text[i], posX + i, posY, Style, Color);
	}
}
void DrawTextGradient(const char* text, int posX, int posY, fontStyle Style, color Left, color Right) {
	size_t lenght = strlen(text);
	DrawTextGradientRaw(text, posX, posY, Style, Left, Right, lenght);
}
void DrawTextGradientV(const char* text, vector2 position, fontStyle Style, color Left, color Right) {
	DrawTextGradient(text, position.x, position.y, Style, Left, Right);
}
