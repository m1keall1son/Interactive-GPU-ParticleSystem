#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "CinderFreenect.h"
#include "cinder/Rand.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/Text.h"
#include "cinder/Camera.h"
#include "KinectMask.h"
#include "Midi.h"
#include "cinder/gl/GlslProg.h"

#include "Resources.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class Particlewall {
public:
    
    Particlewall();
    void setup();
    void update();
    void draw();
    void reset();
    void setDebug();
    void thresholdUP();
    void thresholdDOWN();
    void createVbo();
    void loadMovie();
    
    static const int  VERTICES_X = 800, VERTICES_Y = 800;
    static const int  FBO_WIDTH = 800, FBO_HEIGHT = 800;
    
    gl::GlslProgRef   drawShader,updateShader;
    Surface           kinectPix;
    KinectMask        kinect;
    bool              debug;
    gl::VboMeshRef    mesh;
    gl::Fbo           ping, pong, videoFbo, kinectFbo;
    float             threshold;
    Surface32f        img;
    gl::TextureRef    kinectImg;
    gl::Texture       tex;
	qtime::MovieGl    video;
    CameraPersp       cam;
    float             mWTFoffset;
    float             iterThresh;
    int               mCC1, mCC2, mCC3;

    
    
};