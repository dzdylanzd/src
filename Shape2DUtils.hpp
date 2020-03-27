#ifndef SHAPE2DUTILS_HPP_
#define SHAPE2DUTILS_HPP_

#include "Config.hpp"
#include <string>

#include "BoundedVector.hpp"
#include "MathUtils.hpp"
#include "Point.hpp"
#include "Size.hpp"

namespace Utils
{
	/**
	 *
	 */
	class Shape2DUtils
	{
		public:
			/**
			 *
			 */
			static double getAngle( const Model::BoundedVector& aVector);
			/**
			 *
			 */
			static double getAngle( const Point& aStartpoint,
									const Point& anEndPoint);
			/**
			 *
			 */
			static bool intersect(	const Point& aStartLine1,
									const Point& aEndLine1,
									const Point& aStartLine2,
									const Point& anEndLine2);
			/**
			 *
			 */
			static Point getIntersection(	const Point& aStartLine1,
											const Point& aEndLine1,
											const Point& aStartLine2,
											const Point& anEndLine2);
			/**
			 *
			 * @param aPolygon The array of points
			 * @param numberOfPoints The number of points in the array
			 * @param p The point for which to test
			 * @return True if the point is inside the polygon, false otherwise
			 */
			static bool isInsidePolygon( 	Point* aPolygon,
											int aNumberOfPoints,
											const Point& aPoint);
			/**
			 *
			 * @param aStartpoint
			 * @param anEndPoint
			 * @param aPoint
			 * @param aRadius The number of pixels we can be wrong
			 * @return True if aPoint is within aRadius of the given line
			 */
			static bool isOnLine(	const Point& aStartPoint,
									const Point& anEndPoint,
									const Point& aPoint,
									int aRadius = 6);

			/**
			 *
			 * @param aPolygon The array of points that form a path
			 * @param numberOfPoints The number of points in the array
			 * @param aPoint
			 * @param aRadius The number of pixels we can be wrong
			 * @param aClosedShape True if the path is a closed, false otherwise
			 * @return True if aPoint is within aRadius of the given path lines
			 */
			static bool isOnLine( 	Point* aPolygon,
									int aNumberOfPoints,
									const Point& aPoint,
									int aRadius = 6,
									bool aClosedShape = true);

			/**
			 * The rotation will be done around (0.0)
			 *
			 * @param aPoint
			 * @param anAngle
			 * @return The rotated point
			 */
			static Point rotate(	const Point& aPoint,
									double anAngle);
			/**
			 * Enum to find the point of a rectangle
			 */
			enum CompassPoint
			{
				CP_NORTH,
				CP_NORTHEAST,
				CP_EAST,
				CP_SOUTHEAST,
				CP_SOUTH,
				CP_SOUTHWEST,
				CP_WEST,
				CP_NORTHWEST
			};
			/**
			 *
			 * @param aPoint A Point on the border of the shape
			 * @return
			 */
			static CompassPoint getCompassPoint(	const Point& aPoint,
													const Size& aSize,
													const Point& aBorderPoint,
													int aRadius = 6);
			/**
			 *
			 * @param aBorderPoint
			 * @param aCompassPoint
			 * @param aRadius
			 * @return
			 */
			static bool isCompassPoint( const Point& aPoint,
										const Size& aSize,
										const Point& aBorderPoint,
										CompassPoint aCompassPoint,
										int aRadius = 6);

			/**
			 *
			 */
			static std::string asString( const Point& aPoint);
			/**
			 *
			 */
			static std::string asString( const Size& aSize);

		protected:
			static double rotateX(	const Point& aPoint,
									double anAngle);
			static double rotateY(	const Point& aPoint,
									double anAngle);
			static double rotateX( 	int anX,
									int anY,
									double anAngle);
			static double rotateY( 	int anX,
									int anY,
									double anAngle);

	};
	//	class Shape2DUtils
} // namespace Utils

#endif // SHAPE2DUTILS_HPP_
