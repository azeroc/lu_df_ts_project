#include <windows.h>
#include <iostream>
#include "touch_control.h"

void touch_control::test_msg()
{
    // first determine the id of the current process
    DWORD const id = GetCurrentProcessId();
    std::cout << "Windows, current process ID: " << id << std::endl;
}