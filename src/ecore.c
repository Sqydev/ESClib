#include "../include/esclib.h"

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
		color backgroundColor;
		bool shouldClose;
		bool alternateBuffModeActive;
		bool rawModeActive;
	} Window;
	struct {
		#if defined(__APPLE__) || defined(__linux__)

			struct termios settings;

		#elif defined(_WIN32) || defined(_WIN64)

			DWORD settings;

		#endif
	} Terminal;
	struct {
		unsigned int targetFPS;
	} Time;
} CoreData;

CoreData CORE = { 0 };

// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
void InitTui(unsigned int fps) {
	CORE.Time.targetFPS = fps;

	CORE.Window.backgroundColor = (color){0, 0, 0};

	CORE.Window.shouldClose = false;

	EnableBufferMode();

	EnableRawMode();

	ClearScreen();

	FillScreen(CORE.Window.backgroundColor);
}

void CloseTui(void) {
	CORE.Window.shouldClose = false;
}


void SetBackgroundColor(color Color) {
	printf("\033[48;2;%d;%d;%dm", Color.red, Color.green, Color.blue);
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

void ClearBackground(color Color) {
	ClearScreen();
	FillScreen(Color);
	SetBackgroundColor(Color);
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

void EnableBufferMode(void) {
	printf("\033[?1049h");
	fflush(stdout);
	CORE.Window.alternateBuffModeActive = true;

	atexit(DisableBufferMode);
}

void DisableBufferMode(void) {
	printf("\033[?1049l");
	fflush(stdout);
	CORE.Window.alternateBuffModeActive = false;
}

void ToggleBufferMode(void) {
	if(!CORE.Window.alternateBuffModeActive) {
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

	CORE.Window.rawModeActive = true;

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

	CORE.Window.rawModeActive = false;
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
	if(!CORE.Window.rawModeActive) {
		EnableRawMode();
	}
	else {
		EnableRawMode();
	}
}
