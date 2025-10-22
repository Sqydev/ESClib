#include "../../include/tuiutils.h"
#include "../PrivateErrorProtocols.h"

#include <stdlib.h>
// TODO: MAKE IT MULTIPLATFORM
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

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
