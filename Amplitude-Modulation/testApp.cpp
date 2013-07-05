#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetFrameRate(60);
    ofSetCircleResolution(100);

    // 2 output channels
    // 0 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 buffers (latency)
	ofSoundStreamSetup(2,0,this, 44100,256, 4);

    // we create two sine wave oscillators
	frequency								= 440;
	position								= 0;
	positionAdder							= (frequency / 44100.0) * TWO_PI;
	frequencyModulator						= 100;
	positionModulator						= 0;
	positionModulatorAdder					=  (frequencyModulator / 44100.0) * TWO_PI;
	modulator								= 0;
	sample	                                = 0;

    //edit these to increase number of frequencies or number of modulator nodes
    //note you must ALSO edit these values in testApp.h
    //numNodes must equal numFreq
    numNodes = 16;
    numFreq = 16;

    nodeWidth = 1200/numNodes;
    nodeHeight = 640/numFreq;
    timer = 0;
    triggerIndex = 0;

    //set default values
    for(int a=0;a<numNodes;a++){
    nodeY[a] = 640;
    freqY[a] = 1;
    }
}

//--------------------------------------------------------------
void testApp::update(){

    if(triggerIndex!=numNodes){
    //freqSlope = (nodeY[triggerIndex+1]-nodeY[triggerIndex])/(60*500);
    }

    frequency = freqY[triggerIndex]*60; //increase by 60Hz
	positionAdder = (frequency / 44100.0) * TWO_PI;
	frequencyModulator = (640-nodeY[triggerIndex])/20;
	//frequencyModulatorDynamic += freqSlope;
	//printf("dynamic: %f \n",frequencyModulatorDynamic);
	positionModulatorAdder = (frequencyModulator / 44100.0) * TWO_PI;
}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(0,0,0);
    ofFill();
    ofSetLineWidth(5);

    //timer for trigger band
    if(ofGetElapsedTimeMillis()-timer>500){ //every hald second it moves one step in the x direction
    triggerIndex++;
    //frequencyModulatorDynamic = (640-nodeY[triggerIndex])/20; //reset this
    timer = ofGetElapsedTimeMillis();
        if(triggerIndex==numNodes){
        triggerIndex=0;
        }
    }

    int ii;
    int idxa = 0;
    int innerRectX;
    int innerRectY;
    for(ii=0; ii < 1200; ii=ii+nodeWidth){
        if(triggerIndex==idxa){
        ofSetColor(35,35,35);
        }
        else{
            if((mouseX>ii)&&(mouseX<(ii+nodeWidth))){
            //could add mouseover effect here, but for now, do nothing
            ofSetColor(15,15,15);
            }
            else{
            ofSetColor(15,15,15);
            }
        }
        //draw x rectanges
        ofRect(ii,0,ii+nodeWidth,640);

        //draw frequency rectangles
        ofSetColor(50,50,50);
        ofRect(ii,640-(freqY[idxa]*nodeHeight),ii+nodeWidth,nodeHeight);

        //draw modulation circules
        ofSetColor(15,45,100);
        ofCircle(ii+(nodeWidth/2),nodeY[idxa],nodeWidth/4);

    idxa++;
    }

    //draw lines
    int idx = 0;
    for(ii=0; ii < 1200; ii=ii+nodeWidth){
        ofSetColor(15,45,100);
        if(idx<numNodes-1){
        ofLine(ii+(nodeWidth/2),nodeY[idx],ii+(nodeWidth/2)+(nodeWidth),nodeY[idx+1]);
        }
    idx++;
    }
}



void testApp::audioRequested( float *output, int bufferSize, int nChannels){

	for (int i = 0; i < bufferSize; i++){
		modulator = ofMap(sin (positionModulator), -1,1, 0,1); //We are getting values between -1 and 1, but we need a scalar to multiply the amplitude of the carrier wave, so we scale it to 0-1
		positionModulator += positionModulatorAdder;

		sample = sin( position ) * modulator;  // we use the modulator wave to modulate the amplitude of the
		position += positionAdder;               // carrier wave/oscillator

		output[i * 2 + 0] = sample;			// left channel
		output[i * 2 + 1] = sample;			// right channel
	}

	while (position > TWO_PI) {
		position -= TWO_PI;
	}

	while (positionModulator > TWO_PI){
		positionModulator -= TWO_PI;
	}

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

    //reset
    if(key=='r'){
        for(int a=0;a<numNodes;a++){
        nodeY[a] = 640;
        freqY[a] = 1;
        }
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    if(button==0){
        int idx=0;
        for(int ii=0; ii < 1200; ii=ii+nodeWidth){
            if((x>ii)&&(x<(ii+nodeWidth))){
            nodeY[idx] = y;
            }
        idx++;
        }
    }

    if(button==2){
        int idxa=0;
        for(int ii=0; ii < 1200; ii=ii+nodeWidth){
            if((x>ii)&&(x<(ii+nodeWidth))){
                int idxb=0;
                for(int jj=640; jj > 0; jj=jj-nodeHeight){
                    if((y<jj)&&(y>(jj-nodeHeight))){
                    freqY[idxa] = idxb+1;
                    }
                idxb++;
                }
            }
            idxa++;
        }
    }

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
