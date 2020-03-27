#include "Notifier.hpp"
#include <boost/foreach.hpp>
#include <sstream>
#include <typeinfo>
#include "Observer.hpp"
#include "Logger.hpp"

namespace Base
{
	/**
	 *
	 */
	Notifier::Notifier( bool enable /*= true*/) :
								notify( enable)
	{
	}
	/**
	 *
	 */
	Notifier::~Notifier()
	{
	}
	/**
	 *
	 */
	void Notifier::enableNotification( bool enable /* = true */)
	{
		notify = enable;
	}
	/**
	 *
	 */
	void Notifier::disableNotification()
	{
		notify = false;
	}
	/**
	 *
	 */
	bool Notifier::isEnabledForNotification() const
	{
		return notify;
	}
	/**
	 *	The implementation of operator== uses pointer comparison!
	 */
	void Notifier::addObserver( Observer& aObserver)
	{
		for (Observer* observer : observers)
		{
			if (*observer == aObserver)
			{
				return;
			}
		}
		observers.push_back( &aObserver);
	}
	/**
	 *	The implementation of operator== uses pointer comparison!
	 */
	void Notifier::removeObserver( Observer& aObserver)
	{
		for (std::vector< Observer* >::iterator i = observers.begin(); i != observers.end(); ++i)
		{
			if (*(*i) == aObserver)
			{
				observers.erase( i);
				break;
			}
		}
	}
	/**
	 *
	 */
	void Notifier::removeAllObservers()
	{
		observers.erase( observers.begin(), observers.end());
	}
	/**
	 *
	 */
	void Notifier::notifyObservers()
	{
		if (notify)
		{
			for (Observer* observer : observers)
			{
				observer->handleNotification();
			}
		}
	}
	/**
	 *
	 */
	std::string Notifier::asString() const
	{
		std::ostringstream os;
		os << typeid(*this).name();
		return os.str();
	}
	/**
	 *
	 */
	std::string Notifier::asDebugString() const
	{
		return asString();
	}
} //namespace Base
