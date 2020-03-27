#ifndef MODELOBJECT_HPP_
#define MODELOBJECT_HPP_

/*
 * Copyright (c) 2014 Joost Kraaijeveld. See license.txt for details.
 * For information, bug reports and additions send an e-mail to RobotWorld@Askesis.nl.
 *
 * Author: jkr
 */

#include "Config.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <boost/noncopyable.hpp>

#include "Notifier.hpp"
#include "ObjectId.hpp"

namespace Model
{
	class ModelObject;
	typedef std::shared_ptr<ModelObject> ModelObjectPtr;

	/**
	 *
	 */
	class ModelObject : public boost::noncopyable,
						public Base::Notifier,
						public std::enable_shared_from_this<ModelObject>
	{
		public:
			/**
			 * @name Constructors and destructor
			 */
			//@{
			/**
			 *
			 */
			ModelObject();
			/**
			 *
			 */
			virtual ~ModelObject();
			//@}
			/**
			 * @name Operators
			 */
			//@{
			/**
			 * Equal to operator which compares the objectIds
			 *
			 * @param aModelObject
			 * @return true if objectIds are the same, false otherwise
			 */
			bool operator==( const ModelObject& aModelObject) const
			{
				return objectId == aModelObject.objectId;
			}
			/**
			 * Less than operator which compares the objectIds
			 *
			 * @param aModelObject
			 * @return true if lhs.objectId is less than rhs.objectId, false otherwise
			 */
			bool operator<( const ModelObject& aModelObject) const
			{
				return objectId < aModelObject.objectId;
			}
			//@}
			/**
			 *
			 * @return the objectId (identity) of the ModelObject
			 */
			const Base::ObjectId& getObjectId() const {return  objectId;}
			/**
			 * Converts the contained ModelObject to a std::shared_ptr<DestinationType>
			 *
			 * @return std::shared_ptr<DestinationType>
			 */
			template< typename DestinationType>
			std::shared_ptr<DestinationType> toPtr()
			{
				return std::dynamic_pointer_cast<DestinationType>(shared_from_this());
			}
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

		private:
			Base::ObjectId objectId;
	};
} // namespace Model
#endif // MODELOBJECT_HPP_ 
