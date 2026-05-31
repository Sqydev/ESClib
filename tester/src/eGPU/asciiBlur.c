#include "../../../include/esclib.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#define W 120
#define H 38

static unsigned char input[W * H];
static unsigned char output[W * H];

static const char* ramp = " `.,;'!^-~:=+?><|(){}[]1ilI7JcjsLtfozxnYuvCXUZO08QmwqpdbkhKAFBEPGHRS#&D$M%W@N";
static int ramp_len = 0;

typedef enum { MODE_BLUR = 0, MODE_EDGE, MODE_SHARPEN, MODE_COUNT } Mode;
typedef enum { PAT_XOR = 0, PAT_RINGS, PAT_PLASMA, PAT_WAVE, PAT_COUNT } Pattern;

static const char* mode_names[] = { "Gaussian Blur", "Edge Detect", "Sharpen" };
static const char* pat_names[]  = { "XOR", "Rings", "Plasma", "Wave" };

static Mode    current_mode = MODE_BLUR;
static Pattern current_pat  = PAT_XOR;
static bool    paused       = false;
static float   time_acc     = 0.0f;

static Color val_to_color(int v) {
    Color c; c.trueColor = true;
    if      (v < 64)  { c.r = 0;   c.g = v * 2;          c.b = 128 + v; }
    else if (v < 128) { c.r = 0;   c.g = 128+(v-64)*2;   c.b = 192-(v-64)*2; }
    else if (v < 192) { c.r = (v-128)*4; c.g = 255;       c.b = 0; }
    else              { c.r = 255; c.g = 255-(v-192)*4;   c.b = 0; }
    return c;
}

static void gen_xor(float t) {
    int off = (int)(t * 20.0f);
    for(int y = 0; y < H; y++) {
        for(int x = 0; x < W; x++) { input[y*W+x] = (unsigned char)(((x+off)^y)*255/(W+H)); }
	}
}
static void gen_rings(float t) {
    float cx = W/2.0f + sinf(t*0.7f)*W*0.25f;
    float cy = H/2.0f + cosf(t*0.5f)*H*0.25f;
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            float dx=(x-cx)*0.5f, dy=(float)(y-cy);
            float v=(sinf(sqrtf(dx*dx+dy*dy)*0.6f-t*3.0f)+1.0f)*0.5f;
            input[y*W+x]=(unsigned char)(v*255.0f);
        }
}
static void gen_plasma(float t) {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            float fx=x*0.08f, fy=y*0.15f;
            float v=sinf(fx+t)+sinf(fy+t*1.3f)+sinf((fx+fy)*0.5f+t*0.8f)+sinf(sqrtf(fx*fx+fy*fy)+t);
            input[y*W+x]=(unsigned char)((v/4.0f+1.0f)*0.5f*255.0f);
        }
}
static void gen_wave(float t) {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            float v=sinf(x*0.2f+t*2.0f)*cosf(y*0.35f+t)*0.5f+0.5f;
            input[y*W+x]=(unsigned char)(v*255.0f);
        }
}
static void update_pattern(float t) {
    switch(current_pat) {
        case PAT_XOR:    gen_xor(t);    break;
        case PAT_RINGS:  gen_rings(t);  break;
        case PAT_PLASMA: gen_plasma(t); break;
        case PAT_WAVE:   gen_wave(t);   break;
        default: break;
    }
}

static void render(unsigned char* buf) {
    BeginDrawing();

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            int v = buf[y*W+x];
            int ci = v * (ramp_len-1) / 255;
            char ch[2] = { ramp[ci], 0 };
            Color fg = val_to_color(v);
            Color bg = { 0, 0, 0, true };
            DrawCharEx(ch, x, y, &fg, &bg);
        }
    }

    Color hud_fg = { 255, 220, 50, true };
    Color hud_bg = { 20, 20, 40, true };
    double dt  = GetDeltaTime();
    double fps = dt > 0.0 ? 1.0/dt : 0.0;
    DrawTextfEx(" FPS: %4.0f | dt: %.2fms | Mode: %-14s | Pat: %-7s | %s ",
        0, H, &hud_fg, &hud_bg,
        fps, dt*1000.0, mode_names[current_mode], pat_names[current_pat],
        paused ? "PAUSED" : "LIVE");

    Color key_fg = { 150, 150, 200, true };
    DrawTextfEx(" [M] mode  [P] pattern  [SPACE] pause  [ESC] quit ",
        0, H+1, &key_fg, &hud_bg);

    EndDrawing();
}

static const char* kernel_paths[] = {
    "./tester/src/kernels/asciiBlur.cl",
    "./tester/src/kernels/asciiEdge.cl",
    "./tester/src/kernels/asciiSharpen.cl",
};
static const char* kernel_names[] = { "blur", "edge", "sharpen" };

void BlurGPUTest(void) {
    ramp_len = (int)strlen(ramp);

    InitTui(165, TUI_DYNAMIC);

    Kernel* kernels[MODE_COUNT] = { NULL, NULL, NULL };
    for (int i = 0; i < MODE_COUNT; i++) {
        kernels[i] = CompileKernel(kernel_paths[i], kernel_names[i]);
    }

    if (!kernels[MODE_BLUR]) {
        CloseTui();
        printf("Critical: blur kernel failed. Uruchom z katalogu roota.\n");
        return;
    }

    int width  = W;
    int height = H;

    for (;;) {
        if (IsKeyPressed(KEY_ESCAPE)) break;

        if (IsKeyPressed(KEY_M)) {
            int tries = 0;
            do {
                current_mode = (Mode)((current_mode+1) % MODE_COUNT);
                tries++;
            } while (!kernels[current_mode] && tries < MODE_COUNT);
        }
        if (IsKeyPressed(KEY_P))
            current_pat = (Pattern)((current_pat+1) % PAT_COUNT);
        if (IsKeyPressed(KEY_SPACE))
            paused = !paused;

        if (!paused)
            time_acc += (float)GetDeltaTime();

        update_pattern(time_acc);

        Kernel* k = kernels[current_mode];

        AddKernelArgBuffer(k, 0, sizeof(input),  input);
        AddKernelArgBuffer(k, 1, sizeof(output), output);
        AddKernelArgValue(k, 2, sizeof(int),    &width);
        AddKernelArgValue(k, 3, sizeof(int),    &height);
        RunKernel(k, W*H, 0);
        WaitForKernel();
		ReadKernelArg(k, 1, sizeof(output), output);

        render(output);
    }

    for (int i = 0; i < MODE_COUNT; i++)
        if (kernels[i]) DestroyKernel(kernels[i]);
    CloseTui();
}
