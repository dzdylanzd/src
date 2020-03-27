#include "Logger.hpp"
#include "MainApplication.hpp"
#include "MainFrameWindow.hpp"
#include "DebugTraceFunction.hpp"

namespace Application
{
	/* static */bool Logger::disable = false;
	/**
	 *
	 */
	/*static*/void Logger::log( const std::string& aMessage)
	{
		Application::MainFrameWindow* frame = dynamic_cast< Application::MainFrameWindow* >( Application::TheApp().GetTopWindow());
		if (frame && !disable)
		{
			frame->getTraceFunction().trace( aMessage);
		}
	}
	/**
	 *
	 */
	/* static */void Logger::setDisable( bool aDisable /*= true*/)
	{
		disable = aDisable;
	}
} //namespace Application
