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
#include "./private/common_utils.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "linclude/stb_image.h"

static int level256(int value) {
	if(value < 48) { return 0; }
	if(value < 115) { return 1; }
	if(value < 155) { return 2; }
	if(value < 195) { return 3; }
	if(value < 235) { return 4; }
	return 5;
}

static bool is_grayscale(unsigned char r, unsigned char g, unsigned char b) {
	return abs((int)r - (int)g) < 8 && abs((int)g - (int)b) < 8 && abs((int)r - (int)b) < 8;
}

static int grayscale256(unsigned char value) {
	int index = ((int)value - 8 + 5) / 10;

	if(index < 0) { index = 0; }
	if(index > 23) { index = 23; }

	return 232 + index;
}

Texture LoadTexture(const char* path, TextureType type) {
	Texture texture = { 0 };

	if(!path) {
		errno = EINVAL;
		TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load the texture becouse path is NULL");
		return texture;
	}

	int width;
	int height;
	int channels;

	unsigned char* data;

	switch(type) {
		case TEXTURE_MONO: {
			data = stbi_load(path, &width, &height, &channels, 2);
			if(!data) {
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}

			unsigned char* new_data = malloc((size_t)width * height * sizeof(unsigned char) * 2);
			if(!new_data) {
				stbi_image_free(data);
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}

			size_t idx = 0;
			for(size_t i = 0; i < (size_t)width * height * 2; i += 2) {
				new_data[idx++] = grayscale256(data[i]);
				new_data[idx++] = data[i + 1] >= 128 ? 255 : 0;
			}

			stbi_image_free(data);
			data = new_data;

			break;
		}
		case TEXTURE_8COLOR: {
			data = stbi_load(path, &width, &height, &channels, 4);
			if(!data) {
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}
			
			unsigned char* new_data = malloc((size_t)width * height * sizeof(unsigned char) * 2);
			if(!new_data) {
				stbi_image_free(data);
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}

			size_t idx = 0;
			for(size_t i = 0; i < (size_t)width * height * 4; i += 4) {
				bool r = data[i] >= 128;
				bool g = data[i + 1] >= 128;
				bool b = data[i + 2] >= 128;

				new_data[idx++] = r | (g << 1) | (b << 2);
				new_data[idx++] = data[i + 3] >= 128 ? 255 : 0;
			}

			stbi_image_free(data);
			data = new_data;

			break;
		}
		case TEXTURE_16COLOR: {
			data = stbi_load(path, &width, &height, &channels, 4);
			if(!data) {
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}
			
			unsigned char* new_data = malloc((size_t)width * height * sizeof(unsigned char) * 2);
			if(!new_data) {
				stbi_image_free(data);
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}

			size_t idx = 0;
			for(size_t i = 0; i < (size_t)width * height * 4; i += 4) {
				bool r = data[i] >= 128;
				bool g = data[i + 1] >= 128;
				bool b = data[i + 2] >= 128;

				unsigned char base = r | (g << 1) | (b << 2);
				unsigned char brightness = (data[i] + data[i + 1] + data[i + 2]) / 3 >= 128;

				new_data[idx++] = base + (brightness ? 8 : 0);
				new_data[idx++] = data[i + 3] >= 128 ? 255 : 0;
			}

			stbi_image_free(data);
			data = new_data;

			break;
		}
		case TEXTURE_256COLOR: {
			data = stbi_load(path, &width, &height, &channels, 4);
			if(!data) {
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}
			
			unsigned char* new_data = malloc((size_t)width * height * sizeof(unsigned char) * 2);
			if(!new_data) {
				stbi_image_free(data);
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}

			size_t idx = 0;
			for(size_t i = 0; i < (size_t)width * height * 4; i += 4) {
				unsigned char r = data[i];
				unsigned char g = data[i + 1];
				unsigned char b = data[i + 2];

				if(is_grayscale(r, g, b)) {
					new_data[idx++] = grayscale256(r);
				}
				else {
					new_data[idx++] = 16 + 36 * level256(r) + 6 * level256(g) + level256(b);
				}
				new_data[idx++] = data[i + 3] >= 128 ? 255 : 0;
			}

			stbi_image_free(data);
			data = new_data;

			break;
		}
		case TEXTURE_TRUECOLOR: {
			data = stbi_load(path, &width, &height, &channels, 4);
			if(!data) {
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}

			unsigned char* new_data = malloc((size_t)width * height * sizeof(unsigned char) * 4);
			if(!new_data) {
				stbi_image_free(data);
				errno = ENOMEM;
				TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Failed to load texture from path %s because of alloc issue", path);
				return texture;
			}

			memcpy(new_data, data, (size_t)width * height * 4);

			stbi_image_free(data);
			data = new_data;

			break;
		}
		default: {
			errno = EINVAL;
			TraceLog(LOG_DEBUG, "[ESCLIB.LoadTexture]: ERROR Couldn't load because texture type is wrong");
			return texture;
		}
	}

	texture.data = data;
	texture.type = type;
	texture.id = DATA.Textures.lastId++;
	texture.width = width;
	texture.height = height;

	return texture;
}

void FreeTexture(Texture* texture) {
	if(!texture || !texture->data) { return; }
	
	free(texture->data);

	texture->data = NULL;
	texture->type = 0;
	texture->id = 0;
	texture->width = 0;
	texture->height = 0;
}

Color GetTexturePixel(Texture* texture, size_t x, size_t y) {
	if(texture->type == TEXTURE_TRUECOLOR) { return (Color){ texture->data[((y - 1) * texture->width) + x], texture->data[((y - 1) * texture->width) + x + 1], texture->data[((y - 1) * texture->width) + x + 2], texture->data[((y - 1) * texture->width) + x + 3], true}; }
	return (Color){ texture->data[((y - 1) * texture->width) + x], 0, 0, texture->data[((y - 1) * texture->width) + x + 1], false };
}

void DrawTextureCore(Texture* texture, char* character, Rectangle rec, Rectangle textureSlice, int originX, int originY, double rotation, Color tint, bool affectFg, bool affectBg, ScalingAlgorithms scaling, bool aspectRatiofied, bool isPanel, Panel panel) {
	double aspectRatio = aspectRatiofied ? ((GetCellProportions().x > 0 && GetCellProportions().y > 0) ? (double)GetCellProportions().x / (double)GetCellProportions().y : 0.5) : 1.0;

	if(isPanel) { 
		rec.x += panel.x; 
		rec.y += panel.y; 
	}

	double cosA = ECos(rotation);
	double sinA = ESin(rotation);
	double widthInPixels = rec.width * aspectRatio;
	double maxRadius = sqrt(widthInPixels * widthInPixels + rec.height * rec.height) / aspectRatio + 1;

	int startX = rec.x - (int)maxRadius;
	int endX = rec.x + (int)maxRadius;
	int startY = rec.y - (int)maxRadius;
	int endY = rec.y + (int)maxRadius;

	int minBoundX = isPanel ? panel.x : 0;
	int minBoundY = isPanel ? panel.y : 0;
	int maxBoundX = isPanel ? panel.x + panel.width - 1 : GetLastTuiIndex().x;
	int maxBoundY = isPanel ? panel.y + panel.height - 1 : GetLastTuiIndex().y;

	if(isPanel && (endX < panel.x || endY < panel.y || startX >= panel.x + panel.width || startY >= panel.y + panel.height)) {
		return;
	}

	if(startX < minBoundX) {
		startX = minBoundX;
	}
	if(startY < minBoundY) {
		startY = minBoundY;
	}
	if(endX >= maxBoundX) {
		endX = maxBoundX;
	}
	if(endY >= maxBoundY) {
		endY = maxBoundY;
	}

	double rpx = (double)GetCellSizeInPixels().x * rec.width;
	double rpy = (double)GetCellSizeInPixels().y * rec.height;
	
	int vWidth = GetCharWidth(character);

	for(int y = startY; y <= endY; y++) {
		for(int x = startX; x <= endX; x++) {
			if((x - rec.x) % vWidth != 0) {
				continue;
			}

			double dx = (x + (vWidth / 2.0) - rec.x) * aspectRatio;
			double dy = (y + 0.5 - rec.y);

			double ox = (dx * cosA) + (dy * sinA);
			double oy = -(dx * sinA) + (dy * cosA);
			double srcX = ox / aspectRatio + originX;
			double srcY = oy + originY;

			if(srcX < 0.0 || srcX >= (double)rec.width || srcY < 0.0 || srcY >= (double)rec.height) {
				continue;
			}

			DrawCharEx(character, x, y, fg, bg);
		}
	}
}

void DrawTexture(Texture* texture, Rectangle rec) {
	DrawTextureEx(texture, " ", rec, TERMBLANK, false, true);
}

void DrawTextureEx(Texture* texture, char* character, Rectangle rec, Color tint, bool affectFg, bool affectBg) {
	DrawTexturePro(texture, character, rec, (Rectangle){ 0, 0, texture->width, texture->height }, 0, 0, 0, tint, affectFg, affectBg, SCALEING_BILINEAR, true);
}

void DrawTexturePro(Texture* texture, char* character, Rectangle rec, Rectangle textureSlice, int originX, int originY, double rotation, Color tint, bool affectFg, bool affectBg, ScalingAlgorithms scaling, bool aspectRatiofied) {
	DrawTextureCore(texture, character, rec, textureSlice, originX, originY, rotation, tint, affectFg, affectBg, scaling, aspectRatiofied, false, (Panel){ 0 });
}
