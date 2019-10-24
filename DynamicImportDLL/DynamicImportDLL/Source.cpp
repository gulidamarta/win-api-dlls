#include <windows.h> 
#include <stdio.h> 

typedef VOID(WINAPI *TPrintMessage)(PCSTR);

int main(void)
{
	printf("Explicit dynamic linking.\n");

	HINSTANCE hinstLib;
	TPrintMessage ProcAdd;
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

	// Get a handle to the DLL module.

	hinstLib = LoadLibrary(TEXT("Main.dll"));

	// If the handle is valid, try to get the function address.

	if (hinstLib != NULL)
	{
		ProcAdd = (TPrintMessage)GetProcAddress(hinstLib, "_PrintMessage@4");

		// If the function address is valid, call the function.

		if (NULL != ProcAdd)
		{
			fRunTimeLinkSuccess = TRUE;
			(ProcAdd)("\nMessage sent to the DLL function\n");
			Sleep(3000);
		}
		// Free the DLL module.
		Sleep(30000);
		fFreeResult = FreeLibrary(hinstLib);
	}
	else
	{
		printf("Error occupied");
		Sleep(3000);
	}

	// If unable to call the DLL function, use an alternative.
	if (!fRunTimeLinkSuccess)
		printf("Message printed from executable\n");

	return 0;

}