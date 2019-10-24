#include <windows.h>
#include <stdio.h>
#include "dllmain.h"

extern "C" VOID (WINAPI PrintMessage)(PCSTR);

int main(VOID) {
	Sleep(2000);
	printf("Implicit dynamic linking.\n");

	PrintMessage("Message from static linking file.\n");
	
	Sleep(5000);
	return 0;
}