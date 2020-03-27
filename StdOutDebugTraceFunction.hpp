#ifndef STDOUTDEBUGTRACEFUNCTION_HPP_
#define STDOUTDEBUGTRACEFUNCTION_HPP_

#include "Config.hpp"
#include "DebugTraceFunction.hpp"

namespace Base
{
	/**
	 *
	 */
	class StdOutDebugTraceFunction : public DebugTraceFunction
	{
		public:
			/**
			 *
			 * @param aText The text that will be send to the final trace destination.
			 */
			virtual void trace( const std::string& aText);

	};
	// class StdOutDebugTraceFunction
} // namespace Base
#endif // STDOUTDEBUGTRACEFUNCTION_HPP_
