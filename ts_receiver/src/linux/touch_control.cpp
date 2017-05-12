#include <linux/limits.h>
#include <iostream>
#include "touch_control.h"

void touch_control::test_msg()
{
	std::cout << "Linux limits, PATH_MAX: " << PATH_MAX << std::endl;
}
