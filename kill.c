#include <windows.h>
#include <tlhelp32.h>
#include <signal.h>
#include <stdio.h>
#include <tchar.h>

#ifdef UNICODE
#define _tputs _putws
#else
#define _tputs puts
#endif

static void TerminateProcessByName(const TCHAR *filename) {
	BOOL found = FALSE;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	PROCESSENTRY32 processEntry = { 0 };
	processEntry.dwSize = sizeof(processEntry);
	BOOL hRes = Process32First(hSnapShot, &processEntry);
	while (hRes) {
		// Compare the found process name with the given name (case insensitive)
		if (!_tcsicmp(processEntry.szExeFile, filename)) {
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)processEntry.th32ProcessID);
			if (hProcess != NULL) {
				// Checking for trying terminate itself
				if (processEntry.th32ProcessID == GetCurrentProcessId()) {
					_tputs(_T("ERROR: Can't terminate myself!"));
					found = TRUE;  // Mark it as found to avoid 'not found' error
				} else {
					_tprintf(_T("Terminating '%s'... "), processEntry.szExeFile);
					// Trying to terminate a process
					BOOL ok = TerminateProcess(hProcess, SIGTERM);
					// Wait for the process to complete
					WaitForSingleObject(hProcess, INFINITE);
					// Show the completion result
					_tprintf(_T("%s\n"), ok ? _T("success") : _T("failed"));
					found = TRUE;
				}
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &processEntry);
	}
	if (!found) _tprintf(_T("Process '%s' is not found!\n"), filename);
	CloseHandle(hSnapShot);
}

int _tmain(int argc, TCHAR * argv[]) {
	if (argc>1) {
		for (int i = 1; i < argc; i++) {
			TerminateProcessByName(argv[i]);
		}
	} else {
		TCHAR * basename = _tcschr(argv[0], 0);
		while (basename > argv[0] && !(basename[-1] == _T('/') || basename[-1] == _T('\\'))) basename--;
		_tputs(_T("Process killer, version 1.0\nCopyright (c) FoxTeam, 2020\n"));
		_tprintf(_T("Usage: %s <exename> [exename] ...\n"), basename);
	}
	return 0;
}
