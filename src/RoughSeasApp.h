#include "cinder/app/AppBasic.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/TriMesh.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/Camera.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Perlin.h"
#include "cinder/params/Params.h"
#include "Resources.h"

class RoughSeasApp : public ci::app::AppBasic {
public:
	void setup();
	void prepareSettings( ci::app::AppBasic::Settings *settings );
	void update();
	void draw();
	void shutdown();

	void mouseDown( ci::app::MouseEvent event );
	void mouseMove( ci::app::MouseEvent event );
	void mouseDrag( ci::app::MouseEvent event );
	void mouseUp( ci::app::MouseEvent event );
	void resize( ci::app::ResizeEvent event );

	void keyDown( ci::app::KeyEvent event );

	ci::TriMesh			mTriMesh;

	ci::CameraPersp		mCamera;
	ci::Vec3f			mEyePoint;

	ci::gl::GlslProg	mShader;

	ci::gl::VboMesh		mVboMesh;

	ci::Perlin			noise;
	ci::TriMesh			bird;

	//
	ci::params::InterfaceGl		mParams;
	ci::Vec3f					m_uLightDirection;
	ci::ColorA					m_uColor;
	float						m_uSpecularFactor;

	ci::Vec3f					m_uRotation;
	ci::Vec3f					m_uTranslation;

	float						m_perlinScale;
	bool						m_drawWireframe;

	float sphereSize;
	int	azimuth;
	int elevation;
};
