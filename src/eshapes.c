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

#include <math.h>

void DrawRectangleRec(Rectanglei rec, Color color) {
	DrawRectangleV((Vector2i){ rec.x, rec.y }, (Vector2i){ rec.width, rec.height }, color);
}

void DrawRectangleV(Vector2i pos, Vector2i dimms, Color color) {
	DrawRectangle(pos.x, pos.y, dimms.x, dimms.y, color);
}

void DrawRectangle(int x, int y, int width, int height, Color color) {
	DrawRectangleEx(" ", x, y, width, height, &color, &color);
}

void DrawRectangleExRec(char* character, Rectanglei rec, Color* fg, Color* bg) {
	DrawRectangleExV(character, (Vector2i){ rec.x, rec.y }, (Vector2i){ rec.width, rec.height }, fg, bg);
}

void DrawRectangleExV(char* character, Vector2i pos, Vector2i dimms, Color* fg, Color* bg) {
	DrawRectangleEx(character, pos.x, pos.y, dimms.x, dimms.y, fg, bg);
}

void DrawRectangleEx(char* character, int x, int y, int width, int height, Color* fg, Color* bg) {
	DrawRectanglePro(character, x, y, width, height, 0, 0, fg, bg, 0, 0);
}

void DrawRectangleProRec(char* character, Rectanglei rec, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness) {
	DrawRectangleProV(character, (Vector2i){ rec.x, rec.y }, origin, (Vector2i){ rec.width, rec.height }, fg, bg, rotation, roundness);
}

void DrawRectangleProV(char* character, Vector2i pos, Vector2i dimms, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness) {
	DrawRectanglePro(character, pos.x, pos.y, dimms.x, dimms.y, origin.x, origin.y, fg, bg, rotation, roundness);
}

// NOTE: DO NOT FLOPING REMOVE Color*. Think about the people that want to make the graphicks with @ that have transparent backgrounds
void DrawRectanglePro(char* character, int posX, int posY, int width, int height, int originX, int originY, Color* fg, Color* bg, double rotation, float roundness) {
	(void)roundness;

    Vector2i lastIndex = GetLastTuiIndex();

    double cosA;
    double sinA;

	// NOTE: Check if angle is multiplicity of PI / 2. Accounding for float errors
	float q = rotation / (PI / 2.0f);
	int qi = (int)roundf(q);
	if(fabsf(q - qi) < 0.001f) {
		// NOTE: Somehow get dir
		int dir_idx = (qi % 4 + 4) % 4;

		switch(dir_idx) {
			case 0: { cosA = 1.0f; sinA = 0.0f; break; }
			case 1: { cosA = 0.0f; sinA = 1.0f; break; }
			case 2: { cosA = -1.0f; sinA = 0.0f; break; }
			case 3: { cosA = 0.0f; sinA = -1.0f; break; }
		}
	}
	else {
		cosA = cosf(rotation);
		sinA = sinf(rotation);
	}

    double maxRadius = sqrt(width * width + height * height);
    
    int startX = posX - (int)maxRadius;
    int endX = posX + (int)maxRadius;
    int startY = posY - (int)maxRadius;
    int endY = posY + (int)maxRadius;

    if(startX < 0) startX = 0;
    if(startY < 0) startY = 0;
    if(endX >= lastIndex.x) endX = lastIndex.x - 1;
    if(endY >= lastIndex.y) endY = lastIndex.y - 1;

	int vWidth = GetCharWidth(character);

    for(int wy = startY; wy <= endY; wy++) {
        for(int wx = startX; wx <= endX; wx += vWidth) {
            double dx = wx - posX;
            double dy = wy - posY;

            double ox = (dx * cosA) + (dy * sinA);
            double oy = -(dx * sinA) + (dy * cosA);

            double srcX = ox + originX;
            double srcY = oy + originY;

            if(srcX >= 0.0 && srcX < (double)width && srcY >= 0.0 && srcY < (double)height) {
                DrawCharEx(character, wx, wy, fg, bg);
            }
        }
    }
}

/*
 * To make.
    void DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom);   // Draw a vertical-gradient-filled rectangle
    void DrawRectangleGradientH(int posX, int posY, int width, int height, Color left, Color right);   // Draw a horizontal-gradient-filled rectangle
    void DrawRectangleLines(int posX, int posY, int width, int height, Color color);                   // Draw rectangle outline
*/
