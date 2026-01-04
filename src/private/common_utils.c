#include "./common_utils.h"

#if defined(_WIN32) || defined(_WIN64)
#elif defined(__linux__) || defined(__APPLE__)
	#include <unistd.h>
#endif

// TODO: DO return codes here
int UniWrite(int fd, const void *buf, size_t n) {
	#if defined(_WIN32) || defined(_WIN64)
	#elif defined(__linux__) || defined(__APPLE__)
		write(fd, buf, n);
	#endif

	return 0;
}
