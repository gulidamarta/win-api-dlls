#include <stdio.h>
#include <string.h>
#include <windows.h>

volatile char greetingMessage[10];

int main()
{
	strcpy_s((char *)greetingMessage, sizeof(greetingMessage), "world");

	while (true) {
		printf("Hello, %s\n", greetingMessage);
		Sleep(200);
	}
}