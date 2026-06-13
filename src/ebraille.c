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
#include "./private/signals/signals_actions.h"
#include "./private/common_utils.h"
#include "./private/renderFrame.h"
#include "./private/input/input.h"

#include <errno.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

char* MergeBraille(const char* a, const char* b) {
    if(!a || !b) {
        errno = EINVAL;
        return NULL;
    }

    if(a[3] != '\0' || b[3] != '\0') {
        errno = EINVAL;
        return NULL;
    }

    uint32_t cp1 = ((a[0] & 0x0F) << 12) | ((a[1] & 0x3F) << 6) | (a[2] & 0x3F);

    uint32_t cp2 = ((b[0] & 0x0F) << 12) | ((b[1] & 0x3F) << 6) | (b[2] & 0x3F);

    if(cp1 < 0x2800 || cp1 > 0x28FF || cp2 < 0x2800 || cp2 > 0x28FF) {
        errno = EINVAL;
        return NULL;
    }

    uint32_t merged = 0x2800 | ((cp1 - 0x2800) | (cp2 - 0x2800));

    char* out = malloc(4 * sizeof(char));
    if(!out) { return NULL; }

    out[0] = 0xE0 | ((merged >> 12) & 0x0F);
    out[1] = 0x80 | ((merged >> 6) & 0x3F);
    out[2] = 0x80 | (merged & 0x3F);
    out[3] = '\0';

    return out;
}
