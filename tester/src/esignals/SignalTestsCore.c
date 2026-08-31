#include "../tester.h"

void CTask1(void) { printf("Task 1 exec\n"); fflush(stdout); }
void CTask2(void) { printf("Task 2 exec\n"); fflush(stdout); }
void CTask3(void) { printf("Task 3 exec\n"); fflush(stdout); }

void WaitAndReact(int seconds) {
	int steps = seconds * 60;
    for (int i = 0; i < steps; i++) {
        SignalsStep(); 
        
		ESleep(0, 16, 666666);
    }
}

void RunSignalTester(int signo, const char* signame, int (*disableFn)(int), int (*enableFn)(int), const char* typeDesc) {
	InitTui(60, TUI_DYNAMIC);

	printf("CustomTaskCount = %d\n", GetCustomSignalTasksCount(signo));
	printf("CustomTaskSize = %zu\n", GetCustomSignalTasksSize(signo));

	AddSignalTask(signo, CTask1, 0);
	AddSignalTask(signo, CTask2, 1);
	AddSignalTask(signo, CTask3, 2);

	printf("After adding CustomTaskCount = %d\n", GetCustomSignalTasksCount(signo));
	printf("After adding CustomTaskSize = %zu\n", GetCustomSignalTasksSize(signo));

	RemoveSignalTask(signo, 0);
	AddSignalTask(signo, CTask1, -1);
	RemoveSignalTask(signo, 1);
	AddSignalTask(signo, CTask2, -1);
	RemoveSignalTask(signo, 2);
	AddSignalTask(signo, CTask3, 20);

	printf("After Shuffle CustomTaskCount = %d\n", GetCustomSignalTasksCount(signo));
	printf("After Shuffle CustomTaskSize = %zu\n", GetCustomSignalTasksSize(signo));

	CompressSignalTasks(signo, -1, -1);

	printf("After Compresson CustomTaskCount = %d\n", GetCustomSignalTasksCount(signo));
	printf("After Compresson CustomTaskSize = %zu\n", GetCustomSignalTasksSize(signo));

	disableFn(signo);
	printf("TESTER: Disabled %s (%s type) waiting 5 sec\n", signame, typeDesc);
	fflush(stdout);

	WaitAndReact(5);

	enableFn(signo);
	printf("TESTER: Enabled %s (%s type) waiting 5 sec\n", signame, typeDesc);
	fflush(stdout);

	WaitAndReact(5);

	printf("TESTER: Ending test\n");
	fflush(stdout);

	CloseTui();
}
