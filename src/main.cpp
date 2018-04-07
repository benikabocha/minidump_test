#include <Windows.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>
#include <tchar.h>

#include <iostream>

int GenerateDump(EXCEPTION_POINTERS* expPtr) {
    TCHAR desktopDir[MAX_PATH];
    SHGetSpecialFolderPath(NULL, desktopDir, CSIDL_DESKTOPDIRECTORY, FALSE);

    SYSTEMTIME localTime;
    GetLocalTime(&localTime);

    TCHAR* appName = _T("dump_test");

    TCHAR dumpPath[MAX_PATH];
    StringCchPrintf(
        dumpPath, MAX_PATH, _T("%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp"),
        desktopDir, appName, localTime.wYear, localTime.wMonth, localTime.wDay,
        localTime.wHour, localTime.wMinute, localTime.wSecond,
        GetCurrentProcessId(), GetCurrentThreadId());

    HANDLE dumpFile =
        CreateFile(dumpPath, GENERIC_READ | GENERIC_WRITE,
                   FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

    MINIDUMP_EXCEPTION_INFORMATION expInfo;
    expInfo.ThreadId = GetCurrentThreadId();
    expInfo.ExceptionPointers = expPtr;
    expInfo.ClientPointers = TRUE;

    BOOL miniDumpSuccessful =
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile,
                          MiniDumpWithDataSegs, &expInfo, NULL, NULL);

    return EXCEPTION_EXECUTE_HANDLER;
}

int main() {
    __try {
        int* badPtr = NULL;
        *badPtr = 0;
    } __except (GenerateDump(GetExceptionInformation())) {
    }

    return 0;
}
