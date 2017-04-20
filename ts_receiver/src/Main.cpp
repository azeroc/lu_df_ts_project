#include <iostream>
#include "handlers/TouchHandler.h"
#include "Constants.h"

int main(int argc, char* argv[])
{
	TestClass test;
	test.TestMsg();
    std::cout << "Constant: " << TS_RECEIVER_CONST << std::endl;
	return 0;
}