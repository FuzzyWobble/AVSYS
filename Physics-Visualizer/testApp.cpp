#include "testApp.h"
		
//--------------------------------------------------------------
void testApp::setup(){	 
	
	// 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// BUFFER_SIZE samples per buffer
	// 4 num buffers (latency)
	
	ofSoundStreamSetup(0,2,this, 44100, BUFFER_SIZE, 4);	
	
	left = new float[BUFFER_SIZE];
	right = new float[BUFFER_SIZE];	
	
	FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.95f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies
	
	ofSetFrameRate(30);
	ofEnableSmoothing();
	
	//box2d physics objects
	box2d.init();
	box2d.setGravity(0.0,20.0);
	box2d.createGround();
	//box2d.drawGround();
	box2d.createBounds();
	
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	counter = 0;
	
	firstLoop = true;

}


//--------------------------------------------------------------
void testApp::update(){
	ofBackground(15);
	box2d.update();
	
	float avg_power = 0.0f;
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left, BUFFER_SIZE, &magnitude[0],&phase[0], &power[0],&avg_power);
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	FFTanalyzer.calculate(freq);
	
	if(firstLoop){ //ONLY called ONCE!

		for(int i=0; i<FFTanalyzer.nAverages; i++) {
			
			ofxBox2dCircle circle;
			circle.setPhysics(1.0, 0.0, 0.15);
			circle.setup(box2d.getWorld(), 90+i*65, ofGetHeight()-12.0, 24.0);
			fft_circles.push_back(circle);	
			xpos[i] = 80+i*50;

		}
		firstLoop = false;
		ofSleepMillis(1000);
	}
}

//--------------------------------------------------------------
void testApp::draw(){


	ofSetColor(50);
	for (int i = 0; i < FFTanalyzer.nAverages; i++){
		ofRect(66+i*65,ofGetHeight(),48,-FFTanalyzer.averages[i] * 20);
	}
	ofSetColor(90);
	for (int i = 0; i < FFTanalyzer.nAverages; i++){
		ofRect(66+i*65,ofGetHeight()-FFTanalyzer.peaks[i] * 20,48,-6);
	}
	
	ofSetColor(150,30,30);
	for(int i=0;i<boxes.size();i++) {
		ofFill();
		boxes[i].draw();
	}
	for(int i=0;i<circles.size();i++) {
		ofFill();
		circles[i].draw();
	}
	ofSetColor(255);


	
	float direct;
	for(int i=0; i<FFTanalyzer.nAverages; i++){ //loop=18
		
		ofVec2f current_pos = fft_circles[i].getPosition();
		float pct_height = (ofGetHeight()-current_pos.y)/(ofGetHeight()); 
		
		if(prevFft[i]-FFTanalyzer.peaks[i]>0){ //going down 
			ofSetColor(200);
			//direct = 1.0 * pct_height;	
			direct = 5.0;
		}
		else{  //going up
			ofSetColor(30,160,30); 
			//direct = -0.20 * (1/pct_height);	
			direct = -1.5 * (1.0-pct_height);
		}
		prevFft[i] = FFTanalyzer.peaks[i];
		
		forceFftVec.set(0, direct*FFTanalyzer.peaks[i]);
		fft_circles[i].addForce(forceFftVec,5.0);
		ofDrawBitmapString(ofToString(i+1)+": "+ofToString(pct_height), 10, 15*i+20);
		ofFill();
		fft_circles[i].draw();
		
	}
	 
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	if(key == 'c') { //press c to create circles at mouse position
		float r = ofRandom(6, 10);		
		ofxBox2dCircle circle;
		circle.setPhysics(0.2, 0.25, 0.1);
		circle.setup(box2d.getWorld(), mouseX, mouseY, r);
		circles.push_back(circle);
	}
	
//	if(key == 'b') { //press b to create box at mouse position
//		float w = ofRandom(4, 10);		
//		ofxBox2dRect rect;
//		rect.setPhysics(0.2, 0.25, 0.1);
//		rect.setup(box2d.getWorld(), mouseX, mouseY, w, w);
//		boxes.push_back(rect);
//	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
}

//--------------------------------------------------------------
void testApp::audioReceived (float * input, int bufferSize, int nChannels){	
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}
}

