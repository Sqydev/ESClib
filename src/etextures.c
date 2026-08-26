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
#include <math.h>
#include <stdbool.h>
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
	if(texture->type == TEXTURE_TRUECOLOR) {
		size_t idx = (y * texture->width + x) * 4;
		return (Color){ texture->data[idx], texture->data[idx + 1], texture->data[idx + 2], texture->data[idx + 3], true};
	}
	size_t idx = (y * texture->width + x) * 2;
	return (Color){ texture->data[idx], 0, 0, texture->data[idx + 1], false };
}

void DrawTextureCore(Texture *texture, char *character, Rectangle rec, Rectangle textureSlice, int originX, int originY, double rotation, Color tint, bool affectFg, bool affectBg, ScalingAlgorithms scaling, bool aspectRatiofied, bool isPanel, Panel panel) {
	double aspectRatio = aspectRatiofied ? ((GetCellProportions().x > 0 && GetCellProportions().y > 0) ? (double)GetCellProportions().x / (double)GetCellProportions().y : 0.5) : 1.0;
	if(aspectRatiofied) { rec.width /= aspectRatio; }

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

	int vWidth = GetCharWidth(character);

	for(int y = startY; y <= endY; y++) {
		for(int x = startX; x <= endX; x++) {
			if((x - rec.x) % vWidth != 0) {
				continue;
			}

			double textureScaleX = (double)(rec.width * aspectRatio) / (double)textureSlice.width;
			double textureScaleY = (double)rec.height / (double)textureSlice.height;

			double originScreenX = (double)originX * textureScaleX;
			double originScreenY = (double)originY * textureScaleY;

			double dx = (x + (vWidth / 2.0) - rec.x) * aspectRatio - originScreenX;
			double dy = (y + 0.5 - rec.y) - originScreenY;

			double ox = (dx * cosA) + (dy * sinA);
			double oy = -(dx * sinA) + (dy * cosA);

			double srcX = (double)originX + ox / textureScaleX;
			double srcY = (double)originY + oy / textureScaleY;

			if(srcX < 0.0 || srcX >= (double)textureSlice.width || srcY < 0.0 || srcY >= (double)textureSlice.height) {
				continue;
			}

			double texPixelXf = textureSlice.x + srcX;
			double texPixelYf = textureSlice.y + srcY;

			Color sampled;

			if(texture->type != TEXTURE_TRUECOLOR || scaling == SCALEING_NEAREST_NEIGHBOR) {
				long tpx = (long)round(texPixelXf);
				long tpy = (long)round(texPixelYf);
				if(tpx < 0) { tpx = 0; }
				if(tpy < 0) { tpy = 0; }
				if(tpx >= (long)texture->width) { tpx = (long)texture->width - 1; }
				if(tpy >= (long)texture->height) { tpy = (long)texture->height - 1; }

				sampled = GetTexturePixel(texture, tpx, tpy);
			}
			else if(scaling == SCALEING_BILINEAR) {
				long x0 = (long)floor(texPixelXf);
				long y0 = (long)floor(texPixelYf);
				double fx = texPixelXf - x0;
				double fy = texPixelYf - y0;

				long x0c = x0;
				long x1c = x0 + 1;
				long y0c = y0;
				long y1c = y0 + 1;
				if(x0c < 0) { x0c = 0; }
				if(x1c < 0) { x1c = 0; }
				if(y0c < 0) { y0c = 0; }
				if(y1c < 0) { y1c = 0; }
				if(x0c >= (long)texture->width) { x0c = (long)texture->width - 1; }
				if(x1c >= (long)texture->width) { x1c = (long)texture->width - 1; }
				if(y0c >= (long)texture->height) { y0c = (long)texture->height - 1; }
				if(y1c >= (long)texture->height) { y1c = (long)texture->height - 1; }

				Color c00 = GetTexturePixel(texture, (size_t)x0c, (size_t)y0c);
				Color c10 = GetTexturePixel(texture, (size_t)x1c, (size_t)y0c);
				Color c01 = GetTexturePixel(texture, (size_t)x0c, (size_t)y1c);
				Color c11 = GetTexturePixel(texture, (size_t)x1c, (size_t)y1c);

				double r = c00.r * (1 - fx) * (1 - fy) + c10.r * fx * (1 - fy) + c01.r * (1 - fx) * fy + c11.r * fx * fy;
				double g = c00.g * (1 - fx) * (1 - fy) + c10.g * fx * (1 - fy) + c01.g * (1 - fx) * fy + c11.g * fx * fy;
				double b = c00.b * (1 - fx) * (1 - fy) + c10.b * fx * (1 - fy) + c01.b * (1 - fx) * fy + c11.b * fx * fy;
				double a = c00.a * (1 - fx) * (1 - fy) + c10.a * fx * (1 - fy) + c01.a * (1 - fx) * fy + c11.a * fx * fy;

				sampled = (Color){ (unsigned char)(r + 0.5), (unsigned char)(g + 0.5), (unsigned char)(b + 0.5), (unsigned char)(a + 0.5), true };
			}
			else {
				long x0 = (long)floor(texPixelXf);
				long y0 = (long)floor(texPixelYf);
				double fx = texPixelXf - x0;
				double fy = texPixelYf - y0;

				double sumR = 0.0, sumG = 0.0, sumB = 0.0, sumA = 0.0;
				const double aCoef = -0.5;

				for(int j = -1; j <= 2; j++) {
					double ty = fabs((double)j - fy);
					double wy;
					if(ty <= 1.0) { wy = (aCoef + 2.0) * ty * ty * ty - (aCoef + 3.0) * ty * ty + 1.0; }
					else if(ty < 2.0) { wy = aCoef * ty * ty * ty - 5.0 * aCoef * ty * ty + 8.0 * aCoef * ty - 4.0 * aCoef; }
					else { wy = 0.0; }

					long sy = y0 + j;
					if(sy < 0) { sy = 0; }
					if(sy >= (long)texture->height) { sy = (long)texture->height - 1; }

					for(int i = -1; i <= 2; i++) {
						double tx = fabs((double)i - fx);
						double wx;
						if(tx <= 1.0) { wx = (aCoef + 2.0) * tx * tx * tx - (aCoef + 3.0) * tx * tx + 1.0; }
						else if(tx < 2.0) { wx = aCoef * tx * tx * tx - 5.0 * aCoef * tx * tx + 8.0 * aCoef * tx - 4.0 * aCoef; }
						else { wx = 0.0; }

						long sx = x0 + i;
						if(sx < 0) { sx = 0; }
						if(sx >= (long)texture->width) { sx = (long)texture->width - 1; }

						double w = wx * wy;
						Color c = GetTexturePixel(texture, (size_t)sx, (size_t)sy);

						sumR += c.r * w;
						sumG += c.g * w;
						sumB += c.b * w;
						sumA += c.a * w;

						sampled = (Color){
							(unsigned char)fmax(0.0, fmin(255.0, sumR + 0.5)),
							(unsigned char)fmax(0.0, fmin(255.0, sumG + 0.5)),
							(unsigned char)fmax(0.0, fmin(255.0, sumB + 0.5)),
							(unsigned char)fmax(0.0, fmin(255.0, sumA + 0.5)),
							true
						};
					}
				}
			}

			BlendColors(&sampled, tint);

			DrawCharEx(character, x, y, affectFg ? &sampled : NULL, affectBg ? &sampled : NULL);
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
