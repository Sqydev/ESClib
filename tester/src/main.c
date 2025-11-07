#include "../../include/esclib.h"

#include <unistd.h>
#include <stdio.h>

#define Superfps (1000000 / 60) 

void AltBuffModeSwitchTest() {
}

void ClearTest() {
}

void GetKeyTest() {
}

void FunnyCrsTest() {
}

void BackbufforTest() {
	printf("Adding word of ILOVETHECPROGRAMINGLANGUAGE every half a seccond(and then printing it)(It will print on 0, 0 becouse there's no Init)\n");

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

	InitTui(tfps, false);

	while(!TuiShouldClose()) {
		char key = GetKey();

		BeginDrawing();

		ClearBackground((color){0, 0, 0});

		WriteToBackBuffor(&key, 1);

		if(key == KEY_ESC) { break; }

		EndDrawing();
	}

	CloseTui();
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

	//if(Input == 1) { AltBuffModeSwitchTest(); }
	//else if(Input == 2) { ClearTest(); }
	//else if(Input == 3) { GetKeyTest(); }
	//else if(Input == 4) { FunnyCrsTest(); }
	//else if(Input == 5) { ClearandFillTest(); }
	//else 
	if(Input == 6) { BackbufforTest(); }
	else if(Input == 7) { TuiLoopTest(); }
	
	return 0;
}
