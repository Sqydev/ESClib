#include "../../include/esclib.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(__APPLE__) || defined(__linux__)

#include <unistd.h>
#include <sys/ioctl.h>

#elif defined(_WIN32) || defined(_WIN64)

#include <windows.h>

#endif


typedef struct CoreData {
	struct {
		color backgroundColor;
		bool alternateBuffModeActive;
	} Window;
	struct {
		unsigned int targetFPS;
	} Time;
} CoreData;

CoreData CORE = { 0 };

// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
void InitTui(unsigned int fps) {
	CORE.Time.targetFPS = fps;

	CORE.Window.backgroundColor = (color){0, 0, 0};

	EnableBufferMode();

	EnableRawMode();

	ClearScreen();

	FillScreen(CORE.Window.backgroundColor);
}

void SetCursorPosition(int x, int y) {
	printf("\033[%d;%dH", y, x);
	fflush(stdout);
}

void MoveCursorDirectional(int up, int down, int left, int right) {
	if (up > 0) printf("\033[%dA", up);
	if (down > 0) printf("\033[%dB", down);
	if (right > 0) printf("\033[%dC", right);
	if (left > 0) printf("\033[%dD", left);
	fflush(stdout);
}

void MoveCursor(int x, int y) {
	if (y < 0) printf("\033[%dA", -y);
	if (y > 0) printf("\033[%dB", y);
	if (x > 0) printf("\033[%dC", x);
	if (x < 0) printf("\033[%dD", -x);
	fflush(stdout);
}

void FillScreen(color Color) {
	#if defined(__APPLE__) || defined(__linux__)


		struct winsize sizers;

		// Get that mesures
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &sizers);

		// Chainge color of background in terminal
		printf("\033[48;2;%d;%d;%dm", Color.red, Color.green, Color.blue);

		for (int i = 0; i < sizers.ws_row; i++) {
			for (int j = 0; j < sizers.ws_col; j++) {
            	putchar(' ');
			}
        	putchar('\n');
		}

		printf("\033[48;2;%d;%d;%dm", CORE.Window.backgroundColor.red, CORE.Window.backgroundColor.green, CORE.Window.backgroundColor.blue);


	#elif defined(_WIN32) || defined(_WIN64)
	// TODO: Chatgbt port, litteraly FillScreen for linux ctrl+c ctrl+v to chatgbt. Why? cuz fuck microsoft. I'll do it myself someday.


		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    	DWORD dwMode = 0;
    	GetConsoleMode(hOut, &dwMode);
    	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    	SetConsoleMode(hOut, dwMode);

    	CONSOLE_SCREEN_BUFFER_INFO csbi;
    	GetConsoleScreenBufferInfo(hOut, &csbi);
    	int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    	int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    	printf("\033[48;2;%d;%d;%dm", color.red, color.green, color.blue);

    	for (int i = 0; i < rows; i++) {
        	for (int j = 0; j < cols; j++) {
            	putchar(' ');
			}
        	putchar('\n');
    	}

		printf("\033[48;2;%d;%d;%dm", CORE.Window.backgroundColor.red, CORE.Window.backgroundColor.green, CORE.Window.backgroundColor.blue);


	#endif
}

void SetBackgroundColor(color Color) {
	printf("\033[48;2;%d;%d;%dm", Color.red, Color.green, Color.blue);
}

void ClearBackground(color Color) {
	ClearScreen();
	FillScreen(Color);
}

void ClearScreen(void) {
	printf("\033[2J\033[H");
	fflush(stdout);
}

void ClearLine(void) {
	printf("\r\033[2K");
	fflush(stdout);
}

void ClearChar(void) {
	printf("\033[D\033[K");
	fflush(stdout);
}

void EnableBufferMode(void) {
	printf("\033[?1049h");
	fflush(stdout);

	atexit(DisableBufferMode);
}

void DisableBufferMode(void) {
	printf("\033[?1049l");
	fflush(stdout);
}

void ToggleBufferMode(void) {
	if(!CORE.Window.alternateBuffModeActive) {
		EnableBufferMode();
		CORE.Window.alternateBuffModeActive = true;
	}
	else {
		DisableBufferMode();
		CORE.Window.alternateBuffModeActive = false;
	}
}
