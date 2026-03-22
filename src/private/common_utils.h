#ifndef ESCLIB_PRIVATE_COMMON_UTILS_H
#define ESCLIB_PRIVATE_COMMON_UTILS_H

#include "../../include/esclib.h"

#include <stddef.h>

typedef enum {
	UNI_WRITE_TARGET_STDOUT = 1,
	UNI_WRITE_TARGET_STDERR = 2
} UniWriteTarget;

size_t UniWrite(UniWriteTarget target, const void* buf, size_t n);
size_t UniWriteLen(UniWriteTarget target, const void* buf);
void WriteToBackbuff(const SBCell cell, size_t x, size_t y);

void EnableRawMode(void);
void DisableRawMode(void);

#endif
