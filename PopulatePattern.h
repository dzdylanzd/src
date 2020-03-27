/*
 * PopulatePattern.h
 *
 *  Created on: 26 mrt. 2020
 *      Author: dylan zweers
 */

#ifndef SRC_POPULATEPATTERN_H_
#define SRC_POPULATEPATTERN_H_

class PopulatePattern
{
public:
	typedef enum{patternA =1 ,patternB = 2,patternC = 3} pattern;
	PopulatePattern(const unsigned short aRobotID);
	virtual ~PopulatePattern();
	void populate(const unsigned short pattern);
	void PatternA();
	void PatternB();
	void PatternC();

private:
	const unsigned short robotID;
};

#endif /* SRC_POPULATEPATTERN_H_ */
