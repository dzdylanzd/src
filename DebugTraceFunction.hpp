/*
 * DebugTraceFunction.hpp
 *
 *  Created on: 22 Mar 2013
 *      Author: jkr
 */

#ifndef DEBUGTRACEFUNCTION_HPP_
#define DEBUGTRACEFUNCTION_HPP_

#include "Config.hpp"

#include <string>

namespace Base
{
	/**
	 *
	 */
	class DebugTraceFunction
	{
		public:
			/**
			 * Virtual dtor as this is meant as a base class
			 */
			virtual ~DebugTraceFunction();
			/**
			 *
			 * @param aText The text that will be send to the final trace destination.
			 */
			virtual void trace( const std::string& aText) = 0;
	};
} // namespace Base
#endif /* DEBUGTRACEFUNCTION_HPP_ */
