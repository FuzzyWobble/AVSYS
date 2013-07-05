#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 900,630, OF_WINDOW);	//OF_WINDOW
	ofRunApp( new testApp());

}
