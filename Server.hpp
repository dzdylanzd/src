#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "Config.hpp"
#include "Session.hpp"
#include "CommunicationService.hpp"

namespace Messaging
{
	/**
	 *
	 */
	class Server
	{
		public:
			/**
			 * The server will listen on localhost/ip-address on the port
			 */
			Server( short port,
					RequestHandlerPtr aRequestHandler) :
						io_service( CommunicationService::getCommunicationService().getIOService()),
						acceptor( io_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port)),
						requestHandler( aRequestHandler)
			{
				// start handling incoming connections
				handleAccept( nullptr, boost::system::error_code());
			}
			/**
			 *
			 */
			~Server()
			{
			}
			/**
			 *	Handle any incoming connections
			 *
			 * @startuml
			 * -> server: handleAccept
			 * activate server
			 * server -->> session
			 * server -> session : getSocket
			 * activate session
			 * server <-- session : socket
			 * deactivate session
			 *
			 * server -\ acceptor : asyn_accept(socket,Server::handleAccept)
			 * server -> session : start
			 * deactivate server
			 * activate session
			 *
			 * == Reading the request ==
			 *
			 * session -\ session: readMessage
			 * activate session
			 *
			 * session -\ socket: async_read(header,Session::handleHeaderRead)
			 * deactivate session
			 * activate socket
			 * server <-- session
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
			 * == Handling the request ==
			 *
			 * session -> requestHandler : handleRequest(message)
			 * activate requestHandler
			 * session <-- requestHandler
			 * deactivate requestHandler
			 *
			 * == Writing the response ==
			 *
			 * session -> session: writeMessage(message)
			 * activate session
			 * session -\ socket: async_write(header,Session::handleHeaderWriten)
			 * deactivate session
			 * activate socket
			 * deactivate session
			 * deactivate session
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
			 * deactivate session
			 * deactivate session
			 *
			 * destroy session
			 *
			 * @enduml
			 */
			void handleAccept( 	ServerSession* aSession,
								const boost::system::error_code& error)
			{
				if (!error)
				{
					// Create the session that will handle the next incoming connection
					ServerSession* session = new ServerSession( io_service, requestHandler);
					// Let the acceptor wait for any new incoming connections
					// and let it call server::handle_accept on the happy occasion
					acceptor.async_accept( session->getSocket(),
										   boost::bind( &Server::handleAccept, this, session, boost::asio::placeholders::error));
					// If there is a session, start it up....
					if (aSession)
					{
						aSession->start();
					}
				} else
				{
					delete aSession;
					throw std::runtime_error( __PRETTY_FUNCTION__ + std::string( ": ") + error.message());
				}
			}
		private:
			// Provides core I/O functionality
			// @see http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/reference/io_service
			boost::asio::io_service& io_service;
			// Provides the ability to accept new connections
			// @see http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/reference/basic_socket_acceptor
			boost::asio::ip::tcp::acceptor acceptor;
			/**
			 *
			 */
			RequestHandlerPtr requestHandler;
	};
// class Server
}// namespace Messaging

#endif // SERVER_HPP_
