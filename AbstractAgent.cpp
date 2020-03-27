#include "AbstractAgent.hpp"
#include "AbstractSensor.hpp"
#include "AbstractActuator.hpp"
#include "Logger.hpp"
#include <sstream>

namespace Model
{
	/**
	 *
	 */
	AbstractAgent::AbstractAgent()
	{
	}
	/**
	 *
	 */
	AbstractAgent::~AbstractAgent()
	{
	}
	/**
	 *
	 */
	void AbstractAgent::attachSensor( 	std::shared_ptr< AbstractSensor > aSensor,
										bool attachSensorToAgent /*= false*/)
	{
		sensors.push_back( aSensor);
		if (attachSensorToAgent == true)
		{
			aSensor->attachAgent( this);
		}
	}
	/**
	 *
	 */
	void AbstractAgent::attachActuator( std::shared_ptr< AbstractActuator > anActuator,
										bool attachActuatorToAgent /*= false*/)
	{
		actuators.push_back( anActuator);
		if (attachActuatorToAgent == true)
		{
			anActuator->attachAgent( this);
		}
	}
	/**
	 *
	 */
	void AbstractAgent::addPercept( std::shared_ptr< AbstractPercept > aPercept)
	{
		perceptQueue.enqueue( aPercept);
	}
	/**
	 *
	 */
	std::string AbstractAgent::asString() const
	{
		std::ostringstream os;
		os << "AbstractAgent";
		return os.str();
	}
	/**
	 *
	 */
	std::string AbstractAgent::asDebugString() const
	{
		std::ostringstream os;

		os << "AbstractAgent:\n";
		os << "sensors:\n";
		for (std::shared_ptr< AbstractSensor > sensor : sensors)
		{
			os << sensor->asDebugString() << "\n";
		}
		os << "actuators:\n";
		for (std::shared_ptr< AbstractActuator > actuator : actuators)
		{
			os << actuator->asDebugString() << "\n";
		}

		return os.str();
	}
} // namespace Model
