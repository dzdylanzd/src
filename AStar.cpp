#include <AStar.hpp>
#include <RobotWorld.hpp>
#include <Shape2DUtils.hpp>
#include <Wall.hpp>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace PathAlgorithm
{
	/**
	 *
	 */
	double ActualCost(	const Vertex& aStart,
						const Vertex& aGoal)
	{
		return std::sqrt( (aStart.x - aGoal.x) * (aStart.x - aGoal.x) + (aStart.y - aGoal.y) * (aStart.y - aGoal.y));
	}
	/**
	 *
	 */
	double HeuristicCost(	const Vertex& aStart,
							const Vertex& aGoal)
	{
		return std::sqrt( (aStart.x - aGoal.x) * (aStart.x - aGoal.x) + (aStart.y - aGoal.y) * (aStart.y - aGoal.y));
	}
	/**
	 *
	 */
	Path ConstructPath(	VertexMap& aPredecessorMap,
						const Vertex& aCurrentNode)
	{
		VertexMap::iterator i = aPredecessorMap.find( aCurrentNode);
		if (i != aPredecessorMap.end())
		{
			Path path = ConstructPath( aPredecessorMap, (*i).second);
			path.push_back( aCurrentNode);
			return path;
		} else
		{
			Path path;
			path.push_back( aCurrentNode);
			return path;
		}
	}
	/**
	 *
	 */
	std::vector< Vertex > GetNeighbours(	const Vertex& aVertex,
											int aFreeRadius /*= 1*/)
				{
		static int xOffset[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
		static int yOffset[] = { 1, 1, 0, -1, -1, -1, 0, 1 };

		const std::vector< Model::WallPtr >& walls = Model::RobotWorld::getRobotWorld().getWalls();
		std::vector< Vertex > neighbours;

		for (int i = 0; i < 8; ++i)
		{
			bool addToNeigbours = true;

			Vertex vertex( aVertex.x + xOffset[i], aVertex.y + yOffset[i]);
			for (Model::WallPtr wall : walls)
			{
				if (Utils::Shape2DUtils::isOnLine( wall->getPoint1(), wall->getPoint2(), vertex.asPoint(), aFreeRadius))
				{
					addToNeigbours = false;
					break;
				}
			}
			if (addToNeigbours == true)
			{
				neighbours.push_back( vertex);
			}
		}
		return neighbours;
				}
	/**
	 *
	 */
	std::vector< Edge > GetNeighbourConnections(	const Vertex& aVertex,
													int aFreeRadius /*= 1*/)
				{
		std::vector< Edge > connections;

		const std::vector< Vertex >& neighbours = GetNeighbours( aVertex, aFreeRadius);
		for (const Vertex& vertex : neighbours)
		{
			connections.push_back( Edge( aVertex, vertex));
		}

		return connections;
				}
	/**
	 *
	 */
	Path AStar::search(	const Point& aStartPoint,
						const Point& aGoalPoint,
						const Size& aRobotSize)
	{
		Vertex start( aStartPoint);
		Vertex goal( aGoalPoint);

		Path path = AStar::search( start, goal, aRobotSize);
		return path;
	}
	/**
	 *
	 */
	Path AStar::search( Vertex aStart,
						const Vertex& aGoal,
						const Size& aRobotSize)
	{
		getOS().clear();
		getCS().clear();
		getPM().clear();

		int radius = static_cast<int>(std::ceil(std::sqrt( (aRobotSize.x / 2.0) * (aRobotSize.x / 2.0) + (aRobotSize.y / 2.0) * (aRobotSize.y / 2.0))));

		aStart.actualCost = 0.0; 													// Cost from aStart along the best known path.
		aStart.heuristicCost = aStart.actualCost + HeuristicCost( aStart, aGoal);	// Estimated total cost from aStart to aGoal through y.

		addToOpenSet(aStart);

		//		long long begin = std::clock();

		while (!openSet.empty())
		{
			Vertex current = *openSet.begin();

			if (current.equalPoint( aGoal))
			{
				return ConstructPath( predecessorMap, current);
			} else
			{
				addToClosedSet( current);
				removeFirstFromOpenSet();

				const std::vector< Edge >& connections = GetNeighbourConnections( current, radius);
				for (const Edge& connection : connections)
				{
					Vertex neighbour = connection.otherSide( current);

					// The new costs
					double calculatedActualNeighbourCost = current.actualCost + ActualCost( current, neighbour);
					double totalHeuristicCostNeighbour = calculatedActualNeighbourCost + HeuristicCost( neighbour, aGoal);

					OpenSet::iterator openVertex = findInOpenSet( neighbour);
					if (openVertex != openSet.end())
					{
						// if neighbour is in the openSet we may have found a shorter via-route
						if ((*openVertex).heuristicCost <= totalHeuristicCostNeighbour)
						{
							continue;
						} else
						{
							removeFromOpenSet( openVertex);
						}
					}
					ClosedSet::iterator closedVertex = findInClosedSet( neighbour);
					if (closedVertex != closedSet.end())
					{
						// if neighbour is in the closedSet we may have found a shorter via-route
						if ((*closedVertex).heuristicCost <= totalHeuristicCostNeighbour)
						{
							continue;
						} else
						{
							removeFromClosedSet( closedVertex);
						}
					}

					neighbour.actualCost = calculatedActualNeighbourCost;
					neighbour.heuristicCost = totalHeuristicCostNeighbour;

					std::pair< VertexMap::iterator, bool > insertResult1 = predecessorMap.insert( std::make_pair( neighbour, current));
					if (insertResult1.second != true)
					{
						if (!(*insertResult1.first).first.equalPoint( neighbour))
						{
							std::ostringstream os;
							os << "*** (*insertResult1.first).first != neighbour:\n\t\t" << (*insertResult1.first).first << " <- " << (*insertResult1.first).second << "\n\t\t" << neighbour << " <- " << current;
							throw std::runtime_error(os.str());
						}

						if ((*insertResult1.first).first.heuristicCost > neighbour.heuristicCost)
						{
							predecessorMap.erase( insertResult1.first);
							std::pair< VertexMap::iterator, bool > insertResult2 = predecessorMap.insert( std::make_pair( neighbour, current));
							if (insertResult2.second != true)
							{
								std::ostringstream os;
								os << "**** Failed updating neighbour in the predecessorMap:  " << neighbour << " <- " << current << "\n";
								if (insertResult2.first != predecessorMap.end())
								{
									os << "\tcurrent value in the predecessorMap:  " << (*insertResult2.first).first << " <- " << (*insertResult2.first).second;
								} else
								{
									os << "\tinvalid iterator ";
								}
								throw std::runtime_error(os.str());
							} else
							{
							}
						}
					} else
					{
					}

					// if neighbour is not in openSet, add it to the openSet
					// which should always be the case?
					openVertex = findInOpenSet( neighbour);
					if (openVertex == openSet.end())
					{
						addToOpenSet( neighbour);
					} else
					{
						std::cerr << "**** Failed adding neighbour to the openSet:  " << neighbour << std::endl;
					}
				} //for(Edge connection : connections)

				//			Improving the performance 28-04-2014.....
				//
				//			Sorting after the insertion of an individual:
				//			aRobotSize = (37,29), radius = 23
				//			Duration: 17.349.490 openSet: 1507 closedSet: 76521 predecessorMap: 78027
				//
				//			After the insert of all:
				//			aRobotSize = (37,29), radius = 23
				//			Duration: 7.733.159 openSet: 1507 closedSet: 76521 predecessorMap: 78027
				//
				//			Partial sort after insert of all:
				//			aRobotSize = (37,29), radius = 23
				//			Duration: 3.297.669 openSet: 1507 closedSet: 76521 predecessorMap: 78027
				//
				//			No sorting, just iterator swap for begin() and the minimum element, which is basically
				//			the smallest partial_sort there is...
				//			aRobotSize = (37,29), radius = 23
				//			Duration: 2.626.220 openSet: 1507 closedSet: 76521 predecessorMap: 78027
				//
				//			// Sort the openSet before the next loop, use partial_sort as per Scott Meyers' Effective STL
				//			if(std::distance(openSet.begin(), openSet.end()) > 5 )
				//			{
				//				std::partial_sort( openSet.begin(), openSet.begin() + 4, openSet.end(), VertexLessCostCompare());
				//			}else
				//			{
				//				std::sort( openSet.begin(), openSet.end(), VertexLessCostCompare());
				//			}
				std::iter_swap(openSet.begin(), std::min_element( openSet.begin(), openSet.end(), VertexLessCostCompare()));
			}
		}
		// std::cerr << "Duration: " << (std::clock() - begin) << " openSet: " << getOS().size() << " closedSet: " << getCS().size() << " predecessorMap: " << getPM().size() << std::endl;

		std::cerr << "**** No route from " << aStart << " to " << aGoal << std::endl;
		return Path();
	}
	/**
	 *
	 */
	void AStar::addToOpenSet( const Vertex& aVertex)
	{
		std::unique_lock< std::recursive_mutex > lock( openSetMutex);
		openSet.push_back( aVertex);
		notifyObservers();
	}
	/**
	 *
	 */
	void AStar::removeFromOpenSet( const Vertex& aVertex)
	{
		std::unique_lock< std::recursive_mutex > lock( openSetMutex);
		OpenSet::iterator i = findInOpenSet( aVertex);
		removeFromOpenSet( i);
	}
	/**
	 *
	 */
	void AStar::removeFromOpenSet( OpenSet::iterator& i)
	{
		std::unique_lock< std::recursive_mutex > lock( openSetMutex);
		openSet.erase( i);
		notifyObservers();
	}
	/**
	 *
	 */
	OpenSet::iterator AStar::findInOpenSet( const Vertex& aVertex)
	{
		std::unique_lock< std::recursive_mutex > lock( openSetMutex);
		return std::find_if( openSet.begin(),
							 openSet.end(),
							 [aVertex](const Vertex& rhs)
							 {
			return aVertex.equalPoint(rhs);
							 });
	}
	/**
	 *
	 */
	bool AStar::findRemoveInOpenSet( const Vertex& aVertex)
	{
		std::lock_guard< std::recursive_mutex > lock( openSetMutex);
		OpenSet::iterator i = findInOpenSet( aVertex);
		if (i != openSet.end())
		{
			openSet.erase( i);
			return true;
		}
		return false;

	}
	/**
	 *
	 */
	void AStar::removeFirstFromOpenSet()
	{
		std::unique_lock< std::recursive_mutex > lock( openSetMutex);
		openSet.erase( openSet.begin());
	}
	/**
	 *
	 */
	void AStar::addToClosedSet( const Vertex& aVertex)
	{
		std::unique_lock< std::recursive_mutex > lock( closedSetMutex);
		closedSet.insert( aVertex);
		notifyObservers();
	}
	/**
	 *
	 */
	void AStar::removeFromClosedSet( const Vertex& aVertex)
	{
		std::unique_lock< std::recursive_mutex > lock( closedSetMutex);
		ClosedSet::iterator i = findInClosedSet( aVertex);
		removeFromClosedSet( i);
	}
	/**
	 *
	 */
	void AStar::removeFromClosedSet( ClosedSet::iterator& i)
	{
		std::unique_lock< std::recursive_mutex > lock( closedSetMutex);
		closedSet.erase( i);
		notifyObservers();
	}
	/**
	 *
	 */
	ClosedSet::iterator AStar::findInClosedSet( const Vertex& aVertex)
	{
		std::unique_lock< std::recursive_mutex > lock( closedSetMutex);
		//return std::find_if(closedSet.begin(),closedSet.end(),[aVertex](const Vertex& rhs){return aVertex.equalPoint(rhs);});
		return closedSet.find( aVertex);
	}
	/**
	 *
	 */
	ClosedSet AStar::getClosedSet() const
	{
		std::lock_guard< std::recursive_mutex > lock( closedSetMutex);
		ClosedSet c = getCS();
		return c;
	}
	/**
	 *
	 */
	bool AStar::findRemoveClosedSet( const Vertex& aVertex)
	{
		std::lock_guard< std::recursive_mutex > lock( closedSetMutex);
		ClosedSet::iterator i = findInClosedSet( aVertex);
		if (i != closedSet.end())
		{
			closedSet.erase( i);
			return true;
		}
		return false;
	}
	/**
	 *
	 */
	OpenSet AStar::getOpenSet() const
	{
		std::lock_guard< std::recursive_mutex > lock( openSetMutex);
		OpenSet o = getOS();
		return o;
	}
	/**
	 *
	 */
	VertexMap AStar::getPredecessorMap() const
	{
		std::lock_guard< std::recursive_mutex > lock( predecessorMapMutex);
		VertexMap p = getPM();
		return p;
	}

	/**
	 *
	 */
	ClosedSet& AStar::getCS()
	{
		std::unique_lock< std::recursive_mutex > lock( closedSetMutex);
		return closedSet;
	}
	/**
	 *
	 */
	const ClosedSet& AStar::getCS() const
	{
		std::unique_lock< std::recursive_mutex > lock( closedSetMutex);
		return closedSet;
	}
	/**
	 *
	 */
	OpenSet& AStar::getOS()
	{
		std::unique_lock< std::recursive_mutex > lock( openSetMutex);
		return openSet;
	}
	/**
	 *
	 */
	const OpenSet& AStar::getOS() const
	{
		std::unique_lock< std::recursive_mutex > lock( openSetMutex);
		return openSet;
	}
	/**
	 *
	 */
	VertexMap& AStar::getPM()
	{
		std::unique_lock< std::recursive_mutex > lock( predecessorMapMutex);
		return predecessorMap;
	}
	/**
	 *
	 */
	const VertexMap& AStar::getPM() const
	{
		std::unique_lock< std::recursive_mutex > lock( predecessorMapMutex);
		return predecessorMap;
	}
}// namespace PathAlgorithm
