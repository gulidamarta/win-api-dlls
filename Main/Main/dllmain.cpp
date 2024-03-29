// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "dllmain.h"

void str_replace(char *, const char *, const char *);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		// DLL проецируется на адресное пространство процесса
		// DLL должна выполнить инициализацию, необходимую её функциям (лишь раз выполняется)
    case DLL_THREAD_ATTACH:
		// создается поток
    case DLL_THREAD_DETACH:
		// поток корректно завершается
    case DLL_PROCESS_DETACH:
		// DLL отключается от адресного пространства процесса
        break;
    }
    return TRUE; // Используется только для DLL_PROCESS_ATTACH (корректно ли прошла инициализация DLL)
}

INT DLLEXPORT WINAPI ReplaceString(DWORD dwPid, PCSTR pcTarget, PCSTR pcReplacement)
{
	// getting minimum & maximum address
	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);

	// saving the values as long ints so I won't have to do a lot of casts later
	DWORD_PTR proc_min_address = (DWORD_PTR)sys_info.lpMinimumApplicationAddress;
	DWORD_PTR proc_max_address = (DWORD_PTR)sys_info.lpMaximumApplicationAddress;

	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_WM_READ, false, dwPid);
	if (hProc == NULL)
		return 1;

	MEMORY_BASIC_INFORMATION mem_info;
	PCHAR pchBuff = new CHAR[sys_info.dwPageSize];
	//std::string temp;

	for (DWORD pAddr = proc_min_address; pAddr < proc_max_address; pAddr += sys_info.dwPageSize) {
		VirtualQueryEx(hProc, (LPCVOID)proc_min_address, &mem_info, sizeof(mem_info));
		if (mem_info.Protect == PAGE_READWRITE && mem_info.State == MEM_COMMIT) {
			ZeroMemory(pchBuff, sys_info.dwPageSize);
			ReadProcessMemory(hProc, mem_info.BaseAddress, pchBuff, mem_info.RegionSize, NULL);
			//ReadProcessMemory(hProc, mem_info.BaseAddress, &temp, mem_info.RegionSize, NULL);
			//findAndReplaceAll(temp, pcTarget, pcReplacement);
			//WriteProcessMemory(hProc, (LPVOID)pAddr, &temp, sys_info.dwPageSize, NULL);

			str_replace(pchBuff, pcTarget, pcReplacement);
			WriteProcessMemory(hProc, mem_info.BaseAddress, pchBuff, mem_info.RegionSize, NULL);
		}
	}

	CloseHandle(hProc);
	delete[] pchBuff;
	return 0;
}


void str_replace(char *target, const char *needle, const char *replacement)
{
	char buffer[1024] = { 0 };
	char *insert_point = &buffer[0];
	const char *tmp = target;
	size_t needle_len = strlen(needle);
	size_t repl_len = strlen(replacement);

	while (1) {
		const char *p = strstr(tmp, needle);

		// walked past last occurrence of needle; copy remaining part
		if (p == NULL) {
			strcpy_s(insert_point, sizeof insert_point, tmp);
			break;
		}

		// copy part before needle
		memcpy(insert_point, tmp, p - tmp);
		insert_point += p - tmp;

		// copy replacement string
		memcpy(insert_point, replacement, repl_len);
		insert_point += repl_len;

		// adjust pointers, move on
		tmp = p + needle_len;
	}

	// write altered string back to target
	strcpy_s(target, sizeof target, buffer);
}


VOID DLLEXPORT WINAPI PrintMessage(PCSTR pcPrint) {
	printf("test string: %s", pcPrint);
}






