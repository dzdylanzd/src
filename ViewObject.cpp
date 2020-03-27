/*
 * Copyright (c) 2014 Joost Kraaijeveld. See license.txt for details.
 * For information, bug reports and additions send an e-mail to RobotWorld@Askesis.nl.
 *
 * Author: jkr
 */

#include "ViewObject.hpp"

namespace View
{
	ViewObject::ViewObject() : objectId(Base::ObjectId::newObjectId())
	{
	}

	ViewObject::ViewObject(Model::ModelObjectPtr aModelObject) :
								objectId(Base::ObjectId::newObjectId()),
								modelObject(aModelObject)
	{
		if(modelObject)
		{
			handleNotificationsFor(*modelObject);
		}
	}

	ViewObject::~ViewObject()
	{
	}

	/**
	 *
	 */
	const Base::ObjectId& ViewObject::getObjectId() const
	{
		return objectId;
	}
	/**
	 *
	 */
	Model::ModelObjectPtr ViewObject::getModelObject() const
	{
		return modelObject;
	}
	/**
	 *
	 */
	void ViewObject::setModelObject(Model::ModelObjectPtr aModelObject)
	{
		rebindModelObject(aModelObject);
	}
	/**
	 * This function will call ModelObject::addReference.
	 * It will stop handling the notifications for the old object,
	 * start handling the notifications for the new object
	 * and reset this ViewObject (derived) class.
	 *
	 * See ViewObject::resetViewObject
	 */
	void ViewObject::rebindModelObject(Model::ModelObjectPtr aModelObject)
	{
		if(modelObject)
		{
			stopHandlingNotificationsFor(*modelObject);
		}

		modelObject = aModelObject;

		if(modelObject)
		{
			handleNotificationsFor(*modelObject);
		}
	}
} // namespace View


