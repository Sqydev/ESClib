#include "../../include/escutils.h"

#include <unistd.h>
#include <stdio.h>

#define Superfps (1000000 / 60) 

void AltBuffModeSwitchTest() {
	printf("\nEnter to switch, Any other to end: ");
	fflush(stdout);

	TuiSwitchBuffMode();
	
	while(1) {
		char Input = ' ';

		scanf("%c", &Input);

		if(Input == '\n') { TuiSwitchBuffMode(); continue; }
		else { break; }

		usleep(Superfps);
	}
}

void ClearTest() {
	int gotkey = -1;

	printf("\n1 to clear screen, 2 to clear line, 3 to clear char, 4 to spam\n");
	fflush(stdout);

	EnableRawMode();

	while(1) {
        gotkey = GetKey();

        if (gotkey == KEY_1) {
            TuiClearScreen();
        }
        else if (gotkey == KEY_2) {
            TuiClearLine();
        }
        else if (gotkey == KEY_3) {
            TuiClearChar();
        }
        else if (gotkey == KEY_4) {
            printf("SpAm:)");
        }

		gotkey = -1;

		usleep(Superfps);
	}
}

void GetKeyTest() {
	int gototototor = -1;

	EnableRawMode();

	printf("Input from lib(ctrl+c to esc):\n");

	while(1) {
		gototototor = GetKey();

		if(gototototor != KEY_NULL) {
			TuiClearLine();
			printf("| %c <- From lib |", gototototor);
		}
		if(gototototor == KEY_TAB) {
			TuiClearLine();
			printf("TAB!");
		}

		gototototor = -1;

		usleep(Superfps);
	}
}

int main() {
	int Input = 0;

	printf("1. Mode Switch Test\n");
	printf("2. Clear Test\n");
	printf("3. GetKey Test\n");

	printf("Input: ");
	scanf("%d", &Input);

	if(Input == 1) { AltBuffModeSwitchTest(); }
	else if(Input == 2) { ClearTest(); }
	else if(Input == 3) { GetKeyTest(); }
	
	return 0;
}
