#include "../../../include/esclib.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#endif


#include "./signals_actions.h"
#include "./handlers.h"
#include "../coredata.h"
#include "../common_utils.h"

#include <signal.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


static void SignalFatal(const char *msg) {

#if defined(unix) || defined(__unix) || defined(__unix__)

    write(STDERR_FILENO, msg, strlen(msg));

#elif defined(_WIN32) || defined(_WIN64)

	UniWrite(UNI_WRITE_TARGET_STDERR, msg, strlen(msg) - 1);

#endif
    exit(EXIT_FAILURE);
}
