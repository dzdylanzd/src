#ifndef MINGWSTD_HPP_
#define MINGWSTD_HPP_

/*
 * Copyright (c) 2014 Joost Kraaijeveld. See license.txt for details.
 * For information, bug reports and additions send an e-mail to RobotWorld@Askesis.nl.
 *
 * Author: jkr
 */

#include "Config.hpp"

#include <stdexcept>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/chrono.hpp>

/**
 * Miscellaneous fixes for missing MinGW 4.8.1 features.
 */
namespace std
{
	/**
	 * MinGW 4.8.1 does not have a working thread support library (yet)
	 */

	/**
	 *
	 */
	typedef boost::thread thread;
	namespace this_thread = boost::this_thread;

	/**
	 *
	 */
	typedef boost::mutex mutex;
	typedef boost::timed_mutex timed_mutex;
	typedef boost::recursive_mutex recursive_mutex;
	typedef boost::recursive_timed_mutex recursive_timed_mutex;

	/**
	 *
	 */
	template< typename Lockable  >
	using lock_guard =	boost::lock_guard< Lockable > ;

	template< typename Lockable  >
	using unique_lock =	boost::unique_lock< Lockable > ;

	template< typename Lockable  >
	using shared_lock =	boost::shared_lock< Lockable > ;

	/**
	 *
	 */
	typedef boost::condition_variable condition_variable;
	typedef boost::condition_variable_any condition_variable_any;

	/**
	 * Mingw 4.8.1 does not have the std::to_string family of functions (yet)
	 *
	 * This template requires an implemented std::ostream& operator<<(std::ostream& os,const T& t)
	 */
	template<typename T>
	inline std::string to_string(const T& x)
	{
		std::ostringstream o;
		if (!(o << x))
		{
			throw std::invalid_argument(std::string("to_string(") + typeid(x).name() + ")");
		}
		return o.str();
	}

	/**
	 * Mingw 4.8.1 does not have the std::stoi function (yet)
	 *
	 */
	inline int stoi( const std::string& aString)
	{
		return std::atoi(aString.c_str());
	}

	namespace chrono
	{
		template <typename Rep, typename Period>
		using duration =  boost::chrono::duration< Rep,  Period /*= ratio<1>*/ >;
		typedef duration<boost::int_least64_t, boost::milli> milliseconds;
	}
	;
} // namespace std

#endif // MINGWSTD_HPP_ 
