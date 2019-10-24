#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdio.h>

#define MEM_COMMIT 0x00001000
#define PAGE_READWRITE 0x04
#define PROCESS_WM_READ 0x0010

// TODO: reference additional headers your program requires here
#ifdef DLLEXPORT

#else

#define DLLEXPORT __declspec(dllexport)

#endif

#ifndef WINAPI
#define WINAPI  __stdcall
#endif


extern "C" DLLEXPORT INT WINAPI ReplaceString(DWORD dwPid, PCSTR pcTarget, PCSTR pcDumpFilePath);
extern "C" DLLEXPORT VOID WINAPI PrintMessage(PCSTR pcPrint);
