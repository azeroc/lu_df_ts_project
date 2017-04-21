#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "Constants.h"

int main(int argc, char* argv[])
{
	using namespace boost::lambda;
	typedef std::istream_iterator<int> in;

    std::cout << "ts_sender test, const value: " << TS_SENDER_CONST << std::endl;

	std::for_each(
		in(std::cin), in(), std::cout << (_1 * 3) << " ");

    return 0;
}