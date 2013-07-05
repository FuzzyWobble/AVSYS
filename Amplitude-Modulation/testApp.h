#pragma once
#include "ofMain.h"

class testApp : public ofBaseApp{

        public:
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
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void        audioRequested(float * input, int bufferSize, int nChannels);
		float		frequency;
		float		position;
		float		positionAdder;
		float		frequencyModulator;
		float       frequencyModulatorDynamic;
		float		positionModulator;
		float		positionModulatorAdder;
		float       modulator;
		float       sample;

        //edit these to increase number of frequencies or number of modulator nodes
        //note you must ALSO edit these values in testApp.cpp
        //numNodes must equal numFreq
        int nodeY[16];
        int freqY[16];

        int numNodes;
        int numFreq;
        int nodeWidth;
        int nodeHeight;
        int mouseX;
        int mouseY;
        int triggerIndex;
        long timer;
        float freqSlope;

};
