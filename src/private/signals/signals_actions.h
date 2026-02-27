#ifndef ESCLIB_PRIVATE_SIGNALS_ACTIONS_H
#define ESCLIB_PRIVATE_SIGNALS_ACTIONS_H

#include "../coredata.h"

void SignalsSetup(void);
void SignalsStep(void);
void SignalsCleanup(void);

#if defined(unix) || defined(__unix) || defined(__unix__)

void Install(int sig, struct sigaction* old, void (*handler)(int));

#elif defined(_WIN32) || defined(_WIN64)

BOOL WINAPI ConsoleHandler(DWORD type);

#endif

#endif
