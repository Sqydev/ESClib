#include "../../../include/esclib.h"

#include "../../../include/esclib.h"

#include "./signals_actions.h"
#include "./handlers.h"
#include "../coredata.h"

#include <signal.h>
#include <stddef.h>


// TODO: Add handling for more signals(like SIGWITH to know when terrminal is resized itp.)
void SignalsSetup(void) {
	#if defined(_WIN32) || defined(_WIN64)
	#elif defined(__linux__) || defined(__APPLE__)
		DATA.SignalData.SIG_INT.triggered = 0;
		DATA.SignalData.SIG_INT.enabled = 1;
		DATA.SignalData.SIG_INT.enabledESClibTasks = 1;
		DATA.SignalData.SIG_INT.enabledCustomTasks = 1;
	
		sigaction(SIGINT, NULL, &DATA.SignalData.SIG_INT.old);

		struct sigaction sia = {0};
		sia.sa_handler = sigint_handler;
	
		sigemptyset(&sia.sa_mask);
		sia.sa_flags = 0;
		sigaction(SIGINT, &sia, NULL);
	#endif
}

void SignalsStep(void) {
	#if defined(_WIN32) || defined(_WIN64)
	#elif defined(__linux__) || defined(__APPLE__)
		if(DATA.SignalData.SIG_INT.triggered == 1 && DATA.SignalData.SIG_INT.enabled == true) {
			DATA.SignalData.SIG_INT.triggered = 0;
			
			if(DATA.SignalData.SIG_INT.enabledESClibTasks) {
				CloseTui();
			}

			// NOTE: Retrive the old functions of SIGINT
			sigaction(SIGINT, &DATA.SignalData.SIG_INT.old, NULL);
	
			if(DATA.SignalData.SIG_INT.enabledBuildInTasks) {
				if(DATA.SignalData.SIG_INT.old.sa_handler == SIG_DFL) {
					// NOTE: Call SIGINT
					raise(SIGINT);
				}
				else if(DATA.SignalData.SIG_INT.old.sa_handler != SIG_IGN) {
					// NOTE: Ignore
					DATA.SignalData.SIG_INT.old.sa_handler(SIGINT);
				}
			}
		}
	#endif
}
