#include "../../include/tuiutils.h"

#include <stdio.h>

void AltBuffModeSwitchTest() {
	printf("\nEnter to switch, Any other to end: ");
	fflush(stdout);

	TuiSwitchBuffMode();
	
	while(1) {
		char Input = ' ';

		scanf("%c", &Input);

		if(Input == '\n') { TuiSwitchBuffMode(); continue; }
		else { break; }
	}
}

void ClearTest() {
	printf("\n1 to clear screen, 2 to clear line, 3 to clear char, Any other to end\n");
	fflush(stdout);

	printf("SpAm:)");
	printf("\n");
	printf("SpAm:)");
	printf("\n");
	printf("SpAm:)");
	printf("\n");
	printf("SpAm:)");
	printf("\n");
	printf("SpAm:)");
	printf("\n");
	printf("SpAm:)");
	

	while(1) {
		char Input = ' ';

		scanf("%c", &Input);

		TuiCursorMoveDirectional(1, 0,0,0);

		if(Input == '1') {
			TuiClearScreen();
			continue;
		}
		else if(Input == '2') {
			TuiClearLine();
			continue;
		}
		else if(Input == '1') {
			TuiClearChar();
			continue;
		}
		else if(Input == '\n') {
			continue;
		}
		else {
			break;
		}
	}
}

void GetKeyTest() {
	printf("Got input(ctrl+c to esc): ");
	
	EnableRawMode();

	while(1) {
		int gototototor = GetKey();

		if(gototototor != KEY_NULL) {
			printf("| %c <- From lib ", gototototor);
		}
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
