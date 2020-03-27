#ifndef NOTIFICATIONFUNCTIONTYPETRAITS_HPP_
#define NOTIFICATIONFUNCTIONTYPETRAITS_HPP_

#include "Config.hpp"

#include <functional>

#include "Widgets.hpp"

namespace Base
{
	struct NotificationFunctionTypeTraitsTracing
	{
	};

	template< typename NotificationFunction >
	struct NotificationFunctionTypeTraits
	{
			void call( 	NotificationFunction& aNotificationFunction,
						NotifyEvent&)
			{
				aNotificationFunction();
			}
	};

	template< >
	struct NotificationFunctionTypeTraits< std::function< void( NotifyEvent&) > >
	{
			void call( 	std::function< void( NotifyEvent&) >& aNotificationFunction,
						NotifyEvent& event)
			{
				aNotificationFunction( event);
			}
	};
	/**
	 * Dummy function to allow for usage of not-yet implemented
	 * std::function<void (CommandEvent&)> functions
	 * @param
	 */
	void Ooops( NotifyEvent&);
} //namespace Base
#endif /* NOTIFICATIONFUNCTIONTYPETRAITS_HPP_ */
