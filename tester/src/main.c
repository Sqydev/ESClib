#include "../../include/esclib.h"

#include <unistd.h>
#include <stdio.h>

#define Superfps (1000000 / 60) 

void AltBuffModeSwitchTest() {
	int gotkey = -1;

	EnableRawMode();

	printf("\nEnter to switch, Esc to end: ");
	fflush(stdout);

	//ToggleBufferMode();
	//ClearScreen();
	
	while(1) {
		gotkey = GetKey();

		if(gotkey == KEY_ENTER) { ToggleBufferMode(); }
		if(gotkey == KEY_ESC) { break; }

		usleep(Superfps);
	}
}

void ClearTest() {
	int gotkey = -1;

	printf("\n1 to clear screen, 2 to clear line, 3 to clear char, 4 to spam, Esc to escape\n");
	fflush(stdout);

	EnableRawMode();

	while(1) {
        gotkey = GetKey();

        if (gotkey == KEY_1) {
            ClearScreen();
			gotkey = -1;
			continue;
        }
        else if (gotkey == KEY_2) {
            ClearLine();
			gotkey = -1;
			continue;
        }
        else if (gotkey == KEY_3) {
            ClearChar();
			gotkey = -1;
			continue;
        }
        else if (gotkey == KEY_4) {
            printf("SpAm:)");
			gotkey = -1;
			continue;
        } 
		if(gotkey == KEY_ESC) {
			break;
		}

		gotkey = -1;

		usleep(Superfps);
	}
}

void GetKeyTest() {
	int gototototor = -1;

	EnableRawMode();

	printf("Input from lib(esc to esc):\n");

	while(1) {
		gototototor = GetKey();

		if(gototototor != KEY_NULL) {
			ClearLine();
			printf("| %d <- From lib |", gototototor);
		}
		if(gototototor == KEY_ESC) {
			break;
		}

		gototototor = -1;

		usleep(Superfps);
	}
}

void FunnyCrsTest() {
	int gotkey = -1;

	EnableRawMode();
	
	printf("WSAD or Arrows to move!(Esc to esc)");

	while(1) {
		gotkey = GetKey();

		if(gotkey == KEY_W || gotkey == KEY_UP) {
			MoveCursorDirectional(1, 0, 0, 0);
		}
		if(gotkey == KEY_S || gotkey == KEY_DOWN) {
			MoveCursorDirectional(0, 1, 0, 0);
		}
		if(gotkey == KEY_A || gotkey == KEY_LEFT) {
			MoveCursorDirectional(0, 0, 1, 0);
		}
		if(gotkey == KEY_D || gotkey == KEY_RIGHT) {
			MoveCursorDirectional(0, 0, 0, 1);
		}
		if(gotkey == KEY_ESC) {
			break;
		}

		gotkey = -1;

		usleep(Superfps);
	}
}

void ClearandFillTest() {
	int gotkey = -1;

	EnableRawMode();

	ClearBackground((color){255, 100, 4});

	printf("Esc to esc");

	while(1) {
		gotkey = GetKey();
		
		if(gotkey == KEY_ESC) {
			break;
		}

		gotkey = -1;

		usleep(Superfps);
	}
}

void BackbufforTest() {
	printf("Adding word of ILOVETHECPROGRAMINGLANGUAGE every half a seccond(and then printing it)\n");

	WriteToBackBuffor("I", 1);

	usleep(500000);

	WriteToBackBuffor("LOVE", 4);

	usleep(500000);

	WriteToBackBuffor("THE", 3);

	usleep(500000);

	WriteToBackBuffor("C", 1);

	usleep(500000);

	WriteToBackBuffor("PROGRAMING", 10);

	usleep(500000);

	WriteToBackBuffor("LANGUAGE", 8);

	EndDrawing();

	printf("\nAnd now without double buffor(btw. I've done it with write cuz most terminals make semi double bufforing:) but the tty doesn't :0)\n");

	write(STDOUT_FILENO, "I", 1);
	usleep(500000);
	write(STDOUT_FILENO, "LOVE", 4);
	usleep(500000);
	write(STDOUT_FILENO, "THE", 3);
	usleep(500000);
	write(STDOUT_FILENO, "C", 1);
	usleep(500000);
	write(STDOUT_FILENO, "PROGRAMING", 10);
	usleep(500000);
	write(STDOUT_FILENO, "LANGUAGE", 8);
	usleep(500000);
}

void TuiLoopTest() {

	// BTW. THIS IS ABSOLUTE GARBAGE MADE IN NEED OF TESTING WITHOUT MAKING PROPER TOOLS YET
	// BTW. THIS IS ABSOLUTE GARBAGE MADE IN NEED OF TESTING WITHOUT MAKING PROPER TOOLS YET
	// BTW. THIS IS ABSOLUTE GARBAGE MADE IN NEED OF TESTING WITHOUT MAKING PROPER TOOLS YET
	// BTW. THIS IS ABSOLUTE GARBAGE MADE IN NEED OF TESTING WITHOUT MAKING PROPER TOOLS YET

	int tfps = 0;

	printf("Give targetfps(And btw. This test will do GetKey): ");

	scanf("%d", &tfps);

	InitTui(tfps);

	while(!TuiShouldClose()) {
		BeginDrawing();

		ClearBackground((color){0, 0, 0});

		WriteToBackBuffor("Pressed W SPEED!", 8+1+6+2);
		
		EndDrawing();
	}
	printf("ENDED");
}

int main() {
	int Input = 0;

	printf("1. Mode Switch Test\n");
	printf("2. Clear Test\n");
	printf("3. GetKey Test\n");
	printf("4. Funnyahh cursor movement trick test\n");
	printf("5. ClearandFill test\n");
	printf("6. Backbuffor WriteTo test\n");
	printf("7. TuiLoop test\n");

	printf("Input: ");
	scanf("%d", &Input);

	if(Input == 1) { AltBuffModeSwitchTest(); }
	else if(Input == 2) { ClearTest(); }
	else if(Input == 3) { GetKeyTest(); }
	else if(Input == 4) { FunnyCrsTest(); }
	else if(Input == 5) { ClearandFillTest(); }
	else if(Input == 6) { BackbufforTest(); }
	else if(Input == 7) { TuiLoopTest(); }
	
	return 0;
}
