/*
 * Bird.h
 *
 *  Created on: Aug 26, 2012
 *      Author: onedayitwillmake
 */

#ifndef BIRD_H_
#define BIRD_H_

#include "cinder/TriMesh.h"
#include "cinder/Vector.h"


class Bird {
public:
	Bird();
	virtual ~Bird();

	void	setup();

	///// ACCESSORS
	const ci::TriMesh& getGeometry() { return this->geometry; };
	const ci::Vec3f& getRotation() { return this->m_uRotation; };
	const ci::Vec3f& getTranslation() { return this->m_uTranslation; };
private:
	ci::TriMesh					geometry;
	ci::Vec3f					m_uRotation;
	ci::Vec3f					m_uTranslation;
};

#endif /* BIRD_H_ */
