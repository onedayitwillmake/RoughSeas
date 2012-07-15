/*
 * DrawUtils.h
 *
 *  Created on: Jul 14, 2012
 *      Author: onedayitwillmake
 */

#ifndef DRAWUTILS_H_
#define DRAWUTILS_H_

#include "cinder/TriMesh.h"

namespace zoa {

class DrawUtils {
public:
	// Creates a plane of 'width' with 'sW', 'sH' segements at 'offset' and places geometry into 'mesh'
	static void createPlaneWithTriMesh( ci::TriMesh& mesh, size_t width, size_t depth );
	static void calculateTrimeshNormals( ci::TriMesh& mesh );
	static void drawTrimeshNormals( ci::TriMesh& mesh );
};

} /* namespace zoa */
#endif /* DRAWUTILS_H_ */
