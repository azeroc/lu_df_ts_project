#include <Windows.h>
#include <iostream>
#include "handlers/TouchHandler.h"

void TestClass::TestMsg()
{
	// first determine the id of the current process
    DWORD const id = GetCurrentProcessId();
	std::cout << "Windows, current process ID: " << id << std::endl;
}