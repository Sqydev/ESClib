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

#include "./private/coredata.h"

#include <math.h>
#include <string.h>

void DrawCharVP(Panel panel, const char* character, Vector2i pos, Color color) {
	if(panel.x + panel.width <= panel.x + pos.x || pos.x < 0) { return; }
	if(panel.y + panel.height <= panel.y + pos.y || pos.y < 0) { return; }

	DrawChar(character, panel.x + pos.x, panel.y + pos.y, color);
}

void DrawCharP(Panel panel, const char* character, int x, int y, Color color) {
	if(panel.x + panel.width <= panel.x + x || x < 0) { return; }
	if(panel.y + panel.height <= panel.y + y || y < 0) { return; }

	DrawChar(character, panel.x + x, panel.y + y, color);
}

void DrawCharExVP(Panel panel, const char* character, Vector2i pos, Color* fg, Color* bg) {
	if(panel.x + panel.width <= panel.x + pos.x || pos.x < 0) { return; }
	if(panel.y + panel.height <= panel.y + pos.y || pos.y < 0) { return; }

	DrawCharEx(character, panel.x + pos.x, panel.y + pos.y, fg, bg);
}

void DrawCharExP(Panel panel, const char* character, int x, int y, Color* fg, Color* bg) {
	if(panel.x + panel.width <= panel.x + x || x < 0) { return; }
	if(panel.y + panel.height <= panel.y + y || y < 0) { return; }

	DrawCharEx(character, panel.x + x, panel.y + y, fg, bg);
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
	if(text == NULL) {
		return;
	}

	float cos = 0.0f;
	float sin = 0.0f;
	Vector2d dir = {0};

	dir = EDir(angle);
	cos = dir.x;
	sin = dir.y;

	// NOTE: Calc startpos with origin!
	float curX = x - (originX * cos) + (originY * sin);
	float curY = y - (originX * sin) - (originY * cos);

	const char* ptrr = text;

	while(*ptrr != '\0') {
		if(curX >= panel.x + panel.width) {
			return;
		}
		if(curY >= panel.y + panel.height) {
			return;
		}
		
		int vWidth = GetCharWidth(ptrr);
		
		if(curX >= panel.x + panel.width - (vWidth - 1)) {
			curX -= vWidth - 1;
		}

		int len = 0;
		if((*ptrr & 0x80) == 0) {
			len = 1;
		}
		else if((*ptrr & 0xE0) == 0xC0) {
			len = 2;
		}
		else if((*ptrr & 0xF0) == 0xE0) {
			len = 3;
		}
		else if((*ptrr & 0xF8) == 0xF0) {
			len = 4;
		}
		else {
			ptrr++;
			continue;
		}

		if(curX < panel.x || curY < panel.y) {
			ptrr += len;
			curX += dir.x * (spaceing + 1);
			curY += dir.y * (spaceing + 1);
			continue;
		}

		char tmpChar[5] = {0};
		memcpy(tmpChar, ptrr, len);

		DrawCharEx(tmpChar, (int)roundf(curX), (int)roundf(curY), fg, bg);

		curX += dir.x * (spaceing + 1);
		curY += dir.y * (spaceing + 1);

		ptrr += len;
	}
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
    if(thickness < 1) { return; }

    pointAX += panel.x;
    pointAY += panel.y;
    pointBX += panel.x;
    pointBY += panel.y;

    Vector2d aDir = EDir(CalculateAngleOfAGoingToB((Vector2){ pointAX, pointAY }, (Vector2){ pointBX, pointBY }));

    int vWidth = GetCharWidth(character);

    double maxF = fmax(fabs(aDir.x), fabs(aDir.y));
    if(maxF == 0) { return; }

    double stepX = vWidth / maxF;
    double stepY = 1.0 / maxF;

    Vector2d scaledDir = { aDir.x * stepX, aDir.y * stepY };

    Vector2i termPos = (Vector2i){ pointAX, pointAY };
    Vector2 realPos = (Vector2){ pointAX, pointAY };

    int panelMinX = panel.x;
    int panelMinY = panel.y;
    int panelMaxX = panel.x + panel.width;
    int panelMaxY = panel.y + panel.height;

    while((termPos.x - pointBX) * aDir.x + (termPos.y - pointBY) * aDir.y <= 0) {
        if(termPos.x >= panelMinX && termPos.x < panelMaxX && termPos.y >= panelMinY && termPos.y < panelMaxY) {
            DrawCharEx(character, termPos.x, termPos.y, fg, bg);

            for(int i = 1; i < thickness; i++) {
                int x1 = termPos.x + (int)(i * -aDir.y);
                int y1 = termPos.y + (int)(i *  aDir.x);

                int x2 = termPos.x + (int)(i *  aDir.y);
                int y2 = termPos.y + (int)(i * -aDir.x);

                if(x1 >= panelMinX && x1 < panelMaxX && y1 >= panelMinY && y1 < panelMaxY) { DrawCharEx(character, x1, y1, fg, bg); }

                if(thickness > 1 && x2 >= panelMinX && x2 < panelMaxX && y2 >= panelMinY && y2 < panelMaxY) { DrawCharEx(character, x2, y2, fg, bg); }
            }
        }

        realPos.x += scaledDir.x;
        realPos.y += scaledDir.y;

        termPos.x = (int)round(realPos.x);
        termPos.y = (int)round(realPos.y);
    }
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
	double aspectRatio = 1.0;
    if(aspectRatiofied) {
		aspectRatio = (GetCellProportions().x > 0 && GetCellProportions().y > 0) ? (double)GetCellProportions().x / (double)GetCellProportions().y : 0.5;
	}

	posX += panel.x;
	posY += panel.y;

	double cosA = ECos(rotation);
	double sinA = ESin(rotation);

    double widthInPixels = width * aspectRatio;
	// NOTE: So. sqrt(pw^2 + height^2) is calculating przekątna in pixels and / aspectRatio + 1 is just convert it from pixels to cells
    double maxRadius = sqrt(widthInPixels * widthInPixels + height * height) / aspectRatio + 1;

    int startX = posX - (int)maxRadius;
    int endX = posX + (int)maxRadius;
    int startY = posY - (int)maxRadius;
    int endY = posY + (int)maxRadius;

	if(endX < panel.x || endY < panel.y || startX >= panel.x + panel.width || startY >= panel.y + panel.height) {
	    return;
	}

    if(startX < panel.x) { startX = panel.x; }
    if(startY < panel.y) { startY = panel.y; }
    if(endX >= panel.x + panel.width) { endX = panel.x + panel.width - 1; }
    if(endY >= panel.y + panel.height) { endY = panel.y + panel.height - 1; }

	double rpx = (double)GetCellSizeInPixels().x * width;
    double rpy = (double)GetCellSizeInPixels().y * height;
	// NOTE: Clamp
    if(roundness < 0.0f) { roundness = 0.0f; }
	if(roundness > 100.0f) { roundness = 100.0f; }
	// NOTE: From 0 to 100
	double r = (roundness / 100.0) * fmin(rpx, rpy) / 2.0;

	int vWidth = GetCharWidth(character);

    for(int y = startY; y <= endY; y++) {
        for(int x = startX; x <= endX; x++) {
			if((x - posX) % vWidth != 0) { continue; }

            double dx = (x + (vWidth / 2.0) - posX) * aspectRatio;
            double dy = (y + 0.5 - posY);

            double ox = (dx * cosA) + (dy * sinA);
            double oy = -(dx * sinA) + (dy * cosA);

            double srcX = ox / aspectRatio + originX;
            double srcY = oy + originY;

			if(srcX < 0.0 || srcX >= (double)width || srcY < 0.0 || srcY >= (double)height) continue;

			if(roundness > 0.0f) {
            	double px = srcX * GetCellSizeInPixels().x;
                double py = srcY * GetCellSizeInPixels().y;

                double cx = px < r ? r : (px > rpx - r ? rpx - r : px);
                double cy = py < r ? r : (py > rpy - r ? rpy - r : py);

                double ddx = px - cx;
                double ddy = py - cy;

                if(ddx * ddx + ddy * ddy > r * r) { continue; }
			}

			if(lines) {
            	double px = srcX * GetCellSizeInPixels().x;
                double py = srcY * GetCellSizeInPixels().y;

                double distX = fmin(px, rpx - px);
                double distY = fmin(py, rpy - py);
                double distToEdge = 0.0;

                if(roundness > 0.0f && distX < r && distY < r) {
                    double ddx = r - distX;
                    double ddy = r - distY;
                    distToEdge = r - sqrt(ddx * ddx + ddy * ddy);
                } else {
                    distToEdge = fmin(distX, distY);
                }

                if(distToEdge > thicknessLines * 10) {
                    continue;
                }
            }
            
			DrawCharEx(character, x, y, fg, bg);
        }
    }
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
	double aspectRatio = 1.0;
    if(aspectRatiofied) {
		aspectRatio = (GetCellProportions().x > 0 && GetCellProportions().y > 0) ? (double)GetCellProportions().x / (double)GetCellProportions().y : 0.5;
	}

	centerX += panel.x;
	centerY += panel.y;

	int radiusCellsX = (int)(radius / aspectRatio) + 1;
	int radiusCellsY = radius + 1;

	int startX = centerX - radiusCellsX;
	int startY = centerY - radiusCellsY;
	int endX = centerX + radiusCellsX;
	int endY = centerY + radiusCellsY;

	if(endX < panel.x || endY < panel.y || startX >= panel.x + panel.width || startY >= panel.y + panel.height) {
    	return;
	}

    if(startX < panel.x) { startX = panel.x; }
    if(startY < panel.y) { startY = panel.y; }
    if(endX >= panel.x + panel.width) { endX = panel.x + panel.width - 1; }
    if(endY >= panel.y + panel.height) { endY = panel.y + panel.height - 1; }

	int vWidth = GetCharWidth(character);

	for(int y = startY; y <= endY; y++) {
		for(int x = startX; x <= endX; x++) {
			// NOTE: Magic things from DrawRectangle that scale the chars
			if((x - centerX) % vWidth != 0) { continue; }

			double dx = (x + vWidth / 2.0 - centerX) * aspectRatio;
			double dy = (y + 0.5 - centerY);

			double distance = (dx * dx) + (dy * dy);

			if(!lines) {
				if(distance > radius * radius) { continue; }
			}
			else {
				if(fabs(distance - radius * radius) > thicknessLines * (radius * aspectRatio + (vWidth))) { continue; }
			}

			// NOTE: Remamber tan is from -2pi to +2pi becouse you'll forget. So thoes "angle += 2 * PI;" are correct there is NO what is it's less than -2pi >:(
			double angle = atan2(dy, dx);

			if(angle < 0) { angle += 2 * PI; }

			if(startAngle < 0) { startAngle += 2 * PI; }
			if(endAngle < 0) { endAngle += 2 * PI; }

			bool inArc = false;
			if(startAngle < endAngle) {
				inArc = (angle >= startAngle && angle <= endAngle);
			}
			else {
				inArc = (angle >= startAngle || angle <= endAngle);
			}

			if(!inArc) { continue; }

			DrawCharEx(character, x, y, fg, bg);
		}
	}
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
	double aspectRatio = 1.0;
    if(aspectRatiofied) {
		aspectRatio = (GetCellProportions().x > 0 && GetCellProportions().y > 0) ? (double)GetCellProportions().x / (double)GetCellProportions().y : 0.5;
	}

	Ax += panel.x;
	Ay += panel.y;

	Bx += panel.x;
	By += panel.y;

	Cx += panel.x;
	Cy += panel.y;

	originX += panel.x;
	originY += panel.y;
	
	int oAx = (Ax - originX) * aspectRatio;
	int oBx = (Bx - originX) * aspectRatio;
	int oCx = (Cx - originX) * aspectRatio;
	int oAy = (Ay - originY) * aspectRatio;
	int oBy = (By - originY) * aspectRatio;
	int oCy = (Cy - originY) * aspectRatio;

	double cosA = ECos(rotation);
	double sinA = ESin(rotation);

	double aAx = ((oAx*cosA - oAy*sinA) / aspectRatio) + originX;
	double aBx = ((oBx*cosA - oBy*sinA) / aspectRatio) + originX;
	double aCx = ((oCx*cosA - oCy*sinA) / aspectRatio) + originX;
	double aAy = oAx*sinA + oAy*cosA + originY;
	double aBy = oBx*sinA + oBy*cosA + originY;
	double aCy = oCx*sinA + oCy*cosA + originY;

	int minX = (int)floor(fmin(aAx, fmin(aBx, aCx)));
	int maxX = (int)ceil(fmax(aAx, fmax(aBx, aCx)));
	int minY = (int)floor(fmin(aAy, fmin(aBy, aCy)));
	int maxY = (int)ceil(fmax(aAy, fmax(aBy, aCy)));

	int drawMinX = (minX > panel.x) ? minX : panel.x;
	int drawMaxX = (maxX < panel.x + panel.width - 1) ? maxX : panel.x + panel.width - 1;

	int drawMinY = (minY > panel.y) ? minY : panel.y;
	int drawMaxY = (maxY < panel.y + panel.height - 1) ? maxY : panel.y + panel.height - 1;
	if(drawMinX > drawMaxX || drawMinY > drawMaxY) {
	    return;
	}

	double lenAB;
	double lenBC;
	double lenCA;

	if(lines) {
		lenAB = sqrt((aBx-aAx)*(aBx-aAx) + (aBy-aAy)*(aBy-aAy));
		lenBC = sqrt((aCx-aBx)*(aCx-aBx) + (aCy-aBy)*(aCy-aBy));
		lenCA = sqrt((aAx-aCx)*(aAx-aCx) + (aAy-aCy)*(aAy-aCy));
	}

	for(int y = drawMinY; y <= drawMaxY; y++) {
		for(int x = drawMinX; x <= drawMaxX; x++) {
			double e1 = (x - aAx)*(aBy - aAy) - (y - aAy)*(aBx - aAx);
			double e2 = (x - aBx)*(aCy - aBy) - (y - aBy)*(aCx - aBx);
			double e3 = (x - aCx)*(aAy - aCy) - (y - aCy)*(aAx - aCx);
		
			if(!lines) {
				if((e1 >= 0 && e2 >= 0 && e3 >= 0) || (e1 <= 0 && e2 <= 0 && e3 <= 0)) {
					DrawCharEx(character, x, y, fg, bg);
				}
			}
			else {
				double d1 = fabs(e1) / lenAB;
    			double d2 = fabs(e2) / lenBC;
    			double d3 = fabs(e3) / lenCA;

				if(((e1 >= 0 && e2 >= 0 && e3 >= 0) || (e1 <= 0 && e2 <= 0 && e3 <= 0)) && (d1 <= thicknessLines || d2 <= thicknessLines || d3 <= thicknessLines)) {
					DrawCharEx(character, x, y, fg, bg);
				}
			}
		}
	}
}
