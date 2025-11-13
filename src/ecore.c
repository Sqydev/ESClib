#include "../include/esclib.h"
#include "PrivateErrorProtocols.h"

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// EXPLANATIONCOMMENT1: Soo, I decited to make functions that are in 99% of cases used in init (like HideCursor) do write becouse BeginDrawing clears backbuffor so it wouldn't work AND becouse raylib does this the same way and if it works for raylib it should be ok :)

// Setup
#if defined(__APPLE__) || defined(__linux__)

	#include <unistd.h>
	#include <sys/ioctl.h>
	#include <termios.h>
	#include <fcntl.h>
	#include <time.h>

#elif defined(_WIN32) || defined(_WIN64)

	#include <windows.h>

#endif


typedef struct CoreData {
	struct {
		char* backBuffor;
		size_t lenght;
		size_t capacity;
	} Backbuffor;
	struct {
		color backgroundColor;
		color foregroundColor;
		fontStyle foregroundStyle;
		bool shouldClose;
		bool alternateBufferModeActive;
		bool rawModeActive;
	} Tui;
	struct {
		#if defined(__APPLE__) || defined(__linux__)

			struct termios defaultSettings;
			struct termios esclibSettings;

		#elif defined(_WIN32) || defined(_WIN64)

			DWORD defaultSettings;
			DWORD esclibSettings;

		#endif

		bool signalsOn;
	} Terminal;
	struct {
		vector2 currentPosition;
		intvector2 currentTerminalPosition;
		vector2 lockedPosition;
		bool hidden;
		bool locked;
	} Cursor;
	struct {
		double current;
		double previous;
		double delta;
		double target;
		unsigned int frameCounter;
	} Time;
} CoreData;

CoreData CORE = { 0 };




static void SignalThingies(int signal) {
	if(signal == SIGINT) {
		CloseTui();
		exit(0);
	}
}

// TODO: Check if it works on windows
void InitTui(int fps, bool DisableSignals) {

	#if defined(__APPLE__) || defined(__linux__)

		// This is our workspace
		struct termios raw;

		// Get current term settings
		tcgetattr(STDIN_FILENO, &CORE.Terminal.defaultSettings);
		raw = CORE.Terminal.defaultSettings;


	#elif defined(_WIN32) || defined(_WIN64)

    	DWORD dwMode = 0;

    	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    	GetConsoleMode(hIn, &CORE.Terminal.defaultSettings);

		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    	GetConsoleMode(hOut, &dwMode);

    	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    	DWORD raw = CORE.Terminal.defaultSettings;

	#endif

	if(!DisableSignals) {
		CORE.Terminal.signalsOn = true;

		signal(SIGINT, SignalThingies);

		#if defined(__APPLE__) || defined(__linux__)

			// Turn off echo && canon
			// Echo: Automaticly show wrote chars, so now if you type something it won't be auto shown
			// Cannon: Waiting until \n
			raw.c_lflag &= ~(ICANON | ECHO);

		#elif defined(_WIN32) || defined(_WIN64)

    		raw &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
		
		#endif
	}
	else {
		CORE.Terminal.signalsOn = false;

		#if defined(__APPLE__) || defined(__linux__)

			// Turn off echo && canon
			// Echo: Automaticly show wrote chars, so now if you type something it won't be auto shown
			// Cannon: Waiting until \n
			raw.c_lflag &= ~(ICANON | ECHO | ISIG);

		#elif defined(_WIN32) || defined(_WIN64)

    		raw &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
		
		#endif
	}

	#if defined(__APPLE__) || defined(__linux__)

		// Turn off CR/LF mapping
		// CR: \r -> \n
		// LR: \n -> \r
		// So I'LL decite when to move cursor:)
		raw.c_iflag &= ~(ICRNL | INLCR);

		// Make it so read() returns when there is even no bytes(and waits 0 ms or s idk)
		raw.c_cc[VMIN] = 0;
		raw.c_cc[VTIME] = 0;

		// Apply new settings
		tcsetattr(STDIN_FILENO, TCSANOW, &raw);

		// Get current flags
		int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	
		// Set flags to current flags + O_NONBLOCK(So no waiting for \n to return)
		fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

		CORE.Terminal.esclibSettings = raw;

	#elif defined(_WIN32) || defined(_WIN64)

    	raw |= ENABLE_PROCESSED_INPUT;

    	SetConsoleMode(hIn, raw);

		CORE.Terminal.esclibSettings = raw;
	
	#endif

	CORE.Time.current = GetTime();

	CORE.Time.previous = CORE.Time.current;

	CORE.Time.frameCounter = 0;

	CORE.Time.delta = 0;

	SetTargetFps(fps);

	CORE.Tui.backgroundColor = (color){0, 0, 0};
	CORE.Tui.foregroundColor = (color){0, 0, 0};
	CORE.Tui.foregroundStyle = STYLE_NORMAL;

	CORE.Tui.shouldClose = false;

	CORE.Cursor.hidden = false;

	// EnableBuffMode
	#if defined(__APPLE__) || defined(__linux__)
	
		if(write(STDOUT_FILENO, "\033[?1049h", 8) == -1) { 
			int tries = 0;

			while(tries < 10) {
				if(write(STDOUT_FILENO, "\033[?1049h", 8) != -1) { break; }
				else { tries++; }
			}

			if (tries == 10) {
    			fprintf(stderr, "Failed to write after 10 attempts\n");
			}
		}

	#elif defined(_WIN32) || defined(_WIN64)

		DWORD written = 0;
    	WriteFile(hConsole, "\033[?1049h", 8, &written, NULL);

	#endif
	CORE.Tui.alternateBufferModeActive = true;

	ClearScreen();

	SetCursorPosition(0, 0);

	SetLockedCursorPosition(1, 1);

	atexit(CloseTui);
}

void CloseTui(void) {
	// DO NOT FLUSH FRAME!!!!!!!!!!!!
	DisableBufferMode();


	#if defined(__APPLE__) || defined(__linux__)


		// Restore original terminal settings
		tcsetattr(STDIN_FILENO, TCSANOW, &CORE.Terminal.esclibSettings);

		// Get current flags
		int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
		// Remove O_NONBLOCK bit (bitwise AND with flag negation)
		fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);


	#elif defined(_WIN32) || defined(_WIN64)


    	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    	DWORD dwMode = 0;
    	GetConsoleMode(hOut, &dwMode);

    	SetConsoleMode(hIn, CORE.Terminal.settings);
    	SetConsoleMode(hOut, dwMode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING);


	#endif

	// DisableBuffMode
	#if defined(__APPLE__) || defined(__linux__)

		if(write(STDOUT_FILENO, "\033[?1049l", 8) == -1) { 
			int tries = 0;
	
			while(tries < 10) {
				if(write(STDOUT_FILENO, "\033[?1049l", 8) != -1) { break; }
				else { tries++; }
			}

			if (tries == 10) {
    			fprintf(stderr, "Failed to write after 10 attempts\n");
			}
		}
	
	#elif defined(_WIN32) || defined(_WIN64)

		DWORD written = 0;
    	WriteFile(hConsole, "\033[?1049l", 8, &written, NULL);

	#endif
	CORE.Tui.alternateBufferModeActive = false;

	CORE.Tui.rawModeActive = false;

	CORE.Tui.shouldClose = false;
}

void SetTargetFps(int fps) {
	if(fps < 1) { CORE.Time.target = 0.0; }
	else { CORE.Time.target = 1.0 / (double)fps; }
}




void BeginDrawing(void) {
	CORE.Backbuffor.lenght = 0;
}

void EndDrawing(void) {
	SetCursorPosition(CORE.Cursor.currentPosition.x, CORE.Cursor.currentPosition.y);

	#if defined(__APPLE__) || defined(__linux__)

		if(write(STDOUT_FILENO, CORE.Backbuffor.backBuffor, CORE.Backbuffor.lenght) == -1) { 
			int tries = 0;

			while(tries < 10) {
				if(write(STDOUT_FILENO, CORE.Backbuffor.backBuffor, CORE.Backbuffor.lenght) == -1) { break; }
				else { tries++; }
			}

			if (tries == 10) {
    			fprintf(stderr, "Failed to write after 10 attempts\n");
			}
		}

	#elif defined(_WIN32) || defined(_WIN64)

		DWORD written = 0;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		
		if (hConsole != INVALID_HANDLE_VALUE) {
    		WriteFile(hConsole, CORE.Backbuffor.backBuffor, CORE.Backbuffor.lenght, &written, NULL);
		}

	#endif

	CORE.Time.previous = CORE.Time.current;
    CORE.Time.current = GetTime();

	CORE.Time.delta = CORE.Time.current - CORE.Time.previous;

	if(CORE.Time.delta < CORE.Time.target) {
		double sleepTime = (CORE.Time.target - CORE.Time.delta);

		#if defined(__APPLE__) || defined(__linux__)
		
			usleep((useconds_t)(sleepTime * 1000000.0));

		#elif defined(_WIN32) || defined(_WIN64)
		
			Sleep((DWORD)(sleepTime * 1000.0));

		#endif

		CORE.Time.current = GetTime();
        CORE.Time.delta = CORE.Time.current - CORE.Time.previous;
	}

	CORE.Time.frameCounter++;
}




bool TuiShouldClose(void) { return CORE.Tui.shouldClose; }

bool IsAlternativeBufferOn(void) {
	return CORE.Tui.alternateBufferModeActive;
}

bool IsRawModeOn(void) {
	return CORE.Tui.rawModeActive;
}




int GetTuiWidth(void) {
	#if defined(__APPLE__) || defined(__linux__)

		struct winsize sizers;

		ioctl(STDOUT_FILENO, TIOCGWINSZ, &sizers);

		return sizers.ws_col;

	#elif defined(_WIN32) || defined(_WIN64)

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		
		if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
			return csbi.srWindow.Right - csbi.srWindow.Left + 1;
		}

	#endif
}

int GetTuiHeight(void) {
	#if defined(__APPLE__) || defined(__linux__)

		struct winsize sizers;

		ioctl(STDOUT_FILENO, TIOCGWINSZ, &sizers);

		return sizers.ws_row;

	#elif defined(_WIN32) || defined(_WIN64)

		CONSOLE_SCREEN_BUFFER_INFO csbi;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		
		return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	#endif
}

vector2 GetCursorPosition(void) {
	return CORE.Cursor.currentPosition;
}

vector2 GetLockedCursorPosition(void) {
	return CORE.Cursor.lockedPosition;
}

color GetBackgroundColor(void) {
	return CORE.Tui.backgroundColor;
}

color GetForegroundColor(void) {
	return CORE.Tui.foregroundColor;
}

fontStyle GetForegroundStyle(void) {
	return CORE.Tui.foregroundStyle;
}




void ShowCursor(void) {
	// Explanation in EXPLANATIONCOMMENT1 (just search it in the file)
	write(STDOUT_FILENO, "\033[?25h", 6);

	CORE.Cursor.hidden = false;
}

void HideCursor(void) {
	// Explanation in EXPLANATIONCOMMENT1 (just search it in the file)
	write(STDOUT_FILENO, "\033[?25l", 6);

	CORE.Cursor.hidden = true;
}

void LockCursor(void) {
	CORE.Cursor.locked = true;
}

void UnlockCursor(void) {
	CORE.Cursor.locked = false;
}




void SetBackgroundColor(color Color) {
	char bufi[32];
	char* pointy = bufi;

	*pointy++ = '\033';
    *pointy++ = '[';
    *pointy++ = '4';
    *pointy++ = '8';
    *pointy++ = ';';
    *pointy++ = '2';
    *pointy++ = ';';


	int colorybufi = Color.red;

	// Why this if? becouse 123 is 1 + 2 + 3 and if it was '0' + 123 it would be { but you know, char is 1 char not 3
    if (colorybufi >= 100) {
        *pointy++ = '0' + colorybufi / 100;
        *pointy++ = '0' + (colorybufi / 10) % 10;
        *pointy++ = '0' + colorybufi % 10;
    } else if (colorybufi >= 10) {
        *pointy++ = '0' + colorybufi / 10;
        *pointy++ = '0' + colorybufi % 10;
    } else {
        *pointy++ = '0' + colorybufi;
    }

    *pointy++ = ';';
    colorybufi = Color.green;
    if (colorybufi >= 100) {
        *pointy++ = '0' + colorybufi / 100;
        *pointy++ = '0' + (colorybufi / 10) % 10;
        *pointy++ = '0' + colorybufi % 10;
    } else if (colorybufi >= 10) {
        *pointy++ = '0' + colorybufi / 10;
        *pointy++ = '0' + colorybufi % 10;
    } else {
        *pointy++ = '0' + colorybufi;
    }

    *pointy++ = ';';
    colorybufi = Color.blue;
    if (colorybufi >= 100) {
        *pointy++ = '0' + colorybufi / 100;
        *pointy++ = '0' + (colorybufi / 10) % 10;
        *pointy++ = '0' + colorybufi % 10;
    } else if (colorybufi >= 10) {
        *pointy++ = '0' + colorybufi / 10;
        *pointy++ = '0' + colorybufi % 10;
    } else {
        *pointy++ = '0' + colorybufi;
    }

    *pointy++ = 'm';

	WriteToBackBuffor(bufi, pointy - bufi);

	CORE.Tui.backgroundColor = Color;
}

void SetForegroundColor(color Color) {
	char bufi[32];
	char* pointy = bufi;

	*pointy++ = '\033';
    *pointy++ = '[';
    *pointy++ = '3';
    *pointy++ = '8';
    *pointy++ = ';';
    *pointy++ = '2';
    *pointy++ = ';';


	int colorybufi = Color.red;

	// Why this if? becouse 123 is 1 + 2 + 3 and if it was '0' + 123 it would be { but you know, char is 1 char not 3
    if (colorybufi >= 100) {
        *pointy++ = '0' + colorybufi / 100;
        *pointy++ = '0' + (colorybufi / 10) % 10;
        *pointy++ = '0' + colorybufi % 10;
    } else if (colorybufi >= 10) {
        *pointy++ = '0' + colorybufi / 10;
        *pointy++ = '0' + colorybufi % 10;
    } else {
        *pointy++ = '0' + colorybufi;
    }

    *pointy++ = ';';
    colorybufi = Color.green;
    if (colorybufi >= 100) {
        *pointy++ = '0' + colorybufi / 100;
        *pointy++ = '0' + (colorybufi / 10) % 10;
        *pointy++ = '0' + colorybufi % 10;
    } else if (colorybufi >= 10) {
        *pointy++ = '0' + colorybufi / 10;
        *pointy++ = '0' + colorybufi % 10;
    } else {
        *pointy++ = '0' + colorybufi;
    }

    *pointy++ = ';';
    colorybufi = Color.blue;
    if (colorybufi >= 100) {
        *pointy++ = '0' + colorybufi / 100;
        *pointy++ = '0' + (colorybufi / 10) % 10;
        *pointy++ = '0' + colorybufi % 10;
    } else if (colorybufi >= 10) {
        *pointy++ = '0' + colorybufi / 10;
        *pointy++ = '0' + colorybufi % 10;
    } else {
        *pointy++ = '0' + colorybufi;
    }

    *pointy++ = 'm';

	WriteToBackBuffor(bufi, pointy - bufi);

	CORE.Tui.foregroundColor = Color;
}
// INFO: There are only 10 possible combinations and one need special threatment so It's fastest(In speed runtime sence) to make it 'by hand' (also, fuck microsoft)
void SetForegroundStyle(fontStyle Style) {
	switch(Style) {
		case STYLE_NORMAL:
			WriteToBackBuffor("\033[22;23;24;25;27;28;29m", 23);
			break;
			
		case STYLE_BOLD:
			WriteToBackBuffor("\033[1m", 4);
			break;

		case STYLE_DIMM:
			WriteToBackBuffor("\033[2m", 4);
			break;

		case STYLE_ITALIC:
			WriteToBackBuffor("\033[3m", 4);
			break;

		case STYLE_UNDERLINE:
			WriteToBackBuffor("\033[4m", 4);
			break;

		case STYLE_BACKGROUND:
			WriteToBackBuffor("\033[7m", 4);
			break;

		case STYLE_HIDDEN:
			WriteToBackBuffor("\033[8m", 4);
			break;

		case STYLE_STRIKETHROUGH:
			WriteToBackBuffor("\033[9m", 4);
			break;
	}

	CORE.Tui.foregroundStyle = Style;
}



void ClearBackground(color Color) {
	ClearScreen();

	SetBackgroundColor(Color);
}

void ClearScreen(void) {
	WriteToBackBuffor("\033[2J", 4);
}

void ClearLine(void) {
	WriteToBackBuffor("\r\033[2K", 5);
}

void ClearChar(void) {
	WriteToBackBuffor("\033[D\033[K", 6);
}


void SetCursorPosition(float x, float y) {
	if(x < 0) { fprintf(stderr, "Warning: x is negative, clamping to 0.\n"); x = 0.0f; }
	if(y < 0) { fprintf(stderr, "Warning: y is negative, clamping to 0.\n"); y = 0.0f; }

	unsigned int xinted = (unsigned int)x;
	unsigned int yinted = (unsigned int)y;

	CORE.Cursor.currentPosition.x = x;
	CORE.Cursor.currentPosition.y = y;

	CORE.Cursor.currentTerminalPosition.x = xinted;
	CORE.Cursor.currentTerminalPosition.y = yinted;

	xinted++;
	yinted++;

	char bufi[32]; // Check for 23
    char* pointy = bufi;

	*pointy++ = '\033';
    *pointy++ = '[';

	if(yinted == 0) {
        *pointy++ = '0';
    }
	else {
		char intbufi[10];
		unsigned int len = 0;

        while(yinted > 0 && len < (unsigned int)sizeof(intbufi)) {
            intbufi[len++] = '0' + (yinted % 10);
            yinted /= 10;
        }
        for(int i = len - 1; i >= 0; i--) {
            *pointy++ = intbufi[i];
        }
	}

	*pointy++ = ';';

	if(xinted == 0) {
        *pointy++ = '0';
    }
	else {
		char intbufi[10];
		unsigned int len = 0;

        while(xinted > 0 && len < (unsigned int)sizeof(intbufi)) {
            intbufi[len++] = '0' + (xinted % 10);
            xinted /= 10;
        }
        for(int i = len - 1; i >= 0; i--) {
            *pointy++ = intbufi[i];
        }
	}

	*pointy++ = 'H';

	WriteToBackBuffor(bufi, pointy - bufi);
}

void SetLockedCursorPosition(float x, float y) {
	CORE.Cursor.lockedPosition.x = x;
	CORE.Cursor.lockedPosition.y = y;
}

void MoveCursorDirectional(float up, float down, float left, float right) {
    if (up > 0) {
        if (CORE.Cursor.locked) {
            CORE.Cursor.lockedPosition.y -= up;
        } else {
            printf("\033[%dA", (int)up);
            CORE.Cursor.currentPosition.y -= up;
            CORE.Cursor.currentTerminalPosition.y -= (int)up;
        }
    }
    if (down > 0) {
        if (CORE.Cursor.locked) {
            CORE.Cursor.lockedPosition.y += down;
        } else {
            printf("\033[%dB", (int)down);
            CORE.Cursor.currentPosition.y += down;
            CORE.Cursor.currentTerminalPosition.y += (int)down;
        }
    }
    if (right > 0) {
        if (CORE.Cursor.locked) {
            CORE.Cursor.lockedPosition.x += right;
        } else {
            printf("\033[%dC", (int)right);
            CORE.Cursor.currentPosition.x += right;
            CORE.Cursor.currentTerminalPosition.x += (int)right;
        }
    }
    if (left > 0) {
        if (CORE.Cursor.locked) {
            CORE.Cursor.lockedPosition.x -= left;
        } else {
            printf("\033[%dD", (int)left);
            CORE.Cursor.currentPosition.x -= left;
            CORE.Cursor.currentTerminalPosition.x -= (int)left;
        }
    }
    fflush(stdout);
}

void MoveCursor(float x, float y) {
    if (CORE.Cursor.locked) {
        CORE.Cursor.lockedPosition.x += x;
        CORE.Cursor.lockedPosition.y += y;
    } else {
        if (y < 0) {
            printf("\033[%dA", (int)-y);
            CORE.Cursor.currentPosition.y += y;
            CORE.Cursor.currentTerminalPosition.y += (int)y;
        }
        if (y > 0) {
            printf("\033[%dB", (int)y);
            CORE.Cursor.currentPosition.y += y;
            CORE.Cursor.currentTerminalPosition.y += (int)y;
        }
        if (x > 0) {
            printf("\033[%dC", (int)x);
            CORE.Cursor.currentPosition.x += x;
            CORE.Cursor.currentTerminalPosition.x += (int)x;
        }
        if (x < 0) {
            printf("\033[%dD", (int)-x);
            CORE.Cursor.currentPosition.x += x;
            CORE.Cursor.currentTerminalPosition.x += (int)x;
        }
        fflush(stdout);
    }
}




int GetKey(void) {
	#if defined(__APPLE__) || defined(__linux__)
		unsigned char gotchar;
		int n = read(STDIN_FILENO, &gotchar, 1);
		if(n <= 0) return -1;
		
		
		if(gotchar == '\033') {
			unsigned char afterthing;
			n = read(STDIN_FILENO, &afterthing, 1);

			if(afterthing == '[' || afterthing == 'O') {
				// Here switch for all the esc sequences
				unsigned char finalthing;
				n = read(STDIN_FILENO, &finalthing, 1);

				switch(finalthing) {
					case 'A': return KEY_UP;
	     	       	case 'B': return KEY_DOWN;
	    	        case 'C': return KEY_RIGHT;
	    	        case 'D': return KEY_LEFT;
	    	        case 'H': return KEY_HOME;
	    	        case 'F': return KEY_END;
	
	            	case '1': {
	                	unsigned char afterfinal; 
						n = read(STDIN_FILENO, &afterfinal, 1);

						switch(afterfinal) {
	                		case '~': return KEY_HOME;
							case 'P': return KEY_F1;
							case 'Q': return KEY_F2;
							case 'R': return KEY_F3;
							case 'S': return KEY_F4;
							default: break;
						}
	                	break;
	            	}
	    	        case '2': {
	                	unsigned char afterfinal; 
						n = read(STDIN_FILENO, &afterfinal, 1);

 	    	           	switch(afterfinal) {
							case '~': return KEY_INSERT;
							case 'P': return KEY_F5;
	                		case 'Q': return KEY_F6;
							case 'R': return KEY_F7;
							case 'S': return KEY_F8;
							default: break;
						}
						break;
	      	     	}
	      	      	case '3': {
	                	unsigned char afterfinal; 
						n = read(STDIN_FILENO, &afterfinal, 1);

						switch(afterfinal) {
							case '~': return KEY_DELETE;
							case 'P': return KEY_F9;
							case 'Q': return KEY_F10;
	                		case 'R': return KEY_F11;
							case 'S': return KEY_F12;
							default: break;
						}
	      	          	break;
	     	       	}
	      	      	case '5': {
	                	unsigned char afterfinal; 
						n = read(STDIN_FILENO, &afterfinal, 1);

						switch(afterfinal) {
							case '~': return KEY_PAGE_UP;
							case 'P': return KEY_F13;
							case 'Q': return KEY_F14;
	                		case 'R': return KEY_F15;
							case 'S': return KEY_F16;
							default: break;
						}
	      	          	break;
	            	}
 		           	case '6': {
	                	unsigned char afterfinal; 
						n = read(STDIN_FILENO, &afterfinal, 1);

						switch(afterfinal) {
							case '~': return KEY_PAGE_DOWN;
							case 'P': return KEY_F17;
							case 'Q': return KEY_F18;
	 	               		case 'R': return KEY_F19;
							case 'S': return KEY_F20;
							default: break;
						}
	      	          	break;
 		           	}

	            	default:
						return KEY_NULL;
 		           	    break;
					}
			}
			return KEY_ESC;
		}
		return gotchar;


	#elif defined(_WIN32) || defined(_WIN64)


		if (!_kbhit()) return -1;
		int gotchar = _getch();

		if(gotchar == 0 || gotchar == 224) {
			// Here switch for all the esc sequences
			int afterthing = _getch();
			if(afterthing == '[' || afterthing == 'O') {
				// Here switch for all the esc sequences
				int finalthing = _getch();
				switch(finalthing) {
					case 'A': return KEY_UP;
     		       	case 'B': return KEY_DOWN;
    		        case 'C': return KEY_RIGHT;
    		        case 'D': return KEY_LEFT;
    		        case 'H': return KEY_HOME;
    		        case 'F': return KEY_END;

            		case '1': {
                		int afterfinal = _getch();
						switch(afterfinal) {
                			case '~': return KEY_HOME;
							case 'P': return KEY_F1;
							case 'Q': return KEY_F2;
							case 'R': return KEY_F3;
							case 'S': return KEY_F4;
							default: break;
						}
                		break;
            		}
    		        case '2': {
                		int afterfinal = _getch();
     		           	switch(afterfinal) {
							case '~': return KEY_INSERT;
							case 'P': return KEY_F5;
                			case 'Q': return KEY_F6;
							case 'R': return KEY_F7;
							case 'S': return KEY_F8;
							default: break;
						}
						break;
      		     	}
      		      	case '3': {
                		int afterfinal = _getch();
						switch(afterfinal) {
							case '~': return KEY_DELETE;
							case 'P': return KEY_F9;
							case 'Q': return KEY_F10;
                			case 'R': return KEY_F11;
							case 'S': return KEY_F12;
							default: break;
						}
      		          	break;
     		       	}
      		      	case '5': {
                		int afterfinal = _getch();
						switch(afterfinal) {
							case '~': return KEY_PAGE_UP;
							case 'P': return KEY_F13;
							case 'Q': return KEY_F14;
                			case 'R': return KEY_F15;
							case 'S': return KEY_F16;
							default: break;
						}
      		          	break;
            		}
            		case '6': {
                		int afterfinal = _getch();
						switch(afterfinal) {
							case '~': return KEY_PAGE_DOWN;
							case 'P': return KEY_F17;
							case 'Q': return KEY_F18;
                			case 'R': return KEY_F19;
							case 'S': return KEY_F20;
							default: break;
						}
      		          	break;
            		}

            		default:
						return KEY_NULL;
            		    break;
					}
			}
			return KEY_ESC;
		}
		return gotchar;


	#endif
}

double GetTime(void) {
	#if defined(__APPLE__) || defined(__linux__)
	
		struct timespec ts;
    	clock_gettime(CLOCK_MONOTONIC, &ts);
    	return ts.tv_sec + ts.tv_nsec / 1e9;

	#elif defined(_WIN32) || defined(_WIN64)
		// TODO: Also chatgbt things
	
		static LARGE_INTEGER frequency;
    	static BOOL initialized = FALSE;
    	if (!initialized) {
    	   	QueryPerformanceFrequency(&frequency);
    	    initialized = TRUE;
    	}

    	LARGE_INTEGER counter;
    	QueryPerformanceCounter(&counter);
    	return (double)counter.QuadPart / (double)frequency.QuadPart;

	#endif
}

void WriteToBackBuffor(const char* to_add, size_t lenght) {
	size_t required = CORE.Backbuffor.lenght + lenght;

	if(required > CORE.Backbuffor.capacity) {
		size_t new_capacity = CORE.Backbuffor.capacity * 2;
		if(new_capacity < required) new_capacity = required;

		char* newbackbuffor = realloc(CORE.Backbuffor.backBuffor, new_capacity);
		ReallocErrorProtocolVoid(newbackbuffor);

		CORE.Backbuffor.backBuffor = newbackbuffor;
		CORE.Backbuffor.capacity = new_capacity;
	}

	memcpy(CORE.Backbuffor.backBuffor + CORE.Backbuffor.lenght, to_add, lenght);
	
	CORE.Backbuffor.lenght += lenght;
}




void ToggleBufferMode(void) {
	if(!CORE.Tui.alternateBufferModeActive) {
		EnableBufferMode();
	}
	else {
		DisableBufferMode();
	}
}

void EnableBufferMode(void) {
	WriteToBackBuffor("\033[?1049h", 8);
	CORE.Tui.alternateBufferModeActive = true;
}

void DisableBufferMode(void) {
	WriteToBackBuffor("\033[?1049l", 8);
	CORE.Tui.alternateBufferModeActive = false;
}
