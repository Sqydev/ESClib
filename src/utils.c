// NOTE: This file has system usiversal fns only!

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
	#include <signal.h>
#endif

// NOTE: This will add task just before exit(0);
void AddTaskToSignal() {
	
}
