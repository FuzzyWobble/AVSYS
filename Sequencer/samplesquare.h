#ifndef sequencer__samplesquare_h
#define sequencer__samplesquare_h

#include "ofMain.h"

class samplesquare{
public:
    void setup(ofRectangle _rect);
    void draw();
    void mouseDown(int x, int y);

    ofRectangle rect;
    ofColor color;
    bool bPressed;
};

#endif
