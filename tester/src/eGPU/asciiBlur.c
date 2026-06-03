#include "../../../include/esclib.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

unsigned char* input = NULL;
unsigned char* output = NULL;
int current_w = 0;
int current_h = 0;

const char* ramp = " `.,;'!^-~:=+?><|(){}[]1ilI7JcjsLtfozxnYuvCXUZO08QmwqpdbkhKAFBEPGHRS#&D$M%W@N";
int ramp_len = 0;

typedef enum { MODE_BLUR = 0, MODE_EDGE, MODE_SHARPEN, MODE_COUNT } Mode;
typedef enum { PAT_XOR = 0, PAT_RINGS, PAT_PLASMA, PAT_WAVE, PAT_COUNT } Pattern;

const char* mode_names[] = { "Gaussian Blur", "Edge Detect", "Sharpen" };
const char* pat_names[] = { "XOR", "Rings", "Plasma", "Wave" };

Mode current_mode = MODE_BLUR;
Pattern current_pat = PAT_XOR;
bool paused = false;
float time_acc = 0.0f;

Color val_to_color(int value) {
    Color color;

	color.trueColor = true;
    if(value < 64) {
		color.r = 0;
		color.g = value * 2;
		color.b = 128 + value;
	}
    else if(value < 128) {
		color.r = 0;
		color.g = 128 + (value - 64) * 2;
		color.b = 192 - (value - 64) * 2;
	}
    else if(value < 192) {
		color.r = (value - 128) * 4;
		color.g = 255;
		color.b = 0;
	}
    else {
		color.r = 255;
		color.g = 255 - (value - 192) * 4;
		color.b = 0;
	}
    return color;
}

void gen_xor(float time, int width, int height) {
    int off = (int)(time * 20.0f);
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) { 
            input[y * width + x] = (unsigned char)( ( ( (x + off)^y ) * 255 ) / (width + height) ); 
        }
    }
}

void gen_rings(float time, int width, int height) {
    float cx = width / 2.0f + sinf(time * 0.7f) * width * 0.25f;
    float cy = height / 2.0f + cosf(time * 0.5f) * height * 0.25f;

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            float dx = (x - cx) * 0.5f;
			float dy = (float)(y - cy);
            float v = ( sinf( sqrtf(dx * dx + dy * dy) * 0.6f - time * 3.0f ) + 1.0f ) * 0.5f;
            input[y * width + x] = (unsigned char)(v * 255.0f);
        }
	}
}

void gen_plasma(float time, int width, int height) {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            float fx = x * 0.08f;
			float fy = y * 0.15f;
            float v = sinf(fx + time) + sinf(fy + time * 1.3f) + sinf((fx + fy) * 0.5f + time * 0.8f) + sinf(sqrtf(fx * fx + fy * fy) + time);
            input[y * width + x] = (unsigned char)(( (v / 4.0f) + 1.0f ) * 0.5f * 255.0f);
        }
	}
}

void gen_wave(float time, int width, int height) {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            float v = sinf(x * 0.2f + time * 2.0f) * cosf(y * 0.35f + time) * 0.5f + 0.5f;
            input[y * width + x] = (unsigned char)(v * 255.0f);
        }
	}
}

void update_pattern(float time, int width, int height) {
    switch(current_pat) {
        case PAT_XOR: { gen_xor(time, width, height); break; }
        case PAT_RINGS: { gen_rings(time, width, height); break; }
        case PAT_PLASMA: { gen_plasma(time, width, height); break; }
        case PAT_WAVE: { gen_wave(time, width, height); break; }
        default: { break; }
    }
}

void render(unsigned char* buf, int width, int height) {
    BeginDrawing();

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int v = buf[y * width + x];
            int ci = v * (ramp_len - 1) / 255;
            char ch[2] = { ramp[ci], 0 };
            Color fg = val_to_color(v);
            Color bg = { 0, 0, 0, true };
            DrawCharEx(ch, x, y, &fg, &bg);
        }
    }

    Color hud_fg = { 255, 220, 50, true };
    Color hud_bg = { 20, 20, 40, true };
    double dt = GetDeltaTime();
    double fps = dt > 0.0 ? 1.0 / dt : 0.0;
    
    DrawTextfEx(" FPS: %4.0f | dt: %.2fms | Mode: %-14s | Pat: %-7s | %s ", 0, height, &hud_fg, &hud_bg, fps, dt * 1000.0, mode_names[current_mode], pat_names[current_pat], paused ? "PAUSED" : "LIVE");

    Color key_fg = { 150, 150, 200, true };
    DrawTextfEx(" [M] mode  [P] pattern  [SPACE] pause  [ESC] quit ", 0, height + 1, &key_fg, &hud_bg);

    EndDrawing();
}

static const char* kernel_paths[] = {
    "./tester/src/kernels/asciiBlur.cl",
    "./tester/src/kernels/asciiEdge.cl",
    "./tester/src/kernels/asciiSharpen.cl",
};
static const char* kernel_names[] = { "blur", "edge", "sharpen" };

void PatternsGPUTest(void) {
    ramp_len = (int)strlen(ramp);

    InitTui(165, TUI_DYNAMIC);

    Kernel* kernels[MODE_COUNT] = { NULL, NULL, NULL };
    for(int i = 0; i < MODE_COUNT; i++) {
        kernels[i] = CompileKernel(kernel_paths[i], kernel_names[i]);
    }

    if(!kernels[MODE_BLUR]) {
        CloseTui();
        printf("Critical: kernel didn't load for some reason. RUN FROM PROJECT ROOT.\n");
        return;
    }
  
    for (;;) {
        if(IsKeyPressed(KEY_ESCAPE)) { break; }

        if(IsKeyPressed(KEY_M)) {
            int tries = 0;
            do {
                current_mode = (Mode)((current_mode + 1) % MODE_COUNT);
                tries++;
            } while(!kernels[current_mode] && tries < MODE_COUNT);
        }
        if(IsKeyPressed(KEY_P)) {
            current_pat = (Pattern)((current_pat + 1) % PAT_COUNT);
		}
        if(IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
		}

        if(!paused) {
            time_acc += (float)GetDeltaTime();
		}

        Vector2i dims = GetTuiDimensions();
        int new_w = dims.x;
        int new_h = dims.y - 2;

        if(new_w <= 0 || new_h <= 0) {
            continue; 
        }

        if(new_w != current_w || new_h != current_h) {
            current_w = new_w;
            current_h = new_h;
            size_t new_size = current_w * current_h * sizeof(unsigned char);
            
            input = realloc(input, new_size);
            output = realloc(output, new_size);
            
            if(!input || !output) {
                break;
            }
        }

        update_pattern(time_acc, current_w, current_h);

	Kernel* k = kernels[current_mode];

    size_t buf_size = current_w * current_h * sizeof(unsigned char);

    AddKernelArgBuffer(k, 0, buf_size, input);
    AddKernelArgBuffer(k, 1, buf_size, output);
    AddKernelArgValue(k, 2, sizeof(int), &current_w);
    AddKernelArgValue(k, 3, sizeof(int), &current_h);
        


        RunKernel(k, current_w * current_h, 0);
        WaitForKernel();
        ReadKernelArg(k, 1, buf_size, output);

        render(output, current_w, current_h);
    }

    free(input);
    free(output);

    for(int i = 0; i < MODE_COUNT; i++) { if(kernels[i]) { DestroyKernel(kernels[i]); } }

    CloseTui();
}
