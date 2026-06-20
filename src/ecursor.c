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

void SetCursorPositionV(Vector2i pos) {
	DATA.Cursor.pos = pos;
	DATA.Cursor.oob = (pos.x < 0 || pos.y < 0) || (pos.x > GetLastTuiIndex().x || pos.y > GetLastTuiIndex().y);
}
 
// TODO: MAKE CHECKING ALSO AFTER. LIKE GetLastTuiIndex. yk
void SetCursorPosition(int posX, int posY) {
	DATA.Cursor.pos.x = posX;
	DATA.Cursor.pos.y = posY;
	DATA.Cursor.oob = (posX < 0 || posY < 0) || (posX > GetLastTuiIndex().x || posY > GetLastTuiIndex().y);
}
 
void MoveCursorV(Vector2i pos) {
	DATA.Cursor.pos.x += pos.x;
	DATA.Cursor.pos.y += pos.y;
	DATA.Cursor.oob = (DATA.Cursor.pos.x < 0 || DATA.Cursor.pos.y < 0) || (pos.x > GetLastTuiIndex().x || pos.y > GetLastTuiIndex().y);
}
 
void MoveCursor(int posX, int posY) {
	DATA.Cursor.pos.x += posX;
	DATA.Cursor.pos.y += posY;
	DATA.Cursor.oob = (DATA.Cursor.pos.x < 0 || DATA.Cursor.pos.y < 0) || (posX > GetLastTuiIndex().x || posY > GetLastTuiIndex().y);
}
 
void HideCursor(void) {
	DATA.Cursor.hidden = true;
}
 
void ShowCursor(void) {
	DATA.Cursor.hidden = false;
}

