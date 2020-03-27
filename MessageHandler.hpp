#ifndef MESSAGEHANDLER_HPP__
#define MESSAGEHANDLER_HPP__

#include "Config.hpp"

#include <memory>

/**
 *
 */
namespace Messaging
{
	class Message;

	/**
	 * Base server interface for handling remote requests
	 * Classes derived from this interface can serve as server in the Messaging protocol
	 * by implementing this interface.
	 */
	class RequestHandler
	{
		public:
			virtual ~RequestHandler(){}
			/**
			 * After this function is called aMessage is returned as response to the requesting client,
			 * i.e. it should contain the result/response of/to the request.
			 *
			 * @param aMessage
			 */
			virtual void handleRequest( Message& aMessage) = 0;
	}; // class RequestHandler
	typedef std::shared_ptr< RequestHandler > RequestHandlerPtr;
	/**
	 * Base client interface for handling remote responses
	 * Classes derived from this interface can serve as client in the Messaging protocol
	 * by implementing this interface.
	 */
	class ResponseHandler
	{
		public:
			virtual ~ResponseHandler(){}
			/**
			 * The given argument contains the result/response of/to a previous request.
			 *
			 * @param aMessage
			 */
			virtual void handleResponse( const Message& aMessage) = 0;

	}; // class ResponseHandler
	typedef std::shared_ptr< ResponseHandler > ResponseHandlerPtr;
	/**
	 * Convenience interface class for a class that acts both as server and client in the Messaging protocol
	 * @see RequestHandler::handleRequest( Message& aMessage)
	 * @see ResponseHandler::handleResponse( Message& aMessage)
	 */
	class MessageHandler : public RequestHandler, public ResponseHandler
	{
	}; // class MessageHandler

} // namespace Messaging

#endif // MESSAGEHANDLER_HPP__
