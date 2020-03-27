#ifndef WIDGETDEBUGTRACEFUNCTION_HPP_
#define WIDGETDEBUGTRACEFUNCTION_HPP_

#include "Config.hpp"
#include "Widgets.hpp"
#include "DebugTraceFunction.hpp"

namespace Application
{
	class LogTextCtrl;

	/**
	 *
	 */
	class WidgetDebugTraceFunction : public Base::DebugTraceFunction
	{
		public:
			/**
			 * @param aLogTextCtrl The TextCtrl derived class that will receive the trace string as message
			 * @param useFile If true the trace will also be logged in the file "guitrace.log".
			 */
			WidgetDebugTraceFunction( Application::LogTextCtrl* aLogTextCtrl);
			/**
			 *
			 * @param aText The text that will be send to the final trace destination.
			 */
			virtual void trace( const std::string& aText);
		private:
			/**
			 *
			 */
			Application::LogTextCtrl* outputControl;
	};
} // namespace Application
#endif // WIDGETDEBUGTRACEFUNCTION_HPP_
