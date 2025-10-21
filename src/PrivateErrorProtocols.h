#ifndef PRIVATE_ERROR_PROTOCOLS_H
#define PRIVATE_ERROR_PROTOCOLS_H

#include <stdio.h>

#define MallocErrorProtocol(ptr) do { \
		if (ptr == NULL) { \
			fprintf(stderr, "ERROR: Malloc result just happened to be NULL\n"); \
			return NULL; \
		} \
	} while(0)

#define MallocErrorProtocolVoid(ptr) do { \
		if (ptr == NULL) { \
			fprintf(stderr, "ERROR: Malloc result just happened to be NULL\n"); \
			return; \
		} \
	} while(0)

#define ReallocErrorProtocol(ptr) do { \
		if (ptr == NULL) { \
			fprintf(stderr, "ERROR: Realloc result just happened to be NULL\n"); \
			return NULL; \
		} \
	} while(0)

#define ReallocErrorProtocolVoid(ptr) do { \
		if (ptr == NULL) { \
			fprintf(stderr, "ERROR: Realloc result just happened to be NULL\n"); \
			return; \
		} \
	} while(0)

#endif
