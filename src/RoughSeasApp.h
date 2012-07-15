#include "cinder/app/AppBasic.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/TriMesh.h"
#include "cinder/Vector.h"
#include "cinder/Camera.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
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

	ci::gl::VboMesh mVboMesh;
};
