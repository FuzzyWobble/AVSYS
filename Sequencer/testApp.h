#pragma once

#include "ofMain.h"
#include "samplesquare.h"

//CHANGE the x,y to whatever you want, the program is SCALABLE
#define GRIDX 16
#define GRIDY 10
#define NUMSAMPLES 68

class testApp : public ofBaseApp{

	public:

        //standard
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

        // metronome stuff
        float atimer;
        float intervalTime;
        int nTicks;
        bool bTick;
        float bpm;
        int spacing;

        //holds rectanges for sample select mode
        ofRectangle sampleList[NUMSAMPLES];
        bool sampleListHover[NUMSAMPLES];

        //2D matrix of rectangles for sequencer mode
        //see samplesquare.h and samplesquare.cpp
        samplesquare ssArray[GRIDY][GRIDX]; //draw one row at a time

        //holds current samples (= GRIDY)
        ofSoundPlayer beatArray[GRIDY];

        //holds all samples
        ofSoundPlayer beatArrayFull[NUMSAMPLES];

        //button to select new samples
        ofRectangle loadClipButton[GRIDY];
        bool loadClipHover[GRIDY];

        //button to select volume
        ofRectangle volumeSlider[GRIDY];
        float volumeSliderVal[GRIDY];

        int currentChange;
        bool selectSampleMode;

	//width and height of matrix squares
        int wid;
        int hei;

        //need a vector of strings, C++ makes it difficult cause that's what C++ does
        char* strarray[NUMSAMPLES];
        vector<string> sampleVec;

};
