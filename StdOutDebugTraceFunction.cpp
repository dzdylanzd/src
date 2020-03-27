#include "StdOutDebugTraceFunction.hpp"
#include <iostream>

namespace Base
{
	void StdOutDebugTraceFunction::trace( const std::string& aText)
	{
		std::cout << aText << std::endl;
	}
} // namespace Base
