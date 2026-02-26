#include "../../../include/esclib.h"

#include <unistd.h>

void InitCloseTuiTester() {
	InitTui(0);

	write(STDOUT_FILENO, "TESTER: Doing 2 sec break:)\n", 28);

	sleep(2);

	write(STDOUT_FILENO, "TESTER: Closing Tui\n", 20);

	CloseTui();

	write(STDOUT_FILENO, "TESTER: Ending test\n", 20);
}
