#ifndef MESSAGE_HPP__
#define MESSAGE_HPP__

#include "Config.hpp"

#include <cstddef>
#include <iomanip>
#include <sstream>
#include <string>

/**
 *
 */
namespace Messaging
{
	/**
	 *
	 */
	struct Message
	{
			typedef std::string MessageBody;

			/**
			 *
			 */
			struct MessageHeader
			{
					/**
					 *
					 */
					MessageHeader() :
									messageType( 0),
									messageLength( 0)
					{
					}
					/**
					 *
					 * @param aMessageType
					 * @param aMessageLength
					 */
					MessageHeader( 	char aMessageType,
									unsigned long aMessageLength) :
									messageType( aMessageType),
									messageLength( aMessageLength)
					{
					}
					/**
					 *
					 * @param aMessageHeaderBuffer
					 */
					MessageHeader(	const std::string& aMessageHeaderBuffer) :
									messageType( 0),
									messageLength( 0)
					{
						fromString( aMessageHeaderBuffer);
					}
					/**
					 *
					 * @return ASCII string representation of the message header
					 */
					std::string toString() const
					{
						std::ostringstream os;
						os << magicNumber1 << magicNumber2 << magicNumber3 << magicNumber4 << majorVersion << minorVersion << messageType << std::setw(10 /* unsigned long : 4,294,967,295 ergo 10 numbers */) << messageLength;
						return os.str();
					}
					/**
					 * Stores a ASCII representaion of a message header into this header
					 *
					 * @param aString
					 */
					void fromString( const std::string& aString)
					{
						std::istringstream is( aString);
						char magic[4];
						char major;
						char minor;
						is >> magic[0] >> magic[1] >> magic[2] >> magic[3] >> major >> minor >> messageType >> std::setw( 10 /* unsigned long : 4,294,967,295 ergo 10 numbers */) >> messageLength;
					}
					/**
					 * @return The length of the header in bytes
					 */
					unsigned long getHeaderLength() const
					{
						static size_t l = toString().length();
						return (unsigned long)l;
					}
					/**
					 *
					 * @return The message tpe
					 */
					char getMessageType() const
					{
						return messageType;
					}
					/**
					 * @return The length of the message in bytes
					 */
					unsigned long getMessageLength() const
					{
						return messageLength;
					}
					/**
					 * @name Debug functions
					 */
					//@{
					/**
					 * Returns a 1-line description of the object
					 */
					std::string asString() const
					{
						std::ostringstream os;
						os << magicNumber1 << magicNumber2 << magicNumber3 << magicNumber4 << " " << majorVersion << " " <<  minorVersion << " " << (int)messageType << " " << messageLength;
						return os.str();
					}
					//@}

					static const char magicNumber1 = 'A';
					static const char magicNumber2 = 'S';
					static const char magicNumber3 = 'I';
					static const char magicNumber4 = 'O';
					static const char majorVersion = '1';
					static const char minorVersion = '0';
					char messageType;
					unsigned long messageLength;
			}; // struct MessageHeader
			/**
			 *
			 */
			Message() :
							messageType( 0)
			{
			}
			/**
			 *
			 * @param aMessageType
			 */
			Message( char aMessageType) :
							messageType( aMessageType)
			{
			}
			/**
			 *
			 * @param aMessageType
			 * @param aMessage
			 */
			Message( 	char aMessageType,
						const std::string& aMessage) :
							messageType( aMessageType),
							message( aMessage)
			{
			}
			/**
			 *
			 * @param aMessage
			 */
			Message( const Message& aMessage) :
							messageType( aMessage.messageType),
							message( aMessage.message)
			{
			}
			/**
			 *
			 */
			virtual ~Message()
			{
			}
			/**
			 *
			 * @return
			 */
			MessageHeader getHeader() const
			{
				return MessageHeader( messageType, message.length());
			}
			/**
			 *
			 * @param aHeader
			 */
			void setHeader( const MessageHeader& aHeader)
			{
				setMessageType( aHeader.messageType);
				message.resize( aHeader.messageLength);
			}
			/**
			 *
			 * @return
			 */
			char getMessageType() const
			{
				return messageType;
			}
			/**
			 *
			 * @param aMessageType
			 */
			void setMessageType( char aMessageType)
			{
				messageType = aMessageType;
			}
			/**
			 *
			 * @return
			 */
			MessageBody getBody() const
			{
				return message;
			}
			/**
			 *
			 * @param aBody
			 */
			void setBody( const std::string& aBody)
			{
				message = aBody;
			}
			/**
			 * @return The length of the message in bytes
			 */
			unsigned long length() const
			{
				return message.length();
			}
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const
			{
				std::ostringstream os;
				os << getHeader().asString() << ":\t" << message;
				return os.str();
			}
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const
			{
				return asString();
			}
			//@}

			char messageType;
			MessageBody message;
	}; // struct Message

} // namespace Messaging

#endif // MESSAGE_HPP__
