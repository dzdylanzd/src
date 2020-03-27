#ifndef SESSION_HPP_
#define SESSION_HPP_

#include "Config.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <functional>

#include "Message.hpp"
#include "MessageHandler.hpp"
#include "CommunicationService.hpp"

namespace Messaging
{
	/**
	 * A session is an encapsulation of a request/response transaction sequence.
	 */
	class Session
	{
		public:
			/**
			 *
			 * @param io_service
			 */
			Session( boost::asio::io_service& io_service) :
					socket( io_service)
			{
			}
			/**
			 *
			 */
			virtual ~Session()
			{
			}
			/**
			 * Typically a ServerSession has a read/write sequence,
			 * a ClientSession a write/read sequence
			 */
			virtual void start() = 0;
			/**
			 * Handle the fact that a message is read. This function is called by the framework
			 * after the message (header + body) is read. Normally this is the only function
			 * that a ServerSession or ClientSession has to implement.
			 */
			virtual void handleMessageRead( Message& aMessage) = 0;
			/**
			 * Handle the fact that a message is written. This function is called by the framework
			 * after the message (header + body) is written. Normally this is the only function
			 * that a ServerSession or ClientSession has to implement.
			 */
			virtual void handleMessageWritten( Message& aMessage) = 0;
			/**
			 * This function must be public or Client and Server should be friend of Session
			 *
			 * @return the socket of this Session
			 */
			boost::asio::ip::tcp::socket& getSocket()
			{
				return socket;
			}
		protected:
			/**
			 * readMessage will read the message in 2 a-sync reads, 1 for the header and 1 for the body.
			 * After each read a callback will be called that should handle the stuff just read.
			 * After reading the full message handleMessageRead will be called
			 * whose responsibility it is to handle the message as a whole.
			 *
			 * @see Session::handleHeaderRead
			 * @see Session::handleBodyRead
			 * @see Session::handleMessageRead
			 */
			void readMessage()
			{
				Message aMessage;
				headerBuffer.resize( aMessage.getHeader().getHeaderLength());
				boost::asio::async_read( getSocket(),
										 boost::asio::buffer( headerBuffer),
										 boost::bind( &Session::handleHeaderRead, this, aMessage, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
			/**
			 * This function is called after the header bytes are read.
			 */
			void handleHeaderRead( 	Message& aMessage,
									const boost::system::error_code& error,
									size_t UNUSEDPARAM(bytes_transferred))
			{
				if (!error)
				{
					aMessage.setHeader( std::string( headerBuffer.begin(), headerBuffer.end()));
					bodyBuffer.resize( aMessage.getHeader().getMessageLength());
					boost::asio::async_read( getSocket(),
											 boost::asio::buffer( bodyBuffer),
											 boost::bind( &Session::handleBodyRead, this, aMessage, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
				} else
				{
					// See https://isocpp.org/wiki/faq/freestore-mgmt#delete-this
					delete this;
					throw std::runtime_error( __PRETTY_FUNCTION__ + std::string( ": ") + error.message());
				}
			}
			/**
			 * This function as called after the body bytes are read.
			 *
			 * Any error handling (throwing an exception ;-)) is done in this function and
			 * then the function with the same name but without the error is called.
			 */
			void handleBodyRead( 	Message& aMessage,
									const boost::system::error_code& error,
									size_t bytes_transferred)
			{
				if (!error)
				{
					aMessage.setBody( std::string( bodyBuffer.begin(), bodyBuffer.end()));
					handleMessageRead( aMessage, error, bytes_transferred);
				} else
				{
					// See https://isocpp.org/wiki/faq/freestore-mgmt#delete-this
					delete this;
					// Throwing a exception goes wrong if a "stop" message is send in the (limited)
					// context of this example. If any "strange" things happen, enable the next line.
					// A "end of file" exception will happen on "normal" termination of the message exchange...

					//throw std::runtime_error(__PRETTY_FUNCTION__ + std::string(": ") + error.message());
				}
			}
			/**
			 * This function is called after both the header and body bytes are read.
			 */
			void handleMessageRead( Message& aMessage,
									const boost::system::error_code& error,
									size_t UNUSEDPARAM(bytes_transferred))
			{
				if (!error)
				{
					handleMessageRead( aMessage);
				} else
				{
					delete this;
					throw std::runtime_error( __PRETTY_FUNCTION__ + std::string( ": ") + error.message());
				}
			}
			/**
			 * writeMessage will write the message in 2 a-sync writes, 1 for the header and 1 for the body.
			 * After each write a callback will be called that should handle the stuff just read.
			 * After writing the full message handleMessageWritten will be called.
			 *
			 * @see Session::handleHeaderWritten
			 * @see Session::handleBodyWritten
			 * @see Session::handleMessageWritten
			 */
			void writeMessage( Message& aMessage)
			{
				boost::asio::async_write( getSocket(),
										  boost::asio::buffer( aMessage.getHeader().toString(), aMessage.getHeader().getHeaderLength()),
										  boost::bind( &Session::handleHeaderWritten, this, aMessage, boost::asio::placeholders::error));
			}
			/**
			 * This function is called after the header bytes are written.
			 */
			void handleHeaderWritten( 	Message& aMessage,
										const boost::system::error_code& error)
			{
				if (!error)
				{
					boost::asio::async_write( getSocket(),
											  boost::asio::buffer( aMessage.getBody(), aMessage.length()),
											  boost::bind( &Session::handleBodyWritten, this, aMessage, boost::asio::placeholders::error));
				} else
				{
					delete this;
					throw std::runtime_error( __PRETTY_FUNCTION__ + std::string( ": ") + error.message());
				}
			}
			/**
			 * This function is called after the body bytes are written.
			 */
			void handleBodyWritten( Message& aMessage,
									const boost::system::error_code& error)
			{
				if (!error)
				{
					handleMessageWritten( aMessage, error);
				} else
				{
					delete this;
					throw std::runtime_error( __PRETTY_FUNCTION__ + std::string( ": ") + error.message());
				}
			}
			/**
			 * This function is called after both the header and body bytes are written.
			 *
			 * Any error handling (throwing an exception ;-)) is done in this function and
			 * then the function with the same name but without the error is called.
			 */
			void handleMessageWritten( 	Message& aMessage,
										const boost::system::error_code& error)
			{
				if (!error)
				{
					handleMessageWritten( aMessage);
				} else
				{
					delete this;
					throw std::runtime_error( __PRETTY_FUNCTION__ + std::string( ": ") + error.message());
				}
			}

			boost::asio::ip::tcp::socket socket;
			std::vector< char > headerBuffer;
			std::vector< char > bodyBuffer;
	};
	// class Session
	/**
	 *
	 */
	class ServerSession : virtual public Session
	{
		public:
			/**
			 *
			 * @param io_service
			 * @param aRequestHandler
			 */
			ServerSession( 	boost::asio::io_service& io_service,
							RequestHandlerPtr aRequestHandler) :
							Session( io_service),
							requestHandler( aRequestHandler)
			{
			}
			/**
			 *
			 */
			~ServerSession()
			{
			}
			/**
			 * @see Session::start()
			 */
			virtual void start()
			{
				readMessage();
			}
			/**
			 * @see Session::handleMessageRead( Message& aMessage)
			 */
			virtual void handleMessageRead( Message& aMessage)
			{
				requestHandler->handleRequest( aMessage);
				writeMessage( aMessage);

				// This is part of the original application. If one wants a stop message
				// just leave this here. Otherwise think something up yourself.
				if (aMessage.getBody() == "stop")
				{
					CommunicationService::getCommunicationService().getIOService().stop();
				}
			}
			/**
			 * @see Session::handleMessageWritten( Message& aMessage)
			 */
			virtual void handleMessageWritten( Message& UNUSEDPARAM(aMessage))
			{
				delete this;
			}

		private:
			RequestHandlerPtr  requestHandler;

	};
	// class ServerSession
	/**
	 *
	 */
	class ClientSession : virtual public Session
	{
		public:
			/**
			 *
			 * @param aMessage
			 * @param io_service
			 * @param aResponseHandler
			 */
			ClientSession( 	Message aMessage,
							boost::asio::io_service& io_service,
							ResponseHandlerPtr aResponseHandler) :
							Session( io_service),
							message( aMessage),
							responseHandler( aResponseHandler)
			{
			}
			/**
			 *
			 */
			~ClientSession()
			{
			}
			/**
			 * @see Session::start()
			 */
			virtual void start()
			{
				writeMessage( message);
			}
			/**
			 * @see Session::handleMessageRead( Message& aMessage)
			 */
			virtual void handleMessageRead( Message& aMessage)
			{
				// This is the place where any reply message from the server should
				// be handled
				responseHandler->handleResponse( aMessage);

				delete this;
			}
			/**
			 * @see Session::handleMessageWritten( Message& aMessage)
			 */
			virtual void handleMessageWritten( Message& UNUSEDPARAM(aMessage))
			{
				// This *must* be the last function that is called after
				// sending a message because it will read the response...
				readMessage();
			}
		private:
			Message message;
			ResponseHandlerPtr responseHandler;
	};
//	class ClientSession

}// namespace Messaging

#endif // SESSION_HPP_
