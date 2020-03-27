#ifndef ABSTRACTACTUATOR_HPP_
#define ABSTRACTACTUATOR_HPP_

#include "Config.hpp"
#include <string>
#include "ModelObject.hpp"

namespace Model
{
	class AbstractAgent;

	class AbstractCommand
	{
		public:
			/**
			 *
			 */
			virtual ~AbstractCommand()
			{
			}
	};
	//	class AbstractCommand

	class AbstractActuator  : public ModelObject
	{
		public:
			AbstractActuator();
			/**
			 *
			 */
			AbstractActuator( AbstractAgent* anAgent);
			/**
			 *
			 */
			virtual ~AbstractActuator();
			/**
			 *
			 */
			virtual void handleCommand( AbstractCommand& anAbstractCommand) = 0;
			/**
			 *
			 */
			virtual void attachAgent( AbstractAgent* anAgent);
			/**
			 *
			 */
			virtual void detachAgent();
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const;
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const;
			//@}
		protected:
			AbstractAgent* agent;
	};
	//	class AbstractActuator
} // namespace Model
#endif // ABSTRACTACTUATOR_HPP_
