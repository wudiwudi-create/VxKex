// Build for x64 with: cl /W4 /TC test.c
// Enable VxKex for test.exe before running it on Windows 7.
#include <windows.h>
#include <stdio.h>

typedef VOID (WINAPI *QUERY_INTERRUPT_TIME_PRECISE)(PULONGLONG);

int main(void)
{
	HMODULE loaded, existing;
	QUERY_INTERRUPT_TIME_PRECISE biased, unbiased;
	ULONGLONG biasedBefore, biasedAfter, unbiasedBefore, unbiasedAfter;
	char path[MAX_PATH];

	loaded = LoadLibraryExW(L"kernelbase.dll", NULL, 0);
	if (!loaded) {
		printf("LoadLibraryExW failed: %lu\n", GetLastError());
		return 1;
	}
	existing = GetModuleHandleW(L"kernelbase.dll");
	if (!existing || existing != loaded) {
		printf("GetModuleHandleW failed or returned a different module\n");
		return 1;
	}
	if (!GetModuleFileNameA(loaded, path, MAX_PATH)) {
		printf("GetModuleFileNameA failed: %lu\n", GetLastError());
		return 1;
	}
	printf("Resolved module: %s\n", path);

	biased = (QUERY_INTERRUPT_TIME_PRECISE) GetProcAddress(loaded, "QueryInterruptTimePrecise");
	unbiased = (QUERY_INTERRUPT_TIME_PRECISE) GetProcAddress(existing, "QueryUnbiasedInterruptTimePrecise");
	if (!biased || !unbiased) {
		printf("Missing export: biased=%d, unbiased=%d\n", biased != NULL, unbiased != NULL);
		return 1;
	}
	biased(&biasedBefore);
	unbiased(&unbiasedBefore);
	Sleep(200);
	biased(&biasedAfter);
	unbiased(&unbiasedAfter);
	if (!biasedBefore || !unbiasedBefore || biasedAfter <= biasedBefore || unbiasedAfter <= unbiasedBefore) {
		printf("Clock values did not advance\n");
		return 1;
	}
	printf("PASS: both exports resolve and advance; biased delta=%I64u, unbiased delta=%I64u (100 ns units)\n",
		biasedAfter - biasedBefore, unbiasedAfter - unbiasedBefore);
	FreeLibrary(loaded);
	return 0;
}
