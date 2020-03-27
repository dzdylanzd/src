#include "ObjectId.hpp"
#include <sys/timeb.h>  // _ftime(), time_t and struct timeb
#include <ctime>       // localtime()
#include <climits>
#include "Thread.hpp"

namespace Base
{
	/**
	 *
	 */
	/* static */ std::string ObjectId::objectIdNamespace = "";

	static std::mutex newObjectIdMutex;

	/* static */ObjectId ObjectId::newObjectId()
	{
		std::lock_guard< std::mutex > lock( newObjectIdMutex);

		static time_t seconds = 0;
		static unsigned long fraction = 0;

		time_t newSeconds = 0;
		struct timeb timeBuffer;

		ftime( &timeBuffer);
		newSeconds = timeBuffer.time;

		// Reset seconds and fraction if in a new seconds
		if (seconds < newSeconds)
		{
			seconds = newSeconds;
			fraction = 0;
		}

		// Prevent overflow to 0 within the second. This should only happen on fast machines
		// as the creation of the fraction part is processor speed dependent
		if (fraction == ULONG_MAX)
		{
			ftime( &timeBuffer);
			newSeconds = timeBuffer.time;

			if (seconds < newSeconds)
			{
				seconds = newSeconds;
				fraction = 0;
			} else
			{
				while (seconds >= newSeconds)
				{
					ftime( &timeBuffer);
					newSeconds = timeBuffer.time;
				}
				seconds = newSeconds;
				fraction = 0;
			}
		}

		time_t sec = seconds;
		struct tm* timeStruct = gmtime( &sec);
		if (!timeStruct)
		{
			fprintf( stderr, "timeStruct is nullptr, seconds were %lu\n", seconds);
			throw std::invalid_argument("timeStruct is nullptr");
		}

		// We actually only use 27 chars but it could be 93, based on the maximum int values....
		char objectIdBuffer[93] = {'\0'};
		sprintf( objectIdBuffer, "%04d-%02d-%02d-%02d-%02d-%02d-%06lu", timeStruct->tm_year + 1900, timeStruct->tm_mon + 1, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec, fraction);
		std::string objectIdString(ObjectId::objectIdNamespace);
		objectIdString.append(objectIdBuffer);
		ObjectId objectId( objectIdString);

		fraction++;
		return objectId;
	}

	ObjectId::~ObjectId()
	{
	}
	/**
	 *
	 */
	bool ObjectId::operator==( const ObjectId& anObjectId) const
	{
		return ((base&)(*this)) == ((base&)anObjectId);
	}
	/**
	 *
	 */
	bool ObjectId::operator<( const ObjectId& anObjectId) const
	{
		return ((base&)(*this)) < ((base&)anObjectId);
	}
	/**
	 *
	 */
	bool ObjectId::isNull() const
	{
		if (base::empty())
		{
			return true;
		}
		return false;
	}
	/**
	 *
	 */
	bool ObjectId::isValid() const
	{
		if (isNull())
		{
			return false;
		}
		return true;
	}
	/**
	 *
	 */
	/*virtual*/std::string ObjectId::asString() const
	{
		if (isNull())
		{
			return "";
		}
		return std::string( base::begin(), base::end());
	}

	/**
	 *
	 */
	/*virtual*/std::string ObjectId::asDebugString() const
	{
		return asString();
	}

	std::ostream& operator<<( 	std::ostream& os,
								const ObjectId& anObjectId)
	{
		return os << anObjectId.asString();
	}
} // namespace Base
