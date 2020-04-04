#include "Robot.hpp"
#include <sstream>
#include <ctime>
#include <chrono>
#include "Thread.hpp"
#include "MathUtils.hpp"
#include "Logger.hpp"
#include "Goal.hpp"
#include "WayPoint.hpp"
#include "Wall.hpp"
#include "RobotWorld.hpp"
#include "Shape2DUtils.hpp"
#include "CommunicationService.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "MainApplication.hpp"
#include "LaserDistanceSensor.hpp"

namespace Model
{
/**
 *
 */
Robot::Robot() :
		name(""), size( DefaultSize), position( DefaultPosition), front(0, 0), speed(
				0.0), acting(false), driving(false), communicating(false), otherReady(
				false)
{
	std::shared_ptr<AbstractSensor> laserSensor(new LaserDistanceSensor(this));
	attachSensor(laserSensor);
}
/**
 *
 */
Robot::Robot(const std::string &aName) :
		name(aName), size( DefaultSize), position( DefaultPosition), front(0,
				0), speed(0.0), acting(false), driving(false), communicating(
				false), otherReady(false)
{
	std::shared_ptr<AbstractSensor> laserSensor(new LaserDistanceSensor(this));
	attachSensor(laserSensor);
}
/**
 *
 */
Robot::Robot(const std::string &aName, const Point &aPosition) :
		name(aName), size( DefaultSize), position(aPosition), front(0, 0), speed(
				0.0), acting(false), driving(false), communicating(false), otherReady(
				false)
{
	std::shared_ptr<AbstractSensor> laserSensor(new LaserDistanceSensor(this));
	attachSensor(laserSensor);
}
/**
 *
 */
Robot::~Robot()
{
	if (driving)
	{
		stopDriving();
	}
	if (acting)
	{
		stopActing();
	}
	if (communicating)
	{
		stopCommunicating();
	}
}
/**
 *
 */
void Robot::setName(const std::string &aName, bool aNotifyObservers /*= true*/)
{
	name = aName;
	if (aNotifyObservers == true)
	{
		notifyObservers();
	}

}
/**
 *
 */
Size Robot::getSize() const
{
	return size;
}
/**
 *
 */
void Robot::setSize(const Size &aSize, bool aNotifyObservers /*= true*/)
{
	size = aSize;
	if (aNotifyObservers == true)
	{
		notifyObservers();
	}
}
/**
 *
 */
void Robot::setPosition(const Point &aPosition,
		bool aNotifyObservers /*= true*/)
{
	position = aPosition;
	if (aNotifyObservers == true)
	{
		notifyObservers();
	}
}
/**
 *
 */
BoundedVector Robot::getFront() const
{
	return front;
}
/**
 *
 */
void Robot::setFront(const BoundedVector &aVector,
		bool aNotifyObservers /*= true*/)
{
	front = aVector;
	if (aNotifyObservers == true)
	{
		notifyObservers();
	}
}
/**
 *
 */
float Robot::getSpeed() const
{
	return speed;
}
/**
 *
 */
void Robot::setSpeed(float aNewSpeed, bool aNotifyObservers /*= true*/)
{
	speed = aNewSpeed;
	if (aNotifyObservers == true)
	{
		notifyObservers();
	}
}
/**
 *
 */
void Robot::startActing()
{
	acting = true;
	std::thread newRobotThread([this]
	{	startDriving();});
	robotThread.swap(newRobotThread);
}
/**
 *
 */
void Robot::stopActing()
{
	acting = false;
	driving = false;
	robotThread.join();
}
/**
 *
 */
void Robot::startDriving(std::string goalName)
{
	Application::Logger::log(name + "gaat naar : " + goalName);
	driving = true;
	OriginalGoal = RobotWorld::getRobotWorld().getGoal("goal");
	goal = RobotWorld::getRobotWorld().getGoal(goalName);
	homeGoal = RobotWorld::getRobotWorld().getGoal("home");
	calculateRoute(goal);
	drive();
}
/**
 *
 */
void Robot::stopDriving()
{
	driving = false;
}
/**
 *
 */
void Robot::startCommunicating()
{
	if (!communicating)
	{
		communicating = true;

		std::string localPort = "12345";
		if (Application::MainApplication::isArgGiven("-local_port"))
		{
			localPort =
					Application::MainApplication::getArg("-local_port").value;
		}

		Messaging::CommunicationService::getCommunicationService().runRequestHandler(
				toPtr<Robot>(),
				static_cast<unsigned short>(std::stoi(localPort)));

	}
}
/**
 *
 */
void Robot::stopCommunicating()
{
	if (communicating)
	{
		communicating = false;

		std::string localPort = "12345";
		if (Application::MainApplication::isArgGiven("-local_port"))
		{
			localPort =
					Application::MainApplication::getArg("-local_port").value;
		}

		Messaging::Client c1ient("localhost", localPort, toPtr<Robot>());
		Messaging::Message message(1, "stop");
		c1ient.dispatchMessage(message);
	}
}
/**
 *
 */
Region Robot::getRegion() const
{
	Point translatedPoints[] =
	{ getFrontRight(), getFrontLeft(), getBackLeft(), getBackRight() };
	return Region(4, translatedPoints);
}
/**
 *
 */
bool Robot::intersects(const Region &aRegion) const
{
	Region region = getRegion();
	region.Intersect(aRegion);
	return !region.IsEmpty();
}
/**
 *
 */
Point Robot::getFrontLeft() const
{
	// x and y are pointing to top left now
	int x = position.x - (size.x / 2);
	int y = position.y - (size.y / 2);

	Point originalFrontLeft(x, y);
	double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

	Point frontLeft(
			static_cast<int>((originalFrontLeft.x - position.x)
					* std::cos(angle)
					- (originalFrontLeft.y - position.y) * std::sin(angle)
					+ position.x),
			static_cast<int>((originalFrontLeft.y - position.y)
					* std::cos(angle)
					+ (originalFrontLeft.x - position.x) * std::sin(angle)
					+ position.y));

	return frontLeft;
}
/**
 *
 */
Point Robot::getFrontRight() const
{
	// x and y are pointing to top left now
	int x = position.x - (size.x / 2);
	int y = position.y - (size.y / 2);

	Point originalFrontRight(x + size.x, y);
	double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

	Point frontRight(
			static_cast<int>((originalFrontRight.x - position.x)
					* std::cos(angle)
					- (originalFrontRight.y - position.y) * std::sin(angle)
					+ position.x),
			static_cast<int>((originalFrontRight.y - position.y)
					* std::cos(angle)
					+ (originalFrontRight.x - position.x) * std::sin(angle)
					+ position.y));

	return frontRight;
}
/**
 *
 */
Point Robot::getBackLeft() const
{
	// x and y are pointing to top left now
	int x = position.x - (size.x / 2);
	int y = position.y - (size.y / 2);

	Point originalBackLeft(x, y + size.y);

	double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

	Point backLeft(
			static_cast<int>((originalBackLeft.x - position.x) * std::cos(angle)
					- (originalBackLeft.y - position.y) * std::sin(angle)
					+ position.x),
			static_cast<int>((originalBackLeft.y - position.y) * std::cos(angle)
					+ (originalBackLeft.x - position.x) * std::sin(angle)
					+ position.y));

	return backLeft;

}
/**
 *
 */
Point Robot::getBackRight() const
{
	// x and y are pointing to top left now
	int x = position.x - (size.x / 2);
	int y = position.y - (size.y / 2);

	Point originalBackRight(x + size.x, y + size.y);

	double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

	Point backRight(
			static_cast<int>((originalBackRight.x - position.x)
					* std::cos(angle)
					- (originalBackRight.y - position.y) * std::sin(angle)
					+ position.x),
			static_cast<int>((originalBackRight.y - position.y)
					* std::cos(angle)
					+ (originalBackRight.x - position.x) * std::sin(angle)
					+ position.y));

	return backRight;
}
/**
 *
 */
void Robot::handleNotification()
{
	//	std::unique_lock<std::recursive_mutex> lock(robotMutex);

	static int update = 0;
	if ((++update % 200) == 0)
	{
		notifyObservers();
	}
}
/**
 *
 */
void Robot::handleRequest(Messaging::Message &aMessage)
{
	switch (aMessage.getMessageType())
	{
	case EchoRequest:
	{
		Application::Logger::log(
				__PRETTY_FUNCTION__ + std::string(": EchoRequest"));

		aMessage.setMessageType(EchoResponse);
		aMessage.setBody(": case 1 " + aMessage.asString());
		break;
	}
	case MergeRequest:
	{
		Model::RobotWorld::getRobotWorld().sendMergeData(
				Robot::MessageType::MergeResponse);
		Application::Logger::log(aMessage.getBody());
		Model::RobotWorld::getRobotWorld().mergeWorlds(aMessage.getBody());
		break;
	}
	case MergeResponse:
	{
		Application::Logger::log(aMessage.getBody());
		Model::RobotWorld::getRobotWorld().mergeWorlds(aMessage.getBody());
		break;
	}
	case StartRequest:
	{
		Application::Logger::log("start test");
		startActing();
		break;
	}
	case Location:
	{

		Model::RobotWorld::getRobotWorld().updateOtherRobot(aMessage.getBody());
		break;
	}
	case ReadyToStart:
	{

		otherReady = true;
		break;
	}
	case Done:
	{
		Application::Logger::log("DE ANDERE IS KLAAR");
		otherDone = true;
		break;
	}
	case GoingHome:
	{
		otherGoingHome = true;
		break;
	}
	case AtHome:
	{
		otherGoingHome = false;
		break;
	}
	case PathToGoalState:
	{
		otherHasPathToGoal = (aMessage.getBody() == "1");
		break;
	}
	default:
	{
		Application::Logger::log("DEFAULT: ");
		Application::Logger::log(aMessage.getBody());
	}
	}
}
/**
 *
 */
void Robot::handleResponse(const Messaging::Message &aMessage)
{
	switch (aMessage.getMessageType())
	{
	case EchoResponse:
	{
		Application::Logger::log(
				__PRETTY_FUNCTION__
						+ std::string(": case EchoResponse: not implemented, ")
						+ aMessage.asString());

		break;
	}
	}
}
/**
 *
 */
std::string Robot::asString() const
{
	std::ostringstream os;

	os << "Robot " << name << " at (" << position.x << "," << position.y << ")";

	return os.str();
}
/**
 *
 */
std::string Robot::asDebugString() const
{
	std::ostringstream os;

	os << "Robot:\n";
	os << AbstractAgent::asDebugString();
	os << "Robot " << name << " at (" << position.x << "," << position.y
			<< ")\n";

	return os.str();
}
/**
 *
 */
void Robot::drive()
{
	try
	{
		speed = 1;
		while (driving)
		{
			unsigned long pathPoint = 0;
			while (position.x > 0 && position.x < 500 && position.y > 0 && position.y < 500 && pathPoint < path.size())
			{
				otherReady = false;
				sendReady();
				sendLocation();
				if (detectedOtherRobot())
				{
					// If a robot is detected try to calculate a new route to the goal
					// and set the pathPoint to 0 otherwise the robot will teleport
					calculateRoute(goal);
					pathPoint = 0;
				}

				// Check if there is a path
				if (pathPoint < path.size())
				{
					// Move the robot 1 step over the path
					const PathAlgorithm::Vertex &vertex = path[pathPoint += static_cast<int>(speed)];
					setFront(BoundedVector(vertex.asPoint(), position));
					position.x = vertex.x;
					position.y = vertex.y;

					notifyObservers();
				}

				if (arrived(goal))
				{
					if (goal->getName() == "home")
					{
						startDriving("goal");
						Application::Logger::log(name + ": has reached its home.");
					}
					else if (goal->getName() == "goal")
					{
						driving = false;
						Application::Logger::log(name + ": has reached its goal.");
						std::this_thread::sleep_for(std::chrono::milliseconds(30));
						sendDone();
						stopActing();
						break;
					}
				}

				sendReady();

				while (!otherReady)
				{
					// If the other is done instantly break the while loop
					if (otherDone)
					{
						break;
					}
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(30));
				// this should be the last thing in the loop
				if (driving == false)
				{
					return;
				}
			} // while

			sendReady();
			calculateRoute(goal);
			std::this_thread::sleep_for(std::chrono::milliseconds(30));

			if (name == "player1")
			{
				setFront(BoundedVector(Model::RobotWorld::getRobotWorld().getGoal("home")->getPosition(), position));
				position = getFrontLeft();
				startDriving("home");
			}
		}
	}
	catch (std::exception &e)
	{
		std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
		Application::Logger::log("er ging iets erg mis");
	}
	catch (...)
	{
		std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
		Application::Logger::log("er ging iets erg mis");
	}
}
/**
 *
 */
void Robot::sendLocation()
{
	sendMessage(Model::Robot::MessageType::Location, asString() + front.asString());
}
/**
 *
 */
void Robot::sendDone()
{
	// This part of the code needs to be repeated otherwise the message does not get sent
    unsigned short RobotId = std::stoul(Application::MainApplication::getArg("-robot").value);
    Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot(RobotId);
    if (robot)
    {
        std::string remoteIpAdres = "localhost";
        std::string remotePort = "12345";
        if (Application::MainApplication::isArgGiven("-remote_ip"))
        {
            remoteIpAdres = Application::MainApplication::getArg("-remote_ip").value;
        }
        if (Application::MainApplication::isArgGiven("-remote_port"))
        {
            remotePort = Application::MainApplication::getArg("-remote_port").value;
        }
        Messaging::Client client(remoteIpAdres, remotePort, robot);
        Messaging::Message message(Model::Robot::MessageType::Done, "done");
        client.dispatchMessage(message);
    }
}
/**
 *
 */
void Robot::sendReady()
{
	sendMessage(Model::Robot::MessageType::ReadyToStart, "ready");
}

void Robot::calculateRoute(GoalPtr aGoal)
{
	path.clear();
	if (aGoal)
	{
		Application::Logger::setDisable();
//		handleNotificationsFor(astar);
		path = astar.search(position, aGoal->getPosition(), size);
//		stopHandlingNotificationsFor(astar);

		Application::Logger::setDisable(false);

	}
}

bool Robot::arrived(GoalPtr aGoal)
{
	if (aGoal && intersects(aGoal->getRegion()))
	{
		return true;
	}
	return false;
}


bool Robot::collision()
{
	Point frontLeft = getFrontLeft();
	Point frontRight = getFrontRight();
	Point backLeft = getBackLeft();
	Point backRight = getBackRight();

	const std::vector<WallPtr> &walls = RobotWorld::getRobotWorld().getWalls();
	for (WallPtr wall : walls)
	{
		if (Utils::Shape2DUtils::intersect(frontLeft, frontRight,
				wall->getPoint1(), wall->getPoint2())
				|| Utils::Shape2DUtils::intersect(frontLeft, backLeft,
						wall->getPoint1(), wall->getPoint2())
				|| Utils::Shape2DUtils::intersect(frontRight, backRight,
						wall->getPoint1(), wall->getPoint2()))
		{
			return true;
		}
	}
	return false;
}
/**
 *
 */
bool Robot::detectedOtherRobot()
{
	//Make an array for the robot points
	Point robotPoints[8];
	unsigned short robotID = std::stoi(Application::MainApplication::getArg("-robot").value);
	if (robotID == 1)
	{
		robotPoints[0] = Model::RobotWorld::getRobotWorld().getRobot("player2")->getFrontLeft();
		robotPoints[1] = Model::RobotWorld::getRobotWorld().getRobot("player2")->getFrontRight();
		robotPoints[2] = Model::RobotWorld::getRobotWorld().getRobot("player2")->getBackRight();
		robotPoints[3] = Model::RobotWorld::getRobotWorld().getRobot("player2")->getFrontRight();
		robotPoints[4] = Model::RobotWorld::getRobotWorld().getRobot("player2")->getBackLeft();
		robotPoints[5] = Model::RobotWorld::getRobotWorld().getRobot("player2")->getFrontLeft();
		robotPoints[6] = Model::RobotWorld::getRobotWorld().getRobot("player2")->getBackLeft();
		robotPoints[7] = Model::RobotWorld::getRobotWorld().getRobot("player2")->getBackRight();
	}
	else if (robotID == 2)
	{
		robotPoints[0] = Model::RobotWorld::getRobotWorld().getRobot("player1")->getFrontLeft();
		robotPoints[1] = Model::RobotWorld::getRobotWorld().getRobot("player1")->getFrontRight();
		robotPoints[2] = Model::RobotWorld::getRobotWorld().getRobot("player1")->getBackRight();
		robotPoints[3] = Model::RobotWorld::getRobotWorld().getRobot("player1")->getFrontRight();
		robotPoints[4] = Model::RobotWorld::getRobotWorld().getRobot("player1")->getBackLeft();
		robotPoints[5] = Model::RobotWorld::getRobotWorld().getRobot("player1")->getFrontLeft();
		robotPoints[6] = Model::RobotWorld::getRobotWorld().getRobot("player1")->getBackLeft();
		robotPoints[7] = Model::RobotWorld::getRobotWorld().getRobot("player1")->getBackRight();
	}
	for (int i = 0; i < 8; i = i + 2)
	{
		if (Utils::Shape2DUtils::isOnLine(robotPoints[i], robotPoints[i + 1], position, 50))
		{
			return true;
		}
	}
	return false;
}
/**
 *
 */
void Robot::sendMessage(const Robot::MessageType type, const std::string& messageBody)
{
	unsigned short RobotId = std::stoul(Application::MainApplication::getArg("-robot").value);
	Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot(RobotId);
	if (robot)
	{
		// Defaults
		std::string remoteIpAdres = "localhost";
		std::string remotePort = "12345";
		if (Application::MainApplication::isArgGiven("-remote_ip"))
		{
				remoteIpAdres = Application::MainApplication::getArg("-remote_ip").value;
		}
		if (Application::MainApplication::isArgGiven("-remote_port"))
		{
				remotePort = Application::MainApplication::getArg("-remote_port").value;
		}

		// Create a client and message to send the message
		Messaging::Client client(remoteIpAdres, remotePort, robot);
		Messaging::Message message(type, messageBody);
		client.dispatchMessage(message);
	}
}

} // namespace Model
