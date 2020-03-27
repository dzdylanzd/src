#ifndef ROBOTSHAPE_HPP_
#define ROBOTSHAPE_HPP_

#include "Config.hpp"

#include <string>

#include "RectangleShape.hpp"
#include "Robot.hpp"
#include "Widgets.hpp"

namespace View
{
	class RobotWorldCanvas;

	/**
	 *
	 */
	class RobotShape :	public RectangleShape
	{
		public:
			/**
			 *
			 */
			RobotShape( Model::RobotPtr aRobot);
			/**
			 *
			 */
			virtual ~RobotShape();
			/**
			 * @name Type safe accessors and mutators
			 */
			//@{
			/**
			 * Type safe accessor
			 */
			Model::RobotPtr getRobot() const;
			/**
			 * Type safe mutator
			 */
			void setRobot( Model::RobotPtr aRobot);
			//@}
			/**
			 *
			 */
			virtual std::string getNormalColour() const
			{
				return "BLACK";
			}
			/**
			 *
			 */
			virtual std::string getSelectionColour() const
			{
				return "GREY";
			}
			/**
			 *
			 */
			virtual std::string getActivationColour() const
			{
				return "LIGHT GREY";
			}
			/**
			 * This function is called by the RobotWorldCanvas if enableActivation is set.
			 */
			virtual void handleActivated();
			/**
			 * This function is called by the RobotWorldCanvas if enableSelection is set.
			 */
			virtual void handleSelection();
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
			 * @param aPoint
			 * @return True if the point is in the shape
			 */
			virtual bool occupies( const Point& aPoint) const;
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

			void setRobotWorldCanvas(RobotWorldCanvas* aRobotWorldCanvas)
			{
				robotWorldCanvas = aRobotWorldCanvas;
			}
		protected:

		private:
			RobotWorldCanvas* robotWorldCanvas;
	};
} // namespace View
#endif // ROBOTSHAPE_HPP_
