//
//  KinectMask.cpp
//  kinectResizeDebug
//
//  Created by Mike Allison on 7/20/13.
//
//

#include "KinectMask.h"

KinectMask::KinectMask(){
    
}

void KinectMask::mouseDown( Vec2i event){
    if(mMakeSquare){
        mStart.x = event.x;
        mStart.y = 0;
        mEnd.x = mStart.x + 480;
        mEnd.y = 480;
    }else {
        mStart = event;
    }
    
}

void KinectMask::mouseDrag( Vec2i event){
    if(!mMakeSquare){
        mDragging = true;
        mEnd = event;
        mCursor = event;
    }else{
        mStart.x = event.x;
        mStart.y = 0;
        mEnd.x = mStart.x + 480;
        mEnd.y = 480;
    }
}

void KinectMask::mouseUp( Vec2i event){
    if(!mMakeSquare){
        mDragging = false;
        mEnd = event;
    }
}

void KinectMask::keyDown( char c ){
    if( c == 's' )mMakeSquare = !mMakeSquare;
}

void KinectMask::resetBounds(){
    
    if(mKinect->checkNewDepthFrame()){
        
        
    }
    
}

Surface KinectMask::getSurface(){
        return mResetPix;
   }

gl::Texture KinectMask::getTex(){
        return mResizeTex;

}


void KinectMask::setup()
{
    glEnable(GL_TEXTURE_2D);
    try{
        mKinect = Kinect::create();
        console() << "There are " << Kinect::getNumDevices() << " Kinects connected." << endl;
    }catch (Kinect::Exc e){
        cout << "shit didn't load " << endl;
    }
    //    mCam.setPerspective(60.f, getWindowAspectRatio(), 1.f, 1500.f);
    //    mCam.lookAt(Vec3f(getWindowWidth()/4, getWindowHeight()/4, 500.f), Vec3f(getWindowWidth()/4, getWindowHeight()/4, 0.f), Vec3f::yAxis());
    //    gl::setMatrices( mCam );
    mMakeSquare = true;
    
    mResetPix = Surface(240,240,SurfaceChannelOrder::RGBA);
    mResizeTex = gl::Texture(240,240);
    
    mStart = Vec2f(0,0);
    mEnd = Vec2f(480,480);
    
}


void KinectMask::update()
{
    if(mKinect->checkNewDepthFrame()){
        mTex = mKinect->getDepthImage();
        
        Area curSelection = Area( mStart, mEnd );
        if( curSelection.getWidth() > 20 && curSelection.getHeight() > 20 ){
            gl::Fbo::Format format;
            format.setColorInternalFormat(GL_RGBA32F_ARB);
            format.setSamples( 4 );
            //  mResizeFbo = gl::Fbo(800,800,format);
            mResizeFbo = gl::Fbo(curSelection.getWidth()/2, curSelection.getHeight()/2,format);
            mResizeFbo.bindFramebuffer();
            gl::setMatricesWindow( mResizeFbo.getSize(), false ); // false to prevent vertical flipping
            gl::setViewport(mResizeFbo.getBounds());
            gl::clear();
            gl::color(1.f,1.f,1.f);
            gl::draw( mTex , curSelection, Rectf(Vec2i(0,0), Vec2i(curSelection.getWidth()/2,curSelection.getHeight()/2)));
            //  gl::draw( mTex , curSelection, Rectf(Vec2i(0,0), Vec2i(800,800)));
            mResizeFbo.unbindFramebuffer();
            mResetPix = Surface(mResizeFbo.getTexture());
            mResizeTex = mResizeFbo.getTexture();
        }
        
    }
    
}

void KinectMask::draw()
{
	// clear out the window with black
    
    gl::color(1.f,1.f,1.f);
    
    if(mTex)
        gl::draw( mTex );
    
    if(mResetPix)
        gl::draw( mResetPix , Vec2i(0, mTex.getHeight()+10) );
    
    Rectf debugArea;
    debugArea = Rectf( Area( mStart, mEnd ));
    gl::color(1.0f,0.f,0.f);
    gl::drawStrokedRect( debugArea );
    gl::color(1.f,1.f,1.f);

}
