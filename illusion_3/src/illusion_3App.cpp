#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Particlewall.h"
#include "cinder/qtime/QuickTime.h"
#include "MidiOut.h"
#include "MidiMessage.h"
#include "MidiConstants.h"
#include "cinder/params/Params.h"
#include "cinder/Utilities.h"
#include "cinder/CinderMath.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Illusion_2App : public AppNative {
public:
	void setup();
	void keyDown( KeyEvent event );
    void mouseUp( MouseEvent event );
    void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void prepareSettings( Settings *settings );
	void update();
    void initMidi( float far, float close );
	void draw();
    
    Particlewall particlewall;
    gl::GlslProgRef   updateShader, drawShader;
    bool mDebug;
    
    midi::MidiOut mMidiOut;
    int mPort, mPrePort;
    string mPortName;
    int mChannel;
    
    params::InterfaceGl mParams;
    float mNear, mFar, mInput;
    
};

void Illusion_2App::prepareSettings( Settings *settings ){
    settings->setWindowSize( 800, 800 );
}

void Illusion_2App::initMidi( float far,  float close ){
    
    mParams = params::InterfaceGl( "Music Settings & Info", Vec2i( 500, 150 ) );
    
    
    //        mParams.addParam( "Count", &mNoteCounter );
    //        mParams.addParam( "Tempo (millis)", &mTempo );
    mParams.addParam( "MIDI Port Name", &mPortName,mPortName,true);
    mParams.addParam( "To change port, change port number here", &mPort);
    mParams.addSeparator();
    mParams.addParam( "Number sent from kinect", &mInput, "min=0 max=255 step=1" );
    mParams.addSeparator();
    mParams.addParam( "Music threshold, set this to the number sent from kinect", &mFar);
    mParams.addParam( "Bottom range of thres., 0 is a good number to set it to ", &mNear);
    mMidiOut.getPortList();
    mPort=0;
    mMidiOut.openPort(mPort);
    mPortName=mMidiOut.getName();
    mPort=mMidiOut.getPort();
    mPrePort=mPort;
    mInput=0;
    
}

void Illusion_2App::setup()
{
    mDebug = true;
    initMidi( 65, 40 );
       
    particlewall = Particlewall();
    particlewall.setup();
}

void Illusion_2App::mouseDown( MouseEvent event )
{
    if(mDebug)
        particlewall.kinect.mouseDown(event.getPos());
}
void Illusion_2App::mouseDrag( MouseEvent event )
{
    if(mDebug)
        particlewall.kinect.mouseDrag(event.getPos());
    
}
void Illusion_2App::mouseUp( MouseEvent event )
{
    if(mDebug)
        particlewall.kinect.mouseUp(event.getPos());
}



void Illusion_2App::keyDown( KeyEvent event )
{
    if(event.getChar() == '1') particlewall.thresholdDOWN();
    if(event.getChar() == '2') particlewall.thresholdUP();
    if(event.getChar() == 'd') {
        particlewall.setDebug();
        mDebug = !mDebug;
    }
}

void Illusion_2App::update()
{
    particlewall.update();
    mMidiOut.sendControlChange(1, 1, particlewall.mCC1);
    mMidiOut.sendControlChange(1, 2, particlewall.mCC2);
    mMidiOut.sendControlChange(1, 3, particlewall.mCC3);
}

void Illusion_2App::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
    particlewall.draw();
}

CINDER_APP_NATIVE( Illusion_2App, RendererGl )
