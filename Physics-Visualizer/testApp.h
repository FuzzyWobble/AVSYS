#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "fft.h"
#include "FFTOctaveAnalyzer.h"
#include "ofxBox2d.h"

#define BUFFER_SIZE 512

class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		void audioReceived 	(float * input, int bufferSize, int nChannels); 
	
		FFTOctaveAnalyzer FFTanalyzer;

	private:	
		
		float * left;
		float * right;
		int bufferCounter;
		fft myfft;
		
		float magnitude[BUFFER_SIZE];
		float phase[BUFFER_SIZE];
		float power[BUFFER_SIZE];
		float freq[BUFFER_SIZE/2];
	
		//box 2d
		float px, py;
		bool bDrawLines;
		bool bMouseForce;
		
		ofxBox2d box2d;			 	 
		vector <ofxBox2dCircle>	circles;		 
		vector <ofxBox2dRect> boxes;	
	    vector <ofxBox2dCircle>	fft_circles;
		int counter;
		bool firstLoop;
		ofVec2f forceFftVec;
		int xpos[16];
		int prevFft[16];
		int direction[16];
	
	
};

#endif	

