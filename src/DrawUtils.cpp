/*
 * DrawUtils.cpp
 *
 *  Created on: Jul 14, 2012
 *      Author: onedayitwillmake
 */

#include "DrawUtils.h"
#include "cinder/gl/gl.h"

namespace zoa {

void DrawUtils::createPlaneWithTriMesh( ci::TriMesh& mesh, size_t width, size_t depth ) {

	mesh.clear();

	// create the vertices and texture coords
	for(size_t z = 0; z <= depth; ++z) {
		for(size_t x = 0; x <= width; ++x) {
			float floatX = static_cast<float>(x);
			float floatZ = static_cast<float>(z);

			mesh.appendVertex( ci::Vec3f( floatX, 0, floatZ ) );
			mesh.appendColorRGBA( ci::ColorA(1.0f, 1.0, 1.0, 1.0f ) );
			mesh.appendTexCoord( ci::Vec2f(floatX / width, floatZ / depth ) );
		}
	}

	// next, create the index buffer
	std::vector<uint32_t>	indices;
	//	indices of the 1st row will be:  0,  1,  2, ...,  9, 10
	//	indices of the 2nd row will be: 11, 12, 13, ..., 20, 21
	//	and so on.
	//
	//      therefor:
	//	the 1st triangle will be: 0 -> 11 -> 1
	//	the 2nd triangle will be: 1 -> 11 -> 12
	//	the 3rd triangle will be: 1 -> 12 -> 2
	//	the 4th triangle will be: 2 -> 12 -> 13
	//	and so on.
	for(size_t z=0;z<depth;++z) {

		size_t base = z * (width + 1);
		for(size_t x=0;x<width;++x) {
			indices.push_back( base + x );
			indices.push_back( base + x + width + 1 );
			indices.push_back( base + x + 1 );

			indices.push_back( base + x + 1 );
			indices.push_back( base + x + width + 1 );
			indices.push_back( base + x + width + 2 );
		}
	}
	mesh.appendIndices( &indices.front(), indices.size() );
}

void DrawUtils::calculateTrimeshNormals( ci::TriMesh& mesh ) {
	mesh.getNormals().clear();

	// Create the number of vertices we'll need
	for(size_t i=0; i < mesh.getVertices().size(); ++i)
		mesh.getNormals().push_back( ci::Vec3f::zero() );

	// For each triangle face, get the normal for it's 3 vertices
	// Notice we keep adding to the normal if it is connected to two faces
	for(size_t i=0; i < mesh.getNumTriangles(); ++i) {
		ci::Vec3f v0 = mesh.getVertices()[ mesh.getIndices()[i * 3] ];
		ci::Vec3f v1 = mesh.getVertices()[ mesh.getIndices()[i * 3 + 1] ];
		ci::Vec3f v2 = mesh.getVertices()[ mesh.getIndices()[i * 3 + 2] ];

		ci::Vec3f e0 = (v2 - v0).normalized();
		ci::Vec3f e1 = (v2 - v1).normalized();
		ci::Vec3f n = e0.cross(e1).normalized();

		mesh.getNormals()[ mesh.getIndices()[i * 3] ] += n;
		mesh.getNormals()[ mesh.getIndices()[i * 3 + 1] ] += n;
		mesh.getNormals()[ mesh.getIndices()[i * 3 + 2] ] += n;
	}

	// normalize the final result
	for(size_t i=0;i< mesh.getNormals().size();++i) {
		mesh.getNormals()[i].normalize();
	}
}

void DrawUtils::drawTrimeshNormals( ci::TriMesh& mesh ) {
	std::vector<ci::Vec3f> meshVertices = mesh.getVertices();
	std::vector<ci::Vec3f> meshNormals = mesh.getNormals();
	for( size_t i = 3; i < mesh.getNormals().size(); i+=4 ) {
		float t = 0.5f;
		float scale = 1;

		ci::Vec3f midPoint = ci::Vec3f( (1.0f - t) * ( meshVertices[i-2] ) + t * ( meshVertices[i] ) );
		ci::Vec3f normal = meshNormals[i]* scale;
		ci::gl::drawVector( midPoint, midPoint+normal, scale, scale/4.0f );
	}
}

} /* namespace zoa */
