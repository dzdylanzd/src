#include "RobotWorld.hpp"
#include "Logger.hpp"
#include "Robot.hpp"
#include "WayPoint.hpp"
#include "Goal.hpp"
#include "Wall.hpp"
#include <algorithm>
#include "MainApplication.hpp"
#include "PopulatePattern.h"

namespace Model
{
/**
 *
 */
/* static */RobotWorld& RobotWorld::RobotWorld::getRobotWorld()
{
	static RobotWorld robotWorld;
	return robotWorld;
}
/**
 *
 */
RobotPtr RobotWorld::newRobot(const std::string &aName /*= "New Robot"*/,
		const Point &aPosition /*= Point(-1,-1)*/,
		bool aNotifyObservers /*= true*/)
{
	RobotPtr robot(new Robot(aName, aPosition));
	robots.push_back(robot);
	if (aNotifyObservers == true)
	{
		notifyObservers();
	}
	return robot;
}
/**
 *
 */
WayPointPtr RobotWorld::newWayPoint(
		const std::string &aName /*= "new WayPoint"*/,
		const Point &aPosition /*= Point(-1,-1)*/,
		bool aNotifyObservers /*= true*/)
{
	WayPointPtr wayPoint(new WayPoint(aName, aPosition));
	wayPoints.push_back(wayPoint);
	if (aNotifyObservers == true)
	{
		notifyObservers();
	}
	return wayPoint;
}
/**
 *
 */
GoalPtr RobotWorld::newGoal(const std::string &aName /*= "New Goal"*/,
		const Point &aPosition /*= Point(-1,-1)*/,
		bool aNotifyObservers /*= true*/)
{
	GoalPtr goal(new Goal(aName, aPosition));
	goals.push_back(goal);
	if (aNotifyObservers == true)
	{
		notifyObservers();
	}
	return goal;
}
/**
 *
 */
WallPtr RobotWorld::newWall(const Point &aPoint1, const Point &aPoint2,
		bool aNotifyObservers /*= true*/)
{
	WallPtr wall(new Wall(aPoint1, aPoint2));
	walls.push_back(wall);
	if (aNotifyObservers == true)
	{
		notifyObservers();
	}
	return wall;
}
/**
 *
 */
void RobotWorld::deleteRobot(RobotPtr aRobot, bool aNotifyObservers /*= true*/)
{
	auto i = std::find_if(robots.begin(), robots.end(), [aRobot](RobotPtr r)
	{
		return aRobot->getName() == r->getName();
	});
	if (i != robots.end())
	{
		robots.erase(i);
		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
}
/**
 *
 */
void RobotWorld::deleteWayPoint(WayPointPtr aWayPoint,
		bool aNotifyObservers /*= true*/)
{
	auto i = std::find_if(wayPoints.begin(), wayPoints.end(),
			[aWayPoint](WayPointPtr w)
			{
				return aWayPoint->getName() == w->getName();
			});
	if (i != wayPoints.end())
	{
		wayPoints.erase(i);
		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
}
/**
 *
 */
void RobotWorld::deleteGoal(GoalPtr aGoal, bool aNotifyObservers /*= true*/)
{
	auto i = std::find_if(goals.begin(), goals.end(), [aGoal](GoalPtr g)
	{
		return aGoal->getName() == g->getName();
	});
	if (i != goals.end())
	{
		goals.erase(i);

		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
}
/**
 *
 */
void RobotWorld::deleteWall(WallPtr aWall, bool aNotifyObservers /*= true*/)
{
	auto i = std::find_if(walls.begin(), walls.end(), [aWall](WallPtr w)
	{
		return
		aWall->getPoint1() == w->getPoint1() &&
		aWall->getPoint2() == w->getPoint2();
	});
	if (i != walls.end())
	{
		walls.erase(i);

		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
}
/**
 *
 */
RobotPtr RobotWorld::getRobot(const std::string &aName) const
{
	for (RobotPtr robot : robots)
	{
		if (robot->getName() == aName)
		{
			return robot;
		}
	}
	return nullptr;
}
/**
 *
 */
RobotPtr RobotWorld::getRobot(const unsigned short robotId) const
{
	unsigned short RobotId = std::stoul(Application::MainApplication::getArg("-robot").value);
		std::string name;
	if(RobotId == 1){
	 name = "player1";
	}else if(RobotId == 2){
		name = "player2";
		}
	for (RobotPtr robot : robots)
	{
		if (robot->getName() == name)
		{
			return robot;
		}
	}
	return nullptr;
}
/**
 *
 */
RobotPtr RobotWorld::getRobot(const Base::ObjectId &anObjectId) const
{
	for (RobotPtr robot : robots)
	{
		if (robot->getObjectId() == anObjectId)
		{
			return robot;
		}
	}
	return nullptr;
}
/**
 *
 */
WayPointPtr RobotWorld::getWayPoint(const std::string &aName) const
{
	for (WayPointPtr wayPoint : wayPoints)
	{
		if (wayPoint->getName() == aName)
		{
			return wayPoint;
		}
	}
	return nullptr;
}
/**
 *
 */
WayPointPtr RobotWorld::getWayPoint(const Base::ObjectId &anObjectId) const
{
	for (WayPointPtr wayPoint : wayPoints)
	{
		if (wayPoint->getObjectId() == anObjectId)
		{
			return wayPoint;
		}
	}
	return nullptr;
}
/**
 *
 */
GoalPtr RobotWorld::getGoal(const std::string &aName) const
{
	for (GoalPtr goal : goals)
	{
		if (goal->getName() == aName)
		{
			return goal;
		}
	}
	return nullptr;
}
/**
 *
 */
GoalPtr RobotWorld::getGoal(const Base::ObjectId &anObjectId) const
{
	for (GoalPtr goal : goals)
	{
		if (goal->getObjectId() == anObjectId)
		{
			return goal;
		}
	}
	return nullptr;
}
/**
 *
 */
WallPtr RobotWorld::getWall(const Base::ObjectId &anObjectId) const
{
	for (WallPtr wall : walls)
	{
		if (wall->getObjectId() == anObjectId)
		{
			return wall;
		}
	}
	return nullptr;
}

std::string RobotWorld::getAllWallData() const{
	std::string os;
	for (WallPtr wall : walls)
	{
              os = os + wall->getPointData() + "\n";
	}
	return os;
}

/**
 *
 */
const std::vector<RobotPtr>& RobotWorld::getRobots() const
{
	return robots;
}
/**
 *
 */
const std::vector<WayPointPtr>& RobotWorld::getWayPoints() const
{
	return wayPoints;
}
/**
 *
 */
const std::vector<GoalPtr>& RobotWorld::getGoals() const
{
	return goals;
}
/**
 *
 */
const std::vector<WallPtr>& RobotWorld::getWalls() const
{
	return walls;
}
/**
 *
 */
void RobotWorld::populate(int aNumberOfWalls /*= 2*/)
{
	Application::Logger::log(
			Application::MainApplication::getArg("-robot").value);
	Application::Logger::log(
			Application::MainApplication::getArg("-pattern").value);
	PopulatePattern populater(
			std::stoul(Application::MainApplication::getArg("-robot").value));
	populater.populate(
			std::stoul(Application::MainApplication::getArg("-pattern").value));

	notifyObservers();
}
/**
 *
 */
void RobotWorld::unpopulate(bool aNotifyObservers /*= true*/)
{
	robots.clear();
	wayPoints.clear();
	goals.clear();
	walls.clear();

	if (aNotifyObservers)
	{
		notifyObservers();
	}
}
/**
 *
 */
void RobotWorld::unpopulate(const std::vector<Base::ObjectId> &aKeepObjects,
		bool aNotifyObservers /*= true*/)
{
	if (robots.size() > 0)
	{
		robots.erase(
				std::remove_if(robots.begin(), robots.end(),
						[&aKeepObjects](
								RobotPtr aRobot)
								{
									return std::find( aKeepObjects.begin(),
											aKeepObjects.end(),
											aRobot->getObjectId()) == aKeepObjects.end();
								}), robots.end());
	}
	if (wayPoints.size() > 0)
	{
		wayPoints.erase(
				std::remove_if(wayPoints.begin(), wayPoints.end(),
						[&aKeepObjects](
								WayPointPtr aWayPoint)
								{
									return std::find( aKeepObjects.begin(),
											aKeepObjects.end(),
											aWayPoint->getObjectId()) == aKeepObjects.end();
								}), wayPoints.end());
	}
	if (goals.size() > 0)
	{
		goals.erase(
				std::remove_if(goals.begin(), goals.end(),
						[&aKeepObjects](GoalPtr aGoal)
						{
							return std::find( aKeepObjects.begin(),
									aKeepObjects.end(),
									aGoal->getObjectId()) == aKeepObjects.end();
						}), goals.end());
	}
	if (walls.size() > 0)
	{
		walls.erase(
				std::remove_if(walls.begin(), walls.end(),
						[&aKeepObjects](WallPtr aWall)
						{
							return std::find( aKeepObjects.begin(),
									aKeepObjects.end(),
									aWall->getObjectId()) == aKeepObjects.end();
						}), walls.end());
	}

	if (aNotifyObservers)
	{
		notifyObservers();
	}
}
/**
 *
 */
std::string RobotWorld::asString() const
{
	return ModelObject::asString();
}
/**
 *
 */
std::string RobotWorld::asDebugString() const
{
	std::ostringstream os;

	os << asString() << '\n';

	for (RobotPtr ptr : robots)
	{
		os << ptr->asDebugString() << '\n';
	}
	for (WayPointPtr ptr : wayPoints)
	{
		os << ptr->asDebugString() << '\n';
	}
	for (GoalPtr ptr : goals)
	{
		os << ptr->asDebugString() << '\n';
	}
	for (WallPtr ptr : walls)
	{
		os << ptr->asDebugString() << '\n';
	}

	return os.str();
}
/**
 *
 */
RobotWorld::RobotWorld()
{
}
/**
 *
 */
RobotWorld::~RobotWorld()
{
	// No notification while I am in the destruction mode!
	disableNotification();
	unpopulate();
}

} // namespace Model
