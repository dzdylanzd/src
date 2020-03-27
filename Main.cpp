#include "Config.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include "MainApplication.hpp"

int main( 	int argc,
			char* argv[])
{
	try
	{
		// Call the wxWidgets main variant
		// This will actually call Application
		int result = runGUI( argc, argv);
		return result;
	}
	catch (std::exception& e)
	{
		std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
	}
	return 0;
}
