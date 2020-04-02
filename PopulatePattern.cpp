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
	drawWalls();
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
		Model::RobotWorld::getRobotWorld().newRobot("player1", Point(50, 50), false);
		Model::RobotWorld::getRobotWorld().newGoal("goal", Point(450, 450), false);
		Model::RobotWorld::getRobotWorld().newGoal("home",  Point(50, 50), false);

	}
	else if (robotID == 2)
	{
		Application::Logger::log("pattern A robot 2");
		Model::RobotWorld::getRobotWorld().newRobot("player2", Point(400, 450), false);
		Model::RobotWorld::getRobotWorld().newGoal("goal", Point(100, 50), false);
		Model::RobotWorld::getRobotWorld().newGoal("home", Point(400, 450), false);

	}
}

void PopulatePattern::PatternB()
{
	if (robotID == 1)
	{
		Application::Logger::log("pattern B robot 1");
		Model::RobotWorld::getRobotWorld().newGoal("home", Point(250, 50), false);
		Model::RobotWorld::getRobotWorld().newRobot("player1", Point(250, 50), false);
		Model::RobotWorld::getRobotWorld().newGoal("goal", Point(250, 450), false);

	}
	else if (robotID == 2)
	{
		Application::Logger::log("pattern B robot 2");
		Model::RobotWorld::getRobotWorld().newGoal("home", Point(50, 250), false);
		Model::RobotWorld::getRobotWorld().newRobot("player2", Point(50, 250), false);
		Model::RobotWorld::getRobotWorld().newGoal("goal", Point(450, 250), false);

	}
}

void PopulatePattern::PatternC()
{
	if (robotID == 1)
	{
		Application::Logger::log("pattern C robot 1");
		Model::RobotWorld::getRobotWorld().newRobot("player1", Point(50, 50), false);
		Model::RobotWorld::getRobotWorld().newGoal("Goal P1", Point(450, 450), false);
		Model::RobotWorld::getRobotWorld().newWall(Point(0, 150), Point(300, 150));


	}
	else if (robotID == 2)
	{
		Application::Logger::log("pattern C robot 2");
		Model::RobotWorld::getRobotWorld().newRobot("player2", Point(400, 450), false);
		Model::RobotWorld::getRobotWorld().newGoal("Goal P2", Point(100, 50), false);
		Model::RobotWorld::getRobotWorld().newWall(Point(200, 300), Point(500, 300));

	}
}

void PopulatePattern::drawWalls()
{
	// top wall
	Model::RobotWorld::getRobotWorld().newWall(Point(0,0), Point(500, 0));
	// right wall
	Model::RobotWorld::getRobotWorld().newWall(Point(500,0), Point(500, 500));
	// bottom wall
	Model::RobotWorld::getRobotWorld().newWall(Point(0, 500), Point(500, 500));
	// left wall
	Model::RobotWorld::getRobotWorld().newWall(Point(0,0), Point(0, 500));
}
