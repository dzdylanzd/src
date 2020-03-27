#ifndef RECTANGLESHAPE_HPP_
#define RECTANGLESHAPE_HPP_

#include "Config.hpp"
#include <string>
#include <memory>
#include "Widgets.hpp"
#include "Shape.hpp"
#include "Size.hpp"

namespace View
{

	class RectangleShape;
	typedef std::shared_ptr<RectangleShape> RectangleShapePtr;


	class RectangleShape : public Shape
	{
		public:
			/**
			 *
			 */
			RectangleShape( const std::string& aTitle = "");
			/**
			 *
			 */
			RectangleShape( const Point& aCentrePoint,
							const std::string& aTitle = "",
							int aBorderWidth = 2,
							int aSpacing = 2);
			/**
			 *
			 */
			RectangleShape( Model::ModelObjectPtr aModelObject,
							const Point& aCentrePoint,
							const std::string& aTitle = "",
							int aBorderWidth = 2,
							int aSpacing = 2);
			/**
			 *
			 */
			~RectangleShape();
			/**
			 *
			 */
			RectangleShape& operator=( const RectangleShape& aRectangleShape);
			/**
			 * @name Observer functions
			 */
			//@{
			/**
			 * A Notifier will call this function if this Observer will handle the notifications of that
			 * Notifier. It is the responsibility of the Observer to filter any events it is interested in.
			 */
			virtual void handleNotification(){}
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
			//@}
			/**
			 *
			 * @return True if the point is on the border of the shape
			 */
			virtual bool isBorderPoint( const Point aPoint,
										int aRadius = 3) const;
			/**
			 *
			 */
			virtual Point getCentre() const;
			/**
			 *
			 */
			virtual void setCentre( const Point& aPoint);
			/**
			 *
			 */
			std::string getTitle() const;
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
				return "RED";
			}
			/**
			 *
			 */
			virtual std::string getActivationColour() const
			{
				return "BLUE";
			}
			;
			/**
			 *
			 */
			void setTitle( const std::string& aTitle);
			/**
			 *
			 */
			virtual Size getSize() const;
			/**
			 *
			 */
			virtual void setSize( const Size& aSize);
			/**
			 *
			 */
			int getBorderWidth() const;
			/**
			 *
			 */
			void setBorderWidth( int aBorderWidth);
			/**
			 *
			 */
			int getSpacing() const;
			/**
			 *
			 */
			void setSpacing( int aSpacing);
			/**
			 *
			 */
			virtual void handleActivated();
			/**
			 *
			 */
			virtual void handleSelection();
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
		protected:
			Point centre;
			Size size;
			std::string title;
			Size titleSize;
			int borderWidth;
			int spacing;

	};
	//	class RectangleShape
} // namespace View
#endif // RECTANGLESHAPE_HPP_
