#include "esclib.h"

#include <stdlib.h>

void InitTui(int width, int height, int targetFps, TuiType tuiType) {
	atexit(CloseTui);
}

void CloseTui(void) {

}
