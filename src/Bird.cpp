/*
 * .cpp
 *
 *  Created on: Aug 26, 2012
 *      Author: onedayitwillmake
 */

#include "Bird.h"

Bird::Bird() {
}

void Bird::setup() {
	// Make a bird
	geometry.appendVertex( ci::Vec3f( 5, 0, 0 ) );
	geometry.appendVertex( ci::Vec3f( -5, -2, 1 ) );
	geometry.appendVertex( ci::Vec3f( -5, 0, 0 ) );
	geometry.appendVertex( ci::Vec3f( -5, -2, -1 ) );

	geometry.appendVertex( ci::Vec3f( 0, 2, -6 ) );
	geometry.appendVertex( ci::Vec3f( 0, 2, 6 ) );
	geometry.appendVertex( ci::Vec3f( 2, 0, 0 ) );
	geometry.appendVertex( ci::Vec3f( -3, 0, 0 ) );

	geometry.appendTriangle(0,2,1);
	geometry.appendTriangle(4,7,6);
	geometry.appendTriangle(5,6,7);

	//zoa::DrawUtils::calculateTrimeshNormals( geometry );
}

Bird::~Bird() {
	// TODO Auto-generated destructor stub
}

