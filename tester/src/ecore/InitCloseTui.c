#include "../../../include/esclib.h"

#include <unistd.h>
#include <time.h>

void InitCloseTuiTester() {
	InitTui(0);

	write(STDOUT_FILENO, "TESTER: Doing 2 sec break:)\n", 28);

	ESleep(5, 0, 0);

	write(STDOUT_FILENO, "TESTER: Closing Tui\n", 20);

	CloseTui();

	write(STDOUT_FILENO, "TESTER: Ending test\n", 20);
}
