#include "../../include/esclib.h"

// =================== Universal libs ====================
#include <stdio.h>
#include <stdbool.h>


// =================== Linux / macOS =====================
#if defined(__APPLE__) || defined(__linux__)

#include <unistd.h>
#include <sys/ioctl.h>


void FillScreen(color Color) {
	struct winsize sizers;

	// Get that mesures
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &sizers);

	// Chainge color of background in terminal
	printf("\033[48;2;%d;%d;%dm", Color.red, Color.green, Color.blue);

	for (int i = 0; i < sizers.ws_row; i++) {
		for (int j = 0; j < sizers.ws_col; j++)
            putchar(' ');
        putchar('\n');
	}
}

// ===================== Windows =======================
#elif defined(_WIN32) || defined(_WIN64)

#include <windows.h>


// TODO: Chatgbt port, litteraly FillScreen for linux ctrl+c ctrl+v to chatgbt. Why? cuz fuck you. I'll do it myself someday.
void FillScreen(color Color) {
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
        for (int j = 0; j < cols; j++)
            putchar(' ');
        putchar('\n');
    }
}

#endif

bool AlternateBuffModeActive = false;

// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
void InitTui(int fps) {

}

void ChaingeBg(color Color) {
	printf("\033[48;2;%d;%d;%dm", Color.red, Color.green, Color.blue);
}

void ClearAndFillScreen(color Color) {
	TuiClearScreen();
	FillScreen(Color);
}

void TuiClearScreen() {
	printf("\033[2J\033[H");
	fflush(stdout);
}

void TuiClearLine() {
	printf("\r\033[2K");
	fflush(stdout);
}

void TuiClearChar() {
	printf("\033[D\033[K");
	fflush(stdout);
}

void TuiEnableBuffMode() {
	printf("\033[?1049h");
	fflush(stdout);
}

void TuiDisableBuffMode() {
	printf("\033[?1049l");
	fflush(stdout);
}

void TuiSwitchBuffMode() {
	if(!AlternateBuffModeActive) {
		TuiEnableBuffMode();
		AlternateBuffModeActive = true;
	}
	else {
		TuiDisableBuffMode();
		AlternateBuffModeActive = false;
	}
}

