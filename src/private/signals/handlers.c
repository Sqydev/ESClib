#include "../coredata.h"

void sigint_handler(int sig) {
	(void)sig;
	DATA.SignalData.SIG_INT.triggered = 1;
}

void sigwinch_handler(int sig) {
	(void)sig;
	DATA.SignalData.SIG_WINCH.triggered = 1;
}
