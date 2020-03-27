#ifndef VIEWOBJECT_HPP_
#define VIEWOBJECT_HPP_

/*
 * Copyright (c) 2014 Joost Kraaijeveld. See license.txt for details.
 * For information, bug reports and additions send an e-mail to RobotWorld@Askesis.nl.
 *
 * Author: jkr
 */

#include "Config.hpp"
#include "Observer.hpp"
#include "ObjectId.hpp"
#include "ModelObject.hpp"

namespace View
{
	/**
	 *
	 */
	class ViewObject : public Base::Observer
	{
		public:
			/**
			 * @name Constructors and destructor
			 */
			//@{
			/**
			 * Default constructor
			 */
			ViewObject();
			/**
			 * ModelObject constructor
			 */
			ViewObject(Model::ModelObjectPtr aModelObject);
			/**
			 * Destructor
			 */
			virtual ~ViewObject();
			//@}

			/**
			 * @name Accessors and mutators
			 */
			//@{
			/**
			 * Accessor for the ViewObject objectId
			 */
			const Base::ObjectId& getObjectId() const;
			/**
			 * Accessor for the ModelObject
			 */
			Model::ModelObjectPtr getModelObject() const;
			/**
			 * Mutator for the ModelObject
			 *
			 * See rebindModelObject and resetViewObject
			 */
			void setModelObject(Model::ModelObjectPtr aModelObject) ;
			//@}
		protected:
			/**
			 * @name ViewObject implementation
			 */
			//@{
			/**
			 * Stops observing the current ModelObject, starts observing the new ModelObjects and
			 * displays it by calling ViewObject::resetViewObject
			 */
			virtual void rebindModelObject(Model::ModelObjectPtr aModelObject);
			//@}

		private:
			Base::ObjectId objectId;
			Model::ModelObjectPtr modelObject;
	};
} // namespace View
#endif // VIEWOBJECT_HPP_ 
