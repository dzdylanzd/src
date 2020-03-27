#include "MathUtils.hpp"

namespace Utils
{
	/**
	 *
	 */
	/* static */ double MathUtils::toRadians( double aDegrees)
	{
		return aDegrees * PI / 180.0;
	}
	/**
	 *
	 */
	/* static */ double MathUtils::toDegrees( double aRadian)
	{
		return aRadian * 180.0 / PI;
	}
} //namespace Utils
