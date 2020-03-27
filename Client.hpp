#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "Config.hpp"

#include <string>

#include "Session.hpp"

namespace Messaging
{
	class Client
	{
		public:
			/**
			 *
			 */
			Client( const std::string& aHost,
					const std::string& aPort,
					ResponseHandlerPtr aResponseHandler) :
							io_service( CommunicationService::getCommunicationService().getIOService()),
							host( aHost),
							port( aPort),
							responseHandler( aResponseHandler)
			{
			}
			/**
			 *
			 */
			~Client()
			{
			}
			/**
			 *
			 *
			 * @startuml
			 * -> client: dispatchMessage(message)
			 *
			 * activate client
			 * client -->> session
			 * client -> session : getSocket
			 * activate session
			 * client <-- session : socket
			 * deactivate session
			 *
			 * client -\ socket: async_connect(Client::handleConnect,session)
			 * <-- client
			 * deactivate client
			 * client <- socket : handleConnect(session)
			 * activate socket
			 *
			 * activate client
			 * client -> session :start
			 * deactivate client
			 * deactivate socket
			 * activate session
			 *
			 * == Writing the request ==
			 *
			 * session -> session: writeMessage(message)
			 * activate session
			 * session -\ socket: async_write(header,Session::handleHeaderWriten)
			 * deactivate session
			 * activate socket
			 * client <-- session
			 * deactivate session
			 *
			 * session <- socket : handleHeaderWriten(message,error)
			 * deactivate socket
			 * activate session
			 *
			 * session -\ socket: async_write(body,Session::handleBodyWriten)
			 * deactivate session
			 * activate socket
			 *
			 * session <- socket : handleBodyWriten(message,error)
			 * deactivate socket
			 * activate session
			 *
			 * session -> session : handleMessageWriten(message,error)
			 * activate session
			 * session -> session : handleMessageWritten(message)
			 * activate session
			 *
			 * == Reading the response ==
			 * session -> session: readMessage
			 * activate session
			 *
			 * session -\ socket: async_read(header,Session::handleHeaderRead)
			 * deactivate session
			 * activate socket
			 * deactivate session
			 * deactivate session
			 * deactivate session
			 *
			 * session <- socket : handleHeaderRead(message,error)
			 * deactivate socket
			 * activate session
			 *
			 * session -\ socket: async_read(body,Session::handleBodyRead)
			 * deactivate session
			 * activate socket
			 *
			 * session <- socket : handleBodyRead(message,error)
			 * deactivate socket
			 * activate session
			 *
			 * session -> session : handleMessageRead(message,error)
			 * activate session
			 * session -> session : handleMessageRead(message)
			 * activate session
			 *
			 * == Handling the response ==
			 * session -> responseHandler: handleResponse(message)
			 * activate responseHandler
			 * session <-- responseHandler
			 * deactivate responseHandler
			 * deactivate session
			 * deactivate session
			 *
			 * destroy session
			 *
			 * @enduml
			 */
			void dispatchMessage( Message& aMessage)
			{
				// Create the session that will handle the next outgoing connection
				ClientSession* session = new ClientSession( aMessage, io_service, responseHandler);

				// Build up the remote address to which we will connect
				boost::asio::ip::tcp::resolver resolver( io_service);
				boost::asio::ip::tcp::resolver::query query( boost::asio::ip::tcp::v4(), host, port);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query);
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

				// Let the session handle any outgoing messages
				session->getSocket().async_connect( endpoint,
													boost::bind( &Client::handleConnect, this, session, boost::asio::placeholders::error));
			}
			/**
			 *
			 */
			void handleConnect( ClientSession* aSession,
								const boost::system::error_code& error)
			{
				if (!error)
				{
					aSession->start();
				} else
				{
					std::string sessionDestination =  "host = " + host + ", port =  "+ port;
					delete aSession;
					throw std::runtime_error( __PRETTY_FUNCTION__ + std::string( ": ") + error.message() + ", " + sessionDestination);
				}
			}

		private:
			boost::asio::io_service& io_service;
			std::string host;
			std::string port;
			ResponseHandlerPtr responseHandler;
	};
} // namespace Messaging

#endif // CLIENT_HPP_
