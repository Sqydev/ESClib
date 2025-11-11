#include "../include/esclib.h"
#include "PrivateErrorProtocols.h"





// FIX: SetForegroundStyle BREAKS IT. If in DrawChar SetForegroundStyle isn't present everything works just fine. Jup, SetForegroundStyle is just something else(or it's something with the (fontstyle)enum).
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
