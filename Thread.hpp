#ifndef THREAD_HPP_
#define THREAD_HPP_

/*
 * Copyright (c) 2014 Joost Kraaijeveld. See license.txt for details.
 * For information, bug reports and additions send an e-mail to RobotWorld@Askesis.nl.
 *
 * Author: jkr
 */


#include "Config.hpp"

#ifdef __MINGW32__

# ifdef __MINGW64_VERSION_MAJOR
	/* This is a mingw-w64 header-set.  */
	#include <thread>
	#include <mutex>
	#include <condition_variable>
	#include <chrono>
# else
	/* This is a mingw.org header-set.  */
	#include "MingwStd.hpp"
# endif //__MINGW64_VERSION_MAJOR

#endif // __MINGW32__

#ifdef __linux__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#endif // __linux__

#ifdef __APPLE__ // http://stackoverflow.com/questions/6802903/c-ifdef-mac-os-x-question

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#endif // __APPLE__


#endif // THREAD_HPP_ 
