#include "../../include/esclib.h"

#include "./utils.h"
#include "./coredata.h"

#if defined(__WIN32) || defined(__WIN64)
#elif defined(__linux__) || defined(__APPLE__)
	#include <signal.h>
#endif

#include <stddef.h>

// NOTE: That's global DATA pointer(will point to DATA after SignalsSetup)
CoreData *gDATA = NULL;

#if defined(_WIN32) || defined(_WIN64)
#elif defined(__linux__) || defined(__APPLE__)
	void sigint_handler(int signo) {
		(void)signo;
		if(!gDATA) { return; }
		gDATA->SignalData.sigintOn = 1;
	}
#endif

// TODO: Add handling for more signals(like SIGWITH to know when terrminal is resized itp.)
void SignalsSetup(CoreData *DATA) {
	#if defined(_WIN32) || defined(_WIN64)
	#elif defined(__linux__) || defined(__APPLE__)
		DATA->SignalData.sigintOn = 0;
		gDATA = DATA;
	
		sigaction(SIGINT, NULL, &DATA->SignalData.sigint);

		struct sigaction sia = {0};
		sia.sa_handler = sigint_handler;
	
		sigemptyset(&sia.sa_mask);
		sia.sa_flags = 0;
		sigaction(SIGINT, &sia, NULL);
	#endif
}

void SignalsStep() {
	#if defined(_WIN32) || defined(_WIN64)
	#elif defined(__linux__) || defined(__APPLE__)
		if(gDATA->SignalData.sigintOn == 1) {
			gDATA->SignalData.sigintOn = 0;
			
			CloseTui();
	
			// NOTE: Retrive the old functions of SIGINT
			sigaction(SIGINT, &gDATA->SignalData.sigint, NULL);
	
			if(gDATA->SignalData.sigint.sa_handler == SIG_DFL) {
				// NOTE: Call SIGINT
				raise(SIGINT);
			}
			else if(gDATA->SignalData.sigint.sa_handler != SIG_IGN) {
				// NOTE: Ignore
				gDATA->SignalData.sigint.sa_handler(SIGINT);
			}
		}
	#endif
}
