#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "CinderFreenect.h"
#include "cinder/Xml.h"
#include "cinder/Camera.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Fbo.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class KinectMask {
public:
    
    KinectMask();
    void setup();
	void mouseDown( Vec2i event );
    void mouseDrag( Vec2i event );
    void mouseUp( Vec2i event );
    void keyDown( char c );
    void resetBounds();
	void update();
	void draw();
    Surface getSurface();
    gl::Texture getTex();
    
    KinectRef   mKinect;
    gl::Texture mTex;
    Surface     mResetPix;
    CameraPersp mCam;
    Vec2i       mCursor, mStart, mEnd;
    bool        mDragging;
    bool        mMakeSquare;
    gl::Fbo     mResizeFbo;
    gl::Texture mResizeTex;
    
};