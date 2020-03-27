#ifndef WAYPOINTSHAPE_HPP_
#define WAYPOINTSHAPE_HPP_

#include "Config.hpp"
#include "RectangleShape.hpp"
#include "WayPoint.hpp"

namespace View
{
	/**
	 *
	 */
	class WayPointShape : public RectangleShape
	{
		public:
			/**
			 *
			 */
			WayPointShape( Model::WayPointPtr aWayPoint);
			/**
			 *
			 */
			virtual ~WayPointShape();
			/**
			 * @name Type safe accessors and mutators
			 */
			//@{
			/**
			 * Type safe accessor
			 */
			Model::WayPointPtr getWayPoint() const;
			/**
			 * Type safe mutator
			 */
			void setWayPoint( Model::WayPointPtr aWayPoint);
			//@}
			/**
			 *
			 */
			virtual std::string getNormalColour() const
			{
				return "BLUE";
			}
			/**
			 *
			 */
			virtual std::string getSelectionColour() const
			{
				return "BLUE";
			}
			/**
			 *
			 */
			virtual std::string getActivationColour() const
			{
				return "BLACK";
			}
			;
			/**
			 * @name Observer functions
			 */
			//@{
			/**
			 * A Notifier will call this function if this Observer will handle the notifications of that
			 * Notifier. It is the responsibility of the Observer to filter any events it is interested in.
			 */
			virtual void handleNotification();
			//@}
			/**
			 * @name Pure virtual abstract Shape functions
			 */
			//@{
			/**
			 *
			 */
			virtual void draw( wxDC& dc);
			/**
			 *
			 */
			virtual void setCentre( const Point& aPoint);
			//@}
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

		private:
	};
} // namespace View
#endif // WAYPOINTSHAPE_HPP_
