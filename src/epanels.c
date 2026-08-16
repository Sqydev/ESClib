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
*	you must not claim that you wrote the original software. An acknowledgment in 
*	product documentation is appreciated but not required.
* 
* 2. Source-Level Copyleft: Any altered versions (forks) of this software's source code, 
*	or files containing significant portions of this code, must be distributed under 
*	these same license terms. Such modified source code must be made publicly available 
*	to any recipient, even if used over a network (SaaS).
* 
* 3. Proprietary Integration: This software may be integrated into, linked with, or 
*	used as a component of proprietary and closed-source products. In such cases, 
*	the surrounding proprietary application code does not need to be disclosed, 
*	provided that the original or modified source code of THIS software remains 
*	available under the terms of Section 2.
* 
* 4. Persistent Metadata: All original credits, including those in the source code headers 
*	and binary metadata (e.g., ELF .comment section, PE StringFileInfo, or equivalent), 
*	must not be removed. You may add your own credits to forks, provided the original 
*	authorship remains clearly identified.
* 
* 5. Notice Retention: This license notice may not be removed or altered from any 
*	source or binary distribution.
*/

#include "../include/esclib.h"

#include <string.h>

void DrawCharVP(Panel panel, const char* character, Vector2i pos, Color color) {
	DrawCharP(panel, character, panel.x + pos.x, panel.y + pos.y, color);
}

void DrawCharP(Panel panel, const char* character, int x, int y, Color color) {
	DrawCharExP(panel, character, panel.x + x, panel.y + y, &color, NULL);
}

void DrawCharExVP(Panel panel, const char* character, Vector2i pos, Color* fg, Color* bg) {
	DrawCharExP(panel, character, panel.x + pos.x, panel.y + pos.y, fg, bg);
}

void DrawCharExP(Panel panel, const char* character, int x, int y, Color* fg, Color* bg) {
	DrawCharCore(character, panel.x + x, panel.y + y, fg, bg, true, panel);
}


void DrawTextVP(Panel panel, const char* text, Vector2i pos, Color color) {
	DrawTextP(panel, text, pos.x, pos.y, color);
}

void DrawTextP(Panel panel, const char* text, int x, int y, Color color) {
	DrawTextExP(panel, text, x, y, &color, NULL);
}

void DrawTextExVP(Panel panel, const char* text, Vector2i pos, Color* fg, Color* bg) {
	DrawTextExP(panel, text, pos.x, pos.y, fg, bg);
}

void DrawTextExP(Panel panel, const char* text, int x, int y, Color* fg, Color* bg) {
	DrawTextProP(panel, text, x, y, 0, 0, fg, bg, 0, 0);
}

void DrawTextProVP(Panel panel, const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spaceing, float angle) {
	DrawTextProP(panel, text, pos.x, pos.y, origin.x, origin.y, fg, bg, spaceing, angle);
}

void DrawTextProP(Panel panel, const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, float angle) {
	DrawTextCore(text, x, y, originX, originY, fg, bg, spaceing, angle, true, panel);
}

void DrawLineVP(Panel panel, Vector2i pointA, Vector2i pointB, Color color, int thickness) {
	DrawLineP(panel, pointA.x, pointA.y, pointB.x, pointB.y, color, thickness);
}

void DrawLineP(Panel panel, int pointAX, int pointAY, int pointBX, int pointBY, Color color, int thickness) {
	DrawLineExP(panel, " ", pointAX, pointAY, pointBX, pointBY, &color, &color, thickness);
}

void DrawLineExVP(Panel panel, char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness) {
	DrawLineExP(panel, character, pointA.x, pointA.y, pointB.x, pointB.y, fg, bg, thickness);
}

void DrawLineExP(Panel panel, char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness) {
	DrawLineProP(panel, character, pointAX, pointAY, pointBX, pointBY, fg, bg, thickness);
}

void DrawLineProVP(Panel panel, char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness) {
	DrawLineProP(panel, character, pointA.x, pointA.y, pointB.x, pointB.y, fg, bg, thickness);

}

void DrawLineProP(Panel panel, char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness) {
	DrawLineCore(character, pointAX, pointAY, pointBX, pointBY, fg, bg, thickness, true, panel);
}

void DrawRectangleRecP(Panel panel, Rectangle rec, Color color) {
	DrawRectangleVP(panel, (Vector2i){ rec.x, rec.y }, (Vector2i){ rec.width, rec.height }, color);
}

void DrawRectangleVP(Panel panel, Vector2i pos, Vector2i dimms, Color color) {
	DrawRectangleP(panel, pos.x, pos.y, dimms.x, dimms.y, color);
}

void DrawRectangleP(Panel panel, int x, int y, int width, int height, Color color) {
	DrawRectangleExP(panel, " ", x, y, width, height, &color, &color, false, 0);
}

void DrawRectangleExRecP(Panel panel, char* character, Rectangle rec, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawRectangleExVP(panel, character, (Vector2i){ rec.x, rec.y }, (Vector2i){ rec.width, rec.height }, fg, bg, lines, thicknessLines);
}

void DrawRectangleExVP(Panel panel, char* character, Vector2i pos, Vector2i dimms, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawRectangleExP(panel, character, pos.x, pos.y, dimms.x, dimms.y, fg, bg, lines, thicknessLines);
}

void DrawRectangleExP(Panel panel, char* character, int x, int y, int width, int height, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawRectangleProP(panel, character, x, y, width, height, 0, 0, fg, bg, 0, 0, lines, thicknessLines, true);
}

void DrawRectangleProRecP(Panel panel, char* character, Rectangle rec, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawRectangleProVP(panel, character, (Vector2i){ rec.x, rec.y }, origin, (Vector2i){ rec.width, rec.height }, fg, bg, rotation, roundness, lines, thicknessLines, aspectRatiofied);
}

void DrawRectangleProVP(Panel panel, char* character, Vector2i pos, Vector2i dimms, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawRectangleProP(panel, character, pos.x, pos.y, dimms.x, dimms.y, origin.x, origin.y, fg, bg, rotation, roundness, lines, thicknessLines, aspectRatiofied);
}

void DrawRectangleProP(Panel panel, char* character, int posX, int posY, int width, int height, int originX, int originY, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawRectangleCore(character, posX, posY, width, height, originX, originY, fg, bg, rotation, roundness, lines, thicknessLines, aspectRatiofied, true, panel);
}

void DrawCircleCirP(Panel panel, Circle circle, Color color) {
	DrawCircleVP(panel, (Vector2i){ circle.centerX, circle.centerY }, circle.radius, color);
}

void DrawCircleVP(Panel panel, Vector2i centerPos, int radius, Color color) {
	DrawCircleP(panel, centerPos.x, centerPos.y, radius, color);
}

void DrawCircleP(Panel panel, int centerX, int centerY, int radius, Color color) {
	DrawCircleExP(panel, " ", centerX, centerY, radius, &color, &color, false, 0);
}

void DrawCircleExCirP(Panel panel, char* character, Circle circle, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawCircleExVP(panel, character, (Vector2i){ circle.centerX, circle.centerY }, circle.radius, fg, bg, lines, thicknessLines);
}

void DrawCircleExVP(Panel panel, char* character, Vector2i centerPos, int radius, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawCircleExP(panel, character, centerPos.x, centerPos.y, radius, fg, bg, lines, thicknessLines);
}

void DrawCircleExP(Panel panel, char* character, int centerX, int centerY, int radius, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawCircleProP(panel, character, centerX, centerY, radius, 0, 2 * PI, fg, bg, lines, thicknessLines, true);
}

void DrawCircleProCirP(Panel panel, char* character, Circle circle, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawCircleProVP(panel, character, (Vector2i){ circle.centerX, circle.centerY }, circle.radius, angleSpectrum, fg, bg, lines, thicknessLines, aspectRatiofied);
}

void DrawCircleProVP(Panel panel, char* character, Vector2i centerPos, int radius, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawCircleProP(panel, character, centerPos.x, centerPos.y, radius, angleSpectrum.x, angleSpectrum.y, fg, bg, lines, thicknessLines, aspectRatiofied);
}

void DrawCircleProP(Panel panel, char* character, int centerX, int centerY, int radius, double startAngle, double endAngle, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawCircleCore(character, centerX, centerY, radius, startAngle, endAngle, fg, bg, lines, thicknessLines, aspectRatiofied, true, panel);
}

void DrawTriangleTriP(Panel panel, Triangle triangle, Color color) {
	DrawTriangleVP(panel, triangle.A, triangle.B, triangle.C, color);
}

void DrawTriangleVP(Panel panel, Vector2i posA, Vector2i posB, Vector2i posC, Color color) {
	DrawTriangleP(panel, posA.x, posA.y, posB.x, posB.y, posC.x, posC.y, color);
}

void DrawTriangleP(Panel panel, int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color color) {
	DrawTriangleExP(panel, " ", Ax, Ay, Bx, By, Cx, Cy, &color, &color, false, 0);
}

void DrawTriangleExTriP(Panel panel, char* character, Triangle triangle, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawTriangleExVP(panel, character, triangle.A, triangle.B, triangle.C, fg, bg, lines, thicknessLines);
}

void DrawTriangleExVP(Panel panel, char* character, Vector2i posA, Vector2i posB, Vector2i posC, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawTriangleExP(panel, character, posA.x, posA.y, posB.x, posB.y, posC.x, posC.y, fg, bg, lines, thicknessLines);
}

void DrawTriangleExP(Panel panel, char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawTriangleProP(panel, character, Ax, Ay, Bx, By, Cx, Cy, 0, 0, fg, bg, 0, lines, thicknessLines, true);
}

void DrawTriangleProTriP(Panel panel, char* character, Triangle triangle, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawTriangleProVP(panel, character, triangle.A, triangle.B, triangle.C, origin, fg, bg, rotation, lines, thicknessLines, aspectRatiofied);
}

void DrawTriangleProVP(Panel panel, char* character, Vector2i posA, Vector2i posB, Vector2i posC, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawTriangleProP(panel, character, posA.x, posA.y, posB.x, posB.y, posC.x, posC.y, origin.x, origin.y, fg, bg, rotation, lines, thicknessLines, aspectRatiofied);
}

void DrawTriangleProP(Panel panel, char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int originX, int originY, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawTriangleCore(character, Ax, Ay, Bx, By, Cx, Cy, originX, originY, fg, bg, rotation, lines, thicknessLines, aspectRatiofied, true, panel);
}
