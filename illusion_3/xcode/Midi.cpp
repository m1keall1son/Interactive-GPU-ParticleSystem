#include "Midi.h"

void MidiBirth::setup(float far, float close)
{
    initiateVariables();
    initiateBirthNotes();
    mFar=far;
    mClose=close;
    mParams.setOptions( "", "position='20 500'");
}

void MidiBirth::update(int avgDepth,bool showGUI)
{
    mInput=avgDepth;
    //playBirthNotes();
    checkMidiPort();
    mshowGUI=showGUI;
}

void MidiBirth::draw()
{
    if (mshowGUI) {
        mParams.draw();
    }
    
}
