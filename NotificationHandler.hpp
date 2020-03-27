#ifndef NOTIFICATIONHANDLER_HPP_
#define NOTIFICATIONHANDLER_HPP_

#include "Config.hpp"

#include "NotificationEvent.hpp"
#include "NotificationFunctionTypeTraits.hpp"
#include "Widgets.hpp"

namespace Base
{
	/*
	 *
	 */
	template< typename NotificationFunction >
	class NotificationHandler : public EventHandler
	{
		public:
			NotificationHandler( const NotificationFunction& aNotificationFunction) :
				function( aNotificationFunction)
		{
				Connect( EVT_NOTIFICATIONEVENT,
						 (wxObjectEventFunction)(wxEventFunction)static_cast< NotificationEventFunction >( &NotificationHandler::OnNotificationEvent));
		}
			virtual ~NotificationHandler()
			{
			}
			/**
			 *
			 */
			void OnNotificationEvent( NotifyEvent& aNotifyEvent)
			{
				NotificationFunctionTypeTraits< NotificationFunction > caller;
				caller.call( function, aNotifyEvent);
			}
		private:
			NotificationFunction function;
	};
} //namespace Base
#endif // NOTIFICATIONHANDLER_HPP_
