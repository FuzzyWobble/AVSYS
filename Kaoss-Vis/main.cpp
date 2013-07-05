#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main(){

	ofSetupOpenGL(1110,740, OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new testApp());

}
