#ifndef ESCLIB_PRIVATE_COMMON_UTILS_H
#define ESCLIB_PRIVATE_COMMON_UTILS_H

#include "../../include/esclib.h"

#include <stddef.h>

typedef enum {
	UNI_WRITE_TARGET_STDOUT = 1,
	UNI_WRITE_TARGET_STDERR = 2
} UniWriteTarget;

int UniWrite(UniWriteTarget target, const void* buf, size_t n);
int UniWriteLen(UniWriteTarget target, const void* buf);
size_t WriteToBackbuff(const SBCell* content, size_t cellCount);

#endif
