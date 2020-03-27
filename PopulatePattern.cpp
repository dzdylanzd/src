/*
 * PopulatePattern.cpp
 *
 *  Created on: 26 mrt. 2020
 *      Author: dylan zweers
 */

#include "PopulatePattern.h"
#include "Logger.hpp"
#include "RobotWorld.hpp"
#include "Robot.hpp"

PopulatePattern::PopulatePattern(const unsigned short aRobotID) :
		robotID(aRobotID)
{

}

PopulatePattern::~PopulatePattern()
{
	// TODO Auto-generated destructor stub
}

void PopulatePattern::populate(const unsigned short pattern)
{
	switch (pattern)
	{
	case PopulatePattern::pattern::patternA:
	{
		PatternA();
		break;
	}
	case PopulatePattern::pattern::patternB:
	{
		PatternB();
		break;
	}
	case PopulatePattern::pattern::patternC:
	{
		PatternC();
		break;
	}
	}
}

void PopulatePattern::PatternA()
{
	if (robotID == 1)
	{
		Application::Logger::log("pattern A robot 1");
		Model::RobotWorld::getRobotWorld().newRobot( "player1", Point(100,10),false);
		Model::RobotWorld::getRobotWorld().newGoal("Goal", Point(400, 500),	false);
	}
	else if (robotID == 2)
	{

	}

}

void PopulatePattern::PatternB()
{
	if (robotID == 1)
	{
		Application::Logger::log("robot 1");
		Model::RobotWorld::getRobotWorld().newWall(Point(7, 234),
				Point(419, 234), false);
		Model::RobotWorld::getRobotWorld().newGoal("Goal", Point(500, 500),
				false);
	}
	else if (robotID == 2)
	{
		Model::RobotWorld::getRobotWorld().newWall(Point(7, 234),
				Point(419, 234), false);
		Model::RobotWorld::getRobotWorld().newGoal("Goal", Point(320, 285),
				false);
		Model::RobotWorld::getRobotWorld().newRobot( "Robot", Point(163,111),false);
	}
}

void PopulatePattern::PatternC()
{
}
