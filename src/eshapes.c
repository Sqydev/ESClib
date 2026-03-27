/*
* Copyright (c) 2025-present Wojciech Kaptur ( _Sqyd_ / Sqydev )
* Github: https://github.com/Sqydev
* GPG Fingerprint: 6DC2516B0DFDA9C59661650722F7B8A777F33B56
* 
* This software is provided "as-is", without any express or implied warranty. In no event
* will the authors be held liable for any damages arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose, including commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
* 
* 1. Non-Misrepresentation: The origin of this software must not be misrepresented; 
*    you must not claim that you wrote the original software. An acknowledgment in 
*    product documentation is appreciated but not required.
* 
* 2. Source-Level Copyleft: Any altered versions (forks) of this software's source code, 
*    or files containing significant portions of this code, must be distributed under 
*    these same license terms. Such modified source code must be made publicly available 
*    to any recipient, even if used over a network (SaaS).
* 
* 3. Proprietary Integration: This software may be integrated into, linked with, or 
*    used as a component of proprietary and closed-source products. In such cases, 
*    the surrounding proprietary application code does not need to be disclosed, 
*    provided that the original or modified source code of THIS software remains 
*    available under the terms of Section 2.
* 
* 4. Persistent Metadata: All original credits, including those in the source code headers 
*    and binary metadata (e.g., ELF .comment section, PE StringFileInfo, or equivalent), 
*    must not be removed. You may add your own credits to forks, provided the original 
*    authorship remains clearly identified.
* 
* 5. Notice Retention: This license notice may not be removed or altered from any 
*    source or binary distribution.
*/

#include "../include/esclib.h"

void DrawRectangleRec(Rectanglei rec, Color color) {
	DrawRectangleV((Vector2i){ rec.x, rec.y }, (Vector2i){ rec.width, rec.height }, color);
}

void DrawRectangleV(Vector2i pos, Vector2i dimms, Color color) {
	DrawRectangle(pos.x, pos.y, dimms.x, dimms.y, color);
}

void DrawRectangle(int x, int y, int width, int height, Color color) {
	DrawRectangleEx(" ", x, y, width, height, color, color);
}

void DrawRectangleExRec(char* character, Rectanglei rec, Color fg, Color bg) {
	DrawRectangleExV(character, (Vector2i){ rec.x, rec.y }, (Vector2i){ rec.width, rec.height }, fg, bg);
}

void DrawRectangleExV(char* character, Vector2i pos, Vector2i dimms, Color fg, Color bg) {
	DrawRectangleEx(character, pos.x, pos.y, dimms.x, dimms.y, fg, bg);
}

void DrawRectangleEx(char* character, int x, int y, int width, int height, Color fg, Color bg) {
	DrawRectanglePro(character, x, y, width, height, 0, 0, fg, bg, 0, 0);
}

void DrawRectangleProRec(char* character, Rectanglei rec, Vector2i origin, Color fg, Color bg, double rotation, float roundness) {
	DrawRectangleProV(character, (Vector2i){ rec.x, rec.y }, origin, (Vector2i){ rec.width, rec.height }, fg, bg, rotation, roundness);
}

void DrawRectangleProV(char* character, Vector2i pos, Vector2i dimms, Vector2i origin, Color fg, Color bg, double rotation, float roundness) {
	DrawRectanglePro(character, pos.x, pos.y, dimms.x, dimms.y, origin.x, origin.y, fg, bg, rotation, roundness);
}

void DrawRectanglePro(char* character, int x, int y, int width, int height, int originX, int originY, Color fg, Color bg, double rotation, float roundness) {
	(void)character;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	(void)originX;
	(void)originY;
	(void)fg;
	(void)bg;
	(void)rotation;
	(void)roundness;
}

/*
 * To make.
    void DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom);   // Draw a vertical-gradient-filled rectangle
    void DrawRectangleGradientH(int posX, int posY, int width, int height, Color left, Color right);   // Draw a horizontal-gradient-filled rectangle
    void DrawRectangleLines(int posX, int posY, int width, int height, Color color);                   // Draw rectangle outline
*/
