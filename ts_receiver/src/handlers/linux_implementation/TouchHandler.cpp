#include <linux/limits.h>
#include <iostream>
#include "handlers/TouchHandler.h"

void TestClass::TestMsg()
{
	std::cout << "Linux limits, PATH_MAX: " << PATH_MAX << std::endl;
}
