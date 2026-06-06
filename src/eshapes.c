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

void DrawLineV(Vector2i pointA, Vector2i pointB, Color color, int thickness) {
	DrawLine(pointA.x, pointA.y, pointB.x, pointB.y, color, thickness);
}

void DrawLine(int pointAX, int pointAY, int pointBX, int pointBY, Color color, int thickness) {
	DrawLineEx(" ", pointAX, pointAY, pointBX, pointBY, &color, &color, thickness);
}

void DrawLineExV(char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness) {
	DrawLineEx(character, pointA.x, pointA.y, pointB.x, pointB.y, fg, bg, thickness);
}

void DrawLineEx(char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness) {
	DrawLinePro(character, pointAX, pointAY, pointBX, pointBY, fg, bg, thickness);
}

void DrawLineProV(char* character, Vector2i pointA, Vector2i pointB, Color* fg, Color* bg, int thickness) {
	DrawLinePro(character, pointA.x, pointA.y, pointB.x, pointB.y, fg, bg, thickness);

}

void DrawLinePro(char* character, int pointAX, int pointAY, int pointBX, int pointBY, Color* fg, Color* bg, int thickness) {
	Vector2d aDir = EDir(CalculateAngleOfAGoingToB((Vector2){ pointAX, pointAY }, (Vector2){ pointBX, pointBY }));

	int vWidth = GetCharWidth(character);

	double stepX = vWidth / fmax(fabs(aDir.x), fabs(aDir.y));
	double stepY = 1 / fmax(fabs(aDir.x), fabs(aDir.y));

	Vector2d scaledDir = { aDir.x * stepX, aDir.y * stepY };

	Vector2i termPos = (Vector2i){ pointAX, pointAY };
	Vector2 realPos = (Vector2){ pointAX, pointAY };

	if(thickness < 1) { return; }

	while((termPos.x - pointBX) * aDir.x + (termPos.y - pointBY) * aDir.y <= 0) {
		if(termPos.x > GetLastTuiIndex().x) { return; }
		if(termPos.y > GetLastTuiIndex().y) { return; }
		if(termPos.x < 0) { return; }
		if(termPos.y < 0) { return; }

		DrawCharEx(character, termPos.x, termPos.y, fg, bg);

		for(int i = 1; i < thickness; i++) {
			DrawCharEx(character, termPos.x + (int)(i * -aDir.y), termPos.y + (int)(i * aDir.x), fg, bg);
			if(thickness > 1) { DrawCharEx(character, termPos.x + (int)(i * aDir.y), termPos.y + (int)(i * -aDir.x), fg, bg); }
		}

		realPos.x += scaledDir.x;
		realPos.y += scaledDir.y;

		termPos.x = (int)round(realPos.x);
		termPos.y = (int)round(realPos.y);
	}
}

void DrawRectangleRec(Rectangle rec, Color color) {
	DrawRectangleV((Vector2i){ rec.x, rec.y }, (Vector2i){ rec.width, rec.height }, color);
}

void DrawRectangleV(Vector2i pos, Vector2i dimms, Color color) {
	DrawRectangle(pos.x, pos.y, dimms.x, dimms.y, color);
}

void DrawRectangle(int x, int y, int width, int height, Color color) {
	DrawRectangleEx(" ", x, y, width, height, &color, &color, false, 0);
}

void DrawRectangleExRec(char* character, Rectangle rec, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawRectangleExV(character, (Vector2i){ rec.x, rec.y }, (Vector2i){ rec.width, rec.height }, fg, bg, lines, thicknessLines);
}

void DrawRectangleExV(char* character, Vector2i pos, Vector2i dimms, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawRectangleEx(character, pos.x, pos.y, dimms.x, dimms.y, fg, bg, lines, thicknessLines);
}

void DrawRectangleEx(char* character, int x, int y, int width, int height, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawRectanglePro(character, x, y, width, height, 0, 0, fg, bg, 0, 0, lines, thicknessLines, true);
}

void DrawRectangleProRec(char* character, Rectangle rec, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawRectangleProV(character, (Vector2i){ rec.x, rec.y }, origin, (Vector2i){ rec.width, rec.height }, fg, bg, rotation, roundness, lines, thicknessLines, aspectRatiofied);
}

void DrawRectangleProV(char* character, Vector2i pos, Vector2i dimms, Vector2i origin, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawRectanglePro(character, pos.x, pos.y, dimms.x, dimms.y, origin.x, origin.y, fg, bg, rotation, roundness, lines, thicknessLines, aspectRatiofied);
}

void DrawRectanglePro(char* character, int posX, int posY, int width, int height, int originX, int originY, Color* fg, Color* bg, double rotation, float roundness, bool lines, int thicknessLines, bool aspectRatiofied) {
	double aspectRatio = 1.0;
    if(aspectRatiofied) {
		aspectRatio = (GetCellProportions().x > 0 && GetCellProportions().y > 0) ? (double)GetCellProportions().x / (double)GetCellProportions().y : 0.5;
	}

	double cosA = ECos(rotation);
	double sinA = ESin(rotation);

    double widthInPixels = width * aspectRatio;
	// NOTE: So. sqrt(pw^2 + height^2) is calculating przekątna in pixels and / aspectRatio + 1 is just convert it from pixels to cells
    double maxRadius = sqrt(widthInPixels * widthInPixels + height * height) / aspectRatio + 1;

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

void DrawCircleCir(Circle circle, Color color) {
	DrawCircleV((Vector2i){ circle.centerX, circle.centerY }, circle.radius, color);
}

void DrawCircleV(Vector2i centerPos, int radius, Color color) {
	DrawCircle(centerPos.x, centerPos.y, radius, color);
}

void DrawCircle(int centerX, int centerY, int radius, Color color) {
	DrawCircleEx(" ", centerX, centerY, radius, &color, &color, false, 0);
}

void DrawCircleExCir(char* character, Circle circle, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawCircleExV(character, (Vector2i){ circle.centerX, circle.centerY }, circle.radius, fg, bg, lines, thicknessLines);
}

void DrawCircleExV(char* character, Vector2i centerPos, int radius, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawCircleEx(character, centerPos.x, centerPos.y, radius, fg, bg, lines, thicknessLines);
}

void DrawCircleEx(char* character, int centerX, int centerY, int radius, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawCirclePro(character, centerX, centerY, radius, 0, 2 * PI, fg, bg, lines, thicknessLines, true);
}

void DrawCircleProCir(char* character, Circle circle, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawCircleProV(character, (Vector2i){ circle.centerX, circle.centerY }, circle.radius, angleSpectrum, fg, bg, lines, thicknessLines, aspectRatiofied);
}

void DrawCircleProV(char* character, Vector2i centerPos, int radius, Vector2d angleSpectrum, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawCirclePro(character, centerPos.x, centerPos.y, radius, angleSpectrum.x, angleSpectrum.y, fg, bg, lines, thicknessLines, aspectRatiofied);
}

void DrawCirclePro(char* character, int centerX, int centerY, int radius, double startAngle, double endAngle, Color* fg, Color* bg, bool lines, int thicknessLines, bool aspectRatiofied) {
	double aspectRatio = 1.0;
    if(aspectRatiofied) {
		aspectRatio = (GetCellProportions().x > 0 && GetCellProportions().y > 0) ? (double)GetCellProportions().x / (double)GetCellProportions().y : 0.5;
	}

	int radiusCellsX = (int)(radius / aspectRatio) + 1;
	int radiusCellsY = radius + 1;

	int startX = centerX - radiusCellsX;
	int startY = centerY - radiusCellsY;
	int endX = centerX + radiusCellsX;
	int endY = centerY + radiusCellsY;

	if(startX < 0) { startX = 0; }
    if(startY < 0) { startY = 0; }
    if(endX >= GetLastTuiIndex().x) { endX = GetLastTuiIndex().x - 1; }
    if(endY >= GetLastTuiIndex().y) { endY = GetLastTuiIndex().y - 1; }

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

void DrawTriangleTri(Triangle triangle, Color color) {
	DrawTriangleV(triangle.A, triangle.B, triangle.C, color);
}

void DrawTriangleV(Vector2i posA, Vector2i posB, Vector2i posC, Color color) {
	DrawTriangle(posA.x, posA.y, posB.x, posB.y, posC.x, posC.y, color);
}

void DrawTriangle(int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color color) {
	DrawTriangleEx(" ", Ax, Ay, Bx, By, Cx, Cy, &color, &color, false, 0);
}

void DrawTriangleExTri(char* character, Triangle triangle, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawTriangleExV(character, triangle.A, triangle.B, triangle.C, fg, bg, lines, thicknessLines);
}

void DrawTriangleExV(char* character, Vector2i posA, Vector2i posB, Vector2i posC, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawTriangleEx(character, posA.x, posA.y, posB.x, posB.y, posC.x, posC.y, fg, bg, lines, thicknessLines);
}

void DrawTriangleEx(char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, Color* fg, Color* bg, bool lines, int thicknessLines) {
	DrawTrianglePro(character, Ax, Ay, Bx, By, Cx, Cy, 0, 0, fg, bg, 0, lines, thicknessLines, true);
}

void DrawTriangleProTri(char* character, Triangle triangle, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawTriangleProV(character, triangle.A, triangle.B, triangle.C, origin, fg, bg, rotation, lines, thicknessLines, aspectRatiofied);
}

void DrawTriangleProV(char* character, Vector2i posA, Vector2i posB, Vector2i posC, Vector2i origin, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied) {
	DrawTrianglePro(character, posA.x, posA.y, posB.x, posB.y, posC.x, posC.y, origin.x, origin.y, fg, bg, rotation, lines, thicknessLines, aspectRatiofied);
}

void DrawTrianglePro(char* character, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int originX, int originY, Color* fg, Color* bg, double rotation, bool lines, int thicknessLines, bool aspectRatiofied) {
	// NOTE: NO, I know what your thinking, aspectRatio has sense here. See how it looks when rotating. >:( YOU FORGOR WHY YOU MADE THIS
	double aspectRatio = 1.0;
    if(aspectRatiofied) {
		aspectRatio = (GetCellProportions().x > 0 && GetCellProportions().y > 0) ? (double)GetCellProportions().x / (double)GetCellProportions().y : 0.5;
	}
	(void)aspectRatio;
	(void)lines;
	(void)thicknessLines;

	int oAx = Ax - originX;
	int oBx = Bx - originX;
	int oCx = Cx - originX;
	int oAy = Ay - originY;
	int oBy = By - originY;
	int oCy = Cy - originY;

	double cosA = ECos(rotation);
	double sinA = ESin(rotation);

	// There propably shouldn't be - origin so check it later I'm going to sleep now so
	double aAx = oAx*cosA - oAy*sinA + originX;
	double aBx = oBx*cosA - oBy*sinA + originX;
	double aCx = oCx*cosA - oCy*sinA + originX;
	double aAy = oAx*sinA + oAy*cosA + originY;
	double aBy = oBx*sinA + oBy*cosA + originY;
	double aCy = oCx*sinA + oCy*cosA + originY;

	int minX = (int)floor(fmin(aAx, fmin(aBx, aCx)));
	int maxX = (int)ceil(fmax(aAx, fmax(aBx, aCx)));
	int minY = (int)floor(fmin(aAy, fmin(aBy, aCy)));
	int maxY = (int)ceil(fmax(aAy, fmax(aBy, aCy)));

	for(int y = minY; y <= maxY; y++) {
		for(int x = minX; x <= maxX; x++) {
			double e1 = (x - aAx)*(aBy - aAy) - (y - aAy)*(aBx - aAx);
			double e2 = (x - aBx)*(aCy - aBy) - (y - aBy)*(aCx - aBx);
			double e3 = (x - aCx)*(aAy - aCy) - (y - aCy)*(aAx - aCx);
		
			if((e1 >= 0 && e2 >= 0 && e3 >= 0) || (e1 <= 0 && e2 <= 0 && e3 <= 0)) {
				DrawCharEx(character, x, y, fg, bg);
			}
		}
	}
}
