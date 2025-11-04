#include "../include/esclib.h"
#include "PrivateErrorProtocols.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Setup
#if defined(__APPLE__) || defined(__linux__)

#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>

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
		bool shouldClose;
		bool alternateBufferModeActive;
		bool rawModeActive;
	} Tui;
	struct {
		#if defined(__APPLE__) || defined(__linux__)

			struct termios settings;

		#elif defined(_WIN32) || defined(_WIN64)

			DWORD settings;

		#endif
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
		double update;
		double draw;
		double frame;
		double target;
		unsigned long long int base;
		unsigned int frameCounter;
	} Time;
} CoreData;

CoreData CORE = { 0 };




// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
void InitTui(int fps) {

	#if defined(_WIN32) || defined(_WIN64)
	
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    	DWORD dwMode = 0;
    	GetConsoleMode(hOut, &dwMode);
    	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	
	#endif

	CORE.Tui.backgroundColor = (color){0, 0, 0};

	CORE.Tui.shouldClose = false;

	CORE.Cursor.hidden = false;

	SetTargetFps(fps);

	EnableBufferMode();

	EnableRawMode();

	ClearScreen();

	SetCursorPosition(0, 0);

	SetLockedCursorPosition(0, 0);

	atexit(CloseTui);
}

void CloseTui(void) {

	#if defined(_WIN32) || defined(_WIN64)
	
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    	DWORD dwMode = 0;
    	GetConsoleMode(hOut, &dwMode);
    	SetConsoleMode(hOut, dwMode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	
	#endif

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
	#if defined(__APPLE__) || defined(__linux__)

		write(STDOUT_FILENO, CORE.Backbuffor.backBuffor, CORE.Backbuffor.lenght);

	#elif defined(_WIN32) || defined(_WIN64)

		DWORD written = 0;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if(hConsole != INVALID_HANDLE_VALUE) {
    		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), CORE.Backbuffor.backBuffor, CORE.Backbuffor.lenght, &written, NULL);
		}

	#endif
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




void ShowCursor(void) {
	printf("\033[?25h");
	fflush(stdout);

	CORE.Cursor.hidden = false;
}

void HideCursor(void) {
	printf("\033[?25l");
	fflush(stdout);

	CORE.Cursor.hidden = true;
}

void LockCursor(void) {
	CORE.Cursor.locked = true;
}

void UnlockCursor(void) {
	CORE.Cursor.locked = false;
}




void SetBackgroundColor(color Color) {
	printf("\033[48;2;%d;%d;%dm", Color.red, Color.green, Color.blue);
	fflush(stdout);
	CORE.Tui.backgroundColor = Color;
}

void ClearBackground(color Color) {
	ClearScreen();

	#if defined(__APPLE__) || defined(__linux__)


		struct winsize sizers;

		// Get that mesures
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &sizers);

		// Chainge color of background in terminal
		SetBackgroundColor(Color);

		for (int i = 0; i < sizers.ws_row; i++) {
			for (int j = 0; j < sizers.ws_col; j++) {
            	putchar(' ');
			}
        	putchar('\n');
		}
		fflush(stdout);


	#elif defined(_WIN32) || defined(_WIN64)
	// TODO: Chatgbt port, litteraly FillScreen for linux ctrl+c ctrl+v to chatgbt. Why? cuz fuck microsoft. I'll do it myself someday.


    	CONSOLE_SCREEN_BUFFER_INFO csbi;
    	GetConsoleScreenBufferInfo(hOut, &csbi);
    	int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    	int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;

		SetBackgroundColor(Color);

    	for (int i = 0; i < rows; i++) {
        	for (int j = 0; j < cols; j++) {
            	putchar(' ');
			}
        	putchar('\n');
    	}
		fflush(stdout);


	#endif
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



void SetCursorPosition(float x, float y) {
	printf("\033[%d;%dH", (int)y, (int)x);
	fflush(stdout);

	CORE.Cursor.currentPosition.x = x;
	CORE.Cursor.currentPosition.y = y;

	CORE.Cursor.currentTerminalPosition.x = (int)x;
	CORE.Cursor.currentTerminalPosition.y = (int)y;
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


void EnableBufferMode(void) {
	printf("\033[?1049h");
	fflush(stdout);
	CORE.Tui.alternateBufferModeActive = true;

	atexit(DisableBufferMode);
}

void DisableBufferMode(void) {
	printf("\033[?1049l");
	fflush(stdout);
	CORE.Tui.alternateBufferModeActive = false;
}

void ToggleBufferMode(void) {
	if(!CORE.Tui.alternateBufferModeActive) {
		EnableBufferMode();
	}
	else {
		DisableBufferMode();
	}
}

void EnableRawMode(void) {
	#if defined(__APPLE__) || defined(__linux__)


		// This is our workspace
		struct termios raw;

		// Get current term settings
		tcgetattr(STDIN_FILENO, &CORE.Terminal.settings);
		raw = CORE.Terminal.settings;

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


	#elif defined(_WIN32) || defined(_WIN64)


    	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    	GetConsoleMode(hIn, &CORE.Terminal.settings);

    	DWORD raw = CORE.Terminal.settings;
    	raw &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    	raw |= ENABLE_PROCESSED_INPUT;

    	SetConsoleMode(hIn, raw);


	#endif

	CORE.Tui.rawModeActive = true;

	// Make sure we restore the terminal when program exits
	atexit(DisableRawMode);
}

void DisableRawMode(void) {
	#if defined(__APPLE__) || defined(__linux__)


		// Restore original terminal settings
		tcsetattr(STDIN_FILENO, TCSANOW, &CORE.Terminal.settings);

		// Get current flags
		int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
		// Remove O_NONBLOCK bit (bitwise AND with flag negation)
		fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);


	#elif defined(_WIN32) || defined(_WIN64)


    	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    	SetConsoleMode(hIn, CORE.Terminal.settings);


	#endif

	CORE.Tui.rawModeActive = false;
}

int GetKey(void) {
	#if defined(__APPLE__) || defined(__linux__)


		int gotchar = getchar();
		// NOTHING
		if (gotchar == EOF) return -1;
	
		if(gotchar == '\033') {
			int afterthing = getchar();
			if(afterthing == '[' || afterthing == 'O') {
				// Here switch for all the esc sequences
				int finalthing = getchar();
				switch(finalthing) {
					case 'A': return KEY_UP;
	     	       	case 'B': return KEY_DOWN;
	    	        case 'C': return KEY_RIGHT;
	    	        case 'D': return KEY_LEFT;
	    	        case 'H': return KEY_HOME;
	    	        case 'F': return KEY_END;
	
	            	case '1': {
	                	int afterfinal = getchar();
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
	                	int afterfinal = getchar();
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
	                	int afterfinal = getchar();
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
	                	int afterfinal = getchar();
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
 		               	int afterfinal = getchar();
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

void ToggleRawMode(void) {
	if(!CORE.Tui.rawModeActive) {
		EnableRawMode();
	}
	else {
		EnableRawMode();
	}
}

void WriteToBackBuffor(const char* to_add, size_t lenght) {
	size_t required = CORE.Backbuffor.lenght + lenght + 1;

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
