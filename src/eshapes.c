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
	// NOTE: Advanced technification. Short if statment
	// aspectRatio is used to convert the like width from cells format to pixels format by doing(something * aspectRatio) and recovering by(something / aspectRatio)
    double aspectRatio = (GetCellProportions().x > 0 && GetCellProportions().y > 0) ? (double)GetCellProportions().x / (double)GetCellProportions().y : 0.5;

	double cosA = ECos(rotation);
	double sinA = ESin(rotation);

	// NOTE: Just width in pixels
    double pw = width * aspectRatio;
	// NOTE: sqrt(pw * pw + height * height) is like przekątna cuz I can't do english and we devide it by aspectRatio + 1(it somehow chainges it back into collumns(X) and + 1 is cuz why not
    double maxRadius = sqrt(pw * pw + height * height) / aspectRatio + 1;

	// NOTE: I don't remamber
    int startX = posX - (int)maxRadius;
    int endX = posX + (int)maxRadius;
    int startY = posY - (int)maxRadius;
    int endY = posY + (int)maxRadius;

    if(startX < 0) { startX = 0; }
    if(startY < 0) { startY = 0; }
    if(endX >= GetLastTuiIndex().x) { endX = GetLastTuiIndex().x - 1; }
    if(endY >= GetLastTuiIndex().y) { endY = GetLastTuiIndex().y - 1; }

	double rpx = (double)GetCellSizeInPixels().x * width;
    double rpy = (double)GetCellSizeInPixels().y * height;
	// NOTE: Clamp
    if(roundness < 0.0f)   roundness = 0.0f;
	if(roundness > 100.0f) roundness = 100.0f;
	// NOTE: From 0 to 100
	double r = (roundness / 100.0) * fmin(rpx, rpy) / 2.0;

	int vWidth = GetCharWidth(character);

    for(int y = startY; y <= endY; y++) {
        for(int x = startX; x <= endX; x++) {
			// NOTE: collumns -> pixels
			// IT'S TAKEING THIS FROM THE CENTER OF CELL AND NOT THE LEFT UP CORNER REMAMBERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
            double dx = (x + (vWidth / 2.0) - posX) * aspectRatio;
            double dy = (y + 0.5 - posY);

			// NOTE: Do matrix math shennanygancs or however it's spelled
            double ox = (dx * cosA) + (dy * sinA);
            double oy = -(dx * sinA) + (dy * cosA);

			// NOTE: Come back to cells again
            double srcX = ox / aspectRatio + originX;
            double srcY = oy + originY;

			// NOTE: Skip if out of bounce
			if(srcX < 0.0 || srcX >= (double)width || srcY < 0.0 || srcY >= (double)height) continue;

			if(roundness > 0.0f) {
				// NOTE: Pos of point in pixels
            	double px = srcX * GetCellSizeInPixels().x;
                double py = srcY * GetCellSizeInPixels().y;

				// NOTE: I don't have a mind to expalin that here so here's chatgbt's explenation:
				// // Find the center of the nearest rounded corner circle.
				// Each corner circle center is at distance r from both edges (not at the corner itself).
				// If we're in the corner zone (px < r or px > rpx-r), snap to that corner circle center.
				// If we're in the middle, snap to px itself so distance will be 0 (always draw).
                double cx = px < r ? r : (px > rpx - r ? rpx - r : px);
                double cy = py < r ? r : (py > rpy - r ? rpy - r : py);

				// NOTE: Distance from point to nearest corner circle center (squared, to avoid sqrt)
                double ddx = px - cx;
                double ddy = py - cy;

				// NOTE: If distance > r, the point is outside the corner circle = outside the rounded rect
                if(ddx * ddx + ddy * ddy > r * r) { continue; }
			}
            
			DrawCharEx(character, x, y, fg, bg);
			x += (vWidth - 1);
        }
    }
}

/*
 * To make.
    void DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom);   // Draw a vertical-gradient-filled rectangle
    void DrawRectangleGradientH(int posX, int posY, int width, int height, Color left, Color right);   // Draw a horizontal-gradient-filled rectangle
    void DrawRectangleLines(int posX, int posY, int width, int height, Color color);                   // Draw rectangle outline
*/
