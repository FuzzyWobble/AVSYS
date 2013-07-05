#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "beatDetect.h"
#include "ofEvents.h"
#include "ofxMidi.h"


#define NUM 500

class testApp : public ofBaseApp, public ofxMidiListener {
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
	
	void audioReceived(float* input, int bufferSize, int nChannels);
	
    beatDetect bd;
	
	void drawSquareOutline(int x, int y, int size);
	int squareSize;
	
	float volume;
	float * inputBufferCopy;
	
	int streak[7][2];
	
	int mx;
	int my;
	
	bool beat_toggle;
	long beat_toggle_timer;
	long streak_timer;
	
	int color_mode;
	
	
	
	void newMidiMessage(ofxMidiEventArgs& eventArgs);
	// vars
	int port;
	int id;
	int valueOne;
	int valueTwo;
	int midi_x;
	int midi_y;
	double timestamp;
	char msg[255];
	ofxMidiIn midiIn;
	
};

#endif
