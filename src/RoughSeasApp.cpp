#include "RoughSeasApp.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include "cinder/Matrix44.h"
#include "cinder/Perlin.h"
#include <iostream>

void RoughSeasApp::prepareSettings( ci::app::AppBasic::Settings *settings ) {
	settings->setWindowSize( 1280, 720 );
}

void RoughSeasApp::setup() {
	// Hack to force our app window to show up above other windows on debug launch
	setAlwaysOnTop( true );

	size_t width = 100;
	size_t depth = 100;

	// clear the mesh (assumes a member variable of type ci::TriMesh)
	mTriMesh.clear();

	ci::Perlin noise;
	noise.setSeed( ci::Rand::randInt() );

	// create the vertices and texture coords
	for(size_t z=0;z<=depth;++z) {
		for(size_t x=0;x<=width;++x) {
			mTriMesh.appendVertex( ci::Vec3f( static_cast<float>(x), noise.noise(x*0.5f, z*0.5f) , static_cast<float>(z) ) );
			mTriMesh.appendColorRGBA( ci::ColorA( ci::CM_HSV, fabsf( noise.noise(x*0.01f, z*0.01f) ), 0.8f, 1.0f ) );
			mTriMesh.appendTexCoord( ci::Vec2f( static_cast<float>(x) / width, static_cast<float>(z) / depth ) );
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

	mTriMesh.appendIndices( &indices.front(), indices.size() );
//	void TriMesh::generateNormals(void)
//	{
		mTriMesh.getNormals().clear();

		for(size_t i=0; i < mTriMesh.getVertices().size(); ++i)
			mTriMesh.getNormals().push_back( ci::Vec3f::zero() );

	    for(size_t i=0; i < mTriMesh.getNumTriangles(); ++i) {
			ci::Vec3f v0 = mTriMesh.getVertices()[ mTriMesh.getIndices()[i * 3] ];
			ci::Vec3f v1 = mTriMesh.getVertices()[ mTriMesh.getIndices()[i * 3 + 1] ];
			ci::Vec3f v2 = mTriMesh.getVertices()[ mTriMesh.getIndices()[i * 3 + 2] ];

			ci::Vec3f e0 = (v2 - v0).normalized();
			ci::Vec3f e1 = (v2 - v1).normalized();
			ci::Vec3f n = e0.cross(e1).normalized();

			mTriMesh.getNormals()[ mTriMesh.getIndices()[i * 3] ] += n;
			mTriMesh.getNormals()[ mTriMesh.getIndices()[i * 3 + 1] ] += n;
			mTriMesh.getNormals()[ mTriMesh.getIndices()[i * 3 + 2] ] += n;
		}

		for(size_t i=0;i< mTriMesh.getNormals().size();++i) {
			mTriMesh.getNormals()[i].normalize();
			std::cout << mTriMesh.getNormals()[i] << std::endl;
		}
//	}




	///// CAMERA
	mEyePoint = ci::Vec3f(0.0f, 50.0f, 0.0f );
	mCamera.setPerspective( 65.0f, this->getWindowAspectRatio(), 5.0f, 3000.0f );
	mCamera.setWorldUp( ci::Vec3f::yAxis() );

	///// SHADER
	try {
		mShader = ci::gl::GlslProg( loadResource( "light.vs"), loadResource("light.fs") );
	} catch ( ci::gl::GlslProgCompileExc &exc ) {
		std::cout << "Shader compile error: " << exc.what() << std::endl;
		quit();
	} catch ( ... ) {
		std::cout << "Unable to load shader" << std::endl;
		quit();
	}
}


void RoughSeasApp::mouseDown( ci::app::MouseEvent event ) {
}

void RoughSeasApp::mouseDrag( ci::app::MouseEvent event ) {
}

void RoughSeasApp::mouseMove( ci::app::MouseEvent event ) {
}

void RoughSeasApp::mouseUp( ci::app::MouseEvent event ) {
}

void RoughSeasApp::resize( ci::app::ResizeEvent event ) {
	ci::gl::setMatricesWindow( event.getSize() );
}

void RoughSeasApp::keyDown( ci::app::KeyEvent event ) {
	using ci::app::KeyEvent;

	if( event.getChar() == KeyEvent::KEY_q ) quit();

	float amountToMove = 5.0f;
	switch( event.getCode() ) {
		case KeyEvent::KEY_LEFT:	mEyePoint.x -= amountToMove; break;
		case KeyEvent::KEY_RIGHT:	mEyePoint.x += amountToMove; break;
		case KeyEvent::KEY_UP:		mEyePoint.z -= amountToMove; break;
		case KeyEvent::KEY_DOWN:	mEyePoint.z += amountToMove; break;
	}
}

// All logic updates here
// Called every frame
void RoughSeasApp::update() {
	// This is a work hacky work around, to force our application to be above other windows when launched
	static bool hasBecomeFirstResponder = false; if( !hasBecomeFirstResponder && getElapsedSeconds() > 2 ) { hasBecomeFirstResponder = true; setAlwaysOnTop( false ); }

	mCamera.lookAt( mEyePoint, ci::Vec3f(0.0f, 0.0f, 0.0f) );
	ci::gl::setMatrices( mCamera );
}

// Only drawing here, do not place logic during drawing or it will slow down OpenGL
// Called every frame
void RoughSeasApp::draw() {
	// Clear screen every frame with black
	ci::gl::clear( ci::Color( 0, 0, 0 ) );

	// Set the color and draw a simple line to the mouse
	ci::gl::color( ci::ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );


//	ci::gl::enableWireframe();
	ci::gl::disableWireframe();

	// Camera properties
	mShader.bind();
	mShader.uniform( "uMVMatrix", mCamera.getModelViewMatrix() );
	mShader.uniform( "uPMatrix", mCamera.getProjectionMatrix() );

	ci::Matrix44f mat( mCamera.getModelViewMatrix() );
	mat.invert();
	mat.transpose();

	mShader.uniform( "uNMatrix", mat );


	ci::Vec3f lightDirection( this->getMousePos(), 0.0f );
	std::cout << lightDirection << std::endl;

	// Light properties
	mShader.uniform("uLightDirection",  lightDirection);
	mShader.uniform("uLightDiffuse", ci::ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
	mShader.uniform("uMaterialDiffuse", ci::ColorA( 0.5f, 0.8f, 0.1f, 1.0f ) );

	ci::gl::draw( mTriMesh );
	mShader.unbind();

//	ci::gl::color( ci::ColorA( 1.0f, 1.0f, 1.0f, 0.25 ) );
//	ci::gl::enableWireframe();
//	ci::gl::draw( mTriMesh );
}

void RoughSeasApp::shutdown() {
	std::cout << "Shutdown..." << std::endl;
	AppBasic::shutdown();
}


CINDER_APP_BASIC( RoughSeasApp, ci::app::RendererGl )
