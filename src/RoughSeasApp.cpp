#include "RoughSeasApp.h"

#include <iostream>
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include "cinder/Matrix44.h"
#include "cinder/Perlin.h"

#include "DrawUtils.h"

void RoughSeasApp::prepareSettings( ci::app::AppBasic::Settings *settings ) {
	settings->setWindowSize( 1280, 720 );
}

void RoughSeasApp::setup() {
	// Hack to force our app window to show up above other windows on debug launch
	setAlwaysOnTop( true );

	ci::Perlin noise;
	zoa::DrawUtils::createPlaneWithTriMesh( mTriMesh, 10, 10 );

	///// CAMERA
	elevation = 0;
	azimuth = 0;
	sphereSize = 1;
	mEyePoint = ci::Vec3f(85.0f, 10.0f, 100.0f );
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

	// Make a bird
	bird.appendVertex( ci::Vec3f( 5, 0, 0 ) );
	bird.appendVertex( ci::Vec3f( -5, -2, 1 ) );
	bird.appendVertex( ci::Vec3f( -5, 0, 0 ) );
	bird.appendVertex( ci::Vec3f( -5, -2, -1 ) );

	bird.appendVertex( ci::Vec3f( 0, 2, -6 ) );
	bird.appendVertex( ci::Vec3f( 0, 2, 6 ) );
	bird.appendVertex( ci::Vec3f( 2, 0, 0 ) );
	bird.appendVertex( ci::Vec3f( -3, 0, 0 ) );

	bird.appendTriangle(0,2,1);
	bird.appendTriangle(4,7,6);
	bird.appendTriangle(5,6,7);

	zoa::DrawUtils::calculateTrimeshNormals( bird );


	// Params
	m_uLightDirection = ci::Vec3f(0.00, -1, 0 );
	m_uColor = ci::ColorAf(255, 255, 255);
	m_uRotation = ci::Vec3f(1.0, 0.0, 0.0 );
	m_uTranslation = ci::Vec3f(0.0f, 0.0, 0.0f);

	m_perlinScale = 11.0f;
	m_uSpecularFactor = 6.0;
	m_drawWireframe = false;

	mParams = ci::params::InterfaceGl( "App parameters", ci::Vec2i( 200, 400 ) );
	mParams.addParam( "Color", &m_uColor, "min=0 max=1 step=0.05" );
	mParams.addParam( "Rotation", &m_uRotation);
	mParams.addParam( "Position", &m_uTranslation, "min=0 max=100 step=1"	);


	mParams.addParam( "Light Direction", &m_uLightDirection, "min=0.0 max=1 step=0.05" );
	mParams.addParam( "Shininess", &m_uSpecularFactor, "min=1 max=255 step=1" );
	mParams.addParam( "Perlin Scale", &m_perlinScale, "min=1 max=255 step=1" );
	mParams.addParam( "Draw Wireframe", &m_drawWireframe );
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

	int amountToMove = 5;
	switch( event.getCode() ) {
		case KeyEvent::KEY_LEFT:	azimuth -= amountToMove; break;
		case KeyEvent::KEY_RIGHT:	azimuth += amountToMove; break;
		case KeyEvent::KEY_UP:
			if( event.isShiftDown( ) ) sphereSize += 1;
			else elevation+=amountToMove;
		break;
		case KeyEvent::KEY_DOWN:
			if( event.isShiftDown( ) ) sphereSize -= 1;
			else elevation -= amountToMove;
		break;
	}

	azimuth %= 360;
	elevation %= 360;
	float theta = elevation * M_PI/180.0f;
	float phi = azimuth * M_PI/180.0f;

	// Convert spehrical cordinate to cartesian coordiantes
	mEyePoint.set(
			cosf((float)theta) * sinf((float)phi) * sphereSize,
			sinf((float)theta) * sphereSize,
			cosf((float)theta) * -cosf((float)phi) * sphereSize);
	std::cout << mCamera.getEyePoint() << std::endl;
}

// All logic updates here
// Called every frame
void RoughSeasApp::update() {
	// This is a work hacky work around, to force our application to be above other windows when launched
	static bool hasBecomeFirstResponder = false; if( !hasBecomeFirstResponder && getElapsedSeconds() > 2 ) { hasBecomeFirstResponder = true; setAlwaysOnTop( false ); }

	// For each triangle face, get the normal for it's 3 vertices
	// Notice we keep adding to the normal if it is connected to two faces
	for(size_t i=0; i < mTriMesh.getNumTriangles(); ++i) {
		ci::Vec3f& v0 = mTriMesh.getVertices()[ mTriMesh.getIndices()[i * 3] ];
		ci::Vec3f& v1 = mTriMesh.getVertices()[ mTriMesh.getIndices()[i * 3 + 1] ];
		ci::Vec3f& v2 = mTriMesh.getVertices()[ mTriMesh.getIndices()[i * 3 + 2] ];

		float scaleFactor = 0.05;
		float yValue = noise.fBm( v0.x*scaleFactor + getElapsedFrames() * scaleFactor * scaleFactor, v0.z*scaleFactor,  getElapsedFrames() * scaleFactor * scaleFactor ) * m_perlinScale;
		v0.y = yValue;
		v1.y = yValue;//noise.fBm( v1.x*scaleFactor, v1.z*scaleFactor, getElapsedFrames() * scaleFactor * scaleFactor ) * 3;
		v2.y = yValue;//noise.fBm( v2.x*scaleFactor, v2.z*scaleFactor, getElapsedFrames() * scaleFactor * scaleFactor ) * 3;
	}
	zoa::DrawUtils::calculateTrimeshNormals( mTriMesh );

	mCamera.lookAt( mEyePoint, ci::Vec3f::zero() );
	ci::gl::setMatrices( mCamera );
}

// Only drawing here, do not place logic during drawing or it will slow down OpenGL
// Called every frame
void RoughSeasApp::draw() {
	// Clear screen every frame with black
	ci::gl::clear( ci::Color::white() );

	// Set the color and draw a simple line to the mouse
	ci::gl::color( ci::ColorA::white() );

	ci::gl::enableDepthRead();
	ci::gl::enableDepthWrite();
	ci::gl::enableAlphaBlending();

	// Object props
	ci::Vec3f rotation = m_uRotation;
	rotation.safeNormalize();
	rotation *= M_PI*2;



	// Camera properties
	mShader.bind();
	mShader.uniform( "uTranslation", m_uTranslation );
	mShader.uniform( "uRotation", rotation );
	mShader.uniform( "uMVMatrix", mCamera.getModelViewMatrix() );
	mShader.uniform( "uPMatrix", mCamera.getProjectionMatrix() );

	ci::Matrix44f mat( mCamera.getModelViewMatrix() );
	mat.invert();
	mat.transpose();
	mShader.uniform( "uNMatrix", mat );

	// Light properties
	mShader.uniform("uLightDirection",  m_uLightDirection );
	mShader.uniform("uLightAmbient", ci::ColorA(0.1f, 0.1f, 0.1f, 1.0f ) );
	mShader.uniform("uLightDiffuse", ci::ColorA::white() );
	mShader.uniform("uLightSpecular", ci::ColorA::white() );

	// Material
	ci::Vec3f endColor = ci::Vec3f(m_uColor.r, m_uColor.g, m_uColor.b );
	endColor.safeNormalize();

	mShader.uniform("uMaterialAmbient", ci::ColorA(0.0f, 0.0f, 0.0f, 1.0f ) );
	mShader.uniform("uMaterialDiffuse", ci::ColorA( endColor.x, endColor.y, endColor.z, 1.0f ) );
	mShader.uniform("uMaterialSpecular", ci::ColorA::white() );
	mShader.uniform("uShininess", m_uSpecularFactor );

	ci::gl::draw( mTriMesh );
	//ci::gl::draw( bird );;;
//	ci::gl::drawCube( ci::Vec3f(0.0f,0.0f,0.0f), ci::Vec3f(5.0f, 5.0f, 5.0f) );
	mShader.unbind();

	if( m_drawWireframe ) {
		ci::gl::color( ci::ColorA(0.5, 0.5, 0.5, 0.5) );
		ci::gl::enableWireframe();
		ci::gl::draw( mTriMesh );
	//	ci::gl::draw( bird );
		ci::gl::disableWireframe();
	}

	ci::params::InterfaceGl::draw();
}

void RoughSeasApp::shutdown() {
	std::cout << "Shutdown..." << std::endl;
	AppBasic::shutdown();
}

///// NICE COLORS
// ci::ColorA( 0.5f, 0.8f, 0.1f, 1.0f ) - soft green


CINDER_APP_BASIC( RoughSeasApp, ci::app::RendererGl )
