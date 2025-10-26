#include "../../include/tuiutils.h"

// =================== Universal libs ====================
#include <stdlib.h>

// =================== Linux / macOS =====================
#if defined(__APPLE__) || defined(__linux__)

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

// Enable raw input mode or whatever this is
// Soo, it's something new so there WILL be comments

// Struct to store previose term-i-o settings(usefull in DisableRawMode)
static struct termios orig_termios;

void EnableRawMode() {
	// This is our workspace
	struct termios raw;

	// Get current term settings
	tcgetattr(STDIN_FILENO, &orig_termios);
	raw = orig_termios;

	// Turn off echo && canon
	// Echo: Automaticly show wrote chars, so now if you type something it won't be auto shown
	// Cannon: Waiting until \n
	raw.c_lflag &= ~(ICANON | ECHO);

	// Turn off CR/LF mapping
	// CR: \r -> \n
	// LR: \n -> \r
	// So I'LL decite when to move cursor:)
	raw.c_iflag &= ~(ICRNL | INLCR);

	// Make it so read() returns when there is any byte(and waits 0 ms or s idk)
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;

	// Apply new settings
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);

	// Get current flags
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	
	// Set flags to current flags + O_NONBLOCK(So no waiting for \n to return)
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

	// Make sure we restore the terminal when program exits
	atexit(DisableRawMode);
}

// Disable it
void DisableRawMode() {
	// Restore original terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);

	// Get current flags
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	// Remove O_NONBLOCK bit (bitwise AND with flag negation)
	fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}



int GetKey() {
	int gotchar = getchar();
	// NOTHING
	if (gotchar == EOF) return -1;

	if(gotchar == '\033') {
		int afterthing = getchar();
		if(afterthing == '[' || afterthing == 'O') {
			// Here switch for all the esc sequences
		}
		return KEY_ESC;
	}

	return gotchar;
}


// ===================== Windows =======================
#elif defined(_WIN32) || defined(_WIN64)
// No comment's for windows, you can deducate what does what from the code after reading unix comments

#include <windows.h>

static DWORD orig_mode;

void EnableRawMode() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hIn, &orig_mode);

    DWORD raw = orig_mode;
    raw &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    raw |= ENABLE_PROCESSED_INPUT;

    SetConsoleMode(hIn, raw);
    atexit(DisableRawMode);
}

void DisableRawMode() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hIn, orig_mode);
}

int GetKey() {
	if (!_kbhit()) return -1;
	int gotchar = _getch();

	if(gotchar == '0' || gotchar == '224') {
		// Here switch for all the esc sequences
	}
}

#endif

// =================== Universal =====================
