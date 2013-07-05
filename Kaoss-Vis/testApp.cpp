#include "testApp.h"
#include "beatDetect.h"

int testApp_buffer_size = 1024;
int testApp_fft_size = 512;

bool detect=true;

void testApp::setup(){
	ofBackground(20);
	ofSetFrameRate(60);
	ofEnableSmoothing();
	
	ofSoundStreamSetup(0, 1, this, 44100, testApp_buffer_size, 4);
	inputBufferCopy = new float [512 *2];
	
	midiIn.listPorts();
	midiIn.openPort(0);
	midiIn.addListener(this);
	
	beat_toggle=true;
	beat_toggle_timer = 0;
	streak_timer = 0;
	color_mode = 1; //1,2,3:red,green,blue
}

void testApp::audioReceived(float* input, int bufferSize, int nChannels) {
    bd.audioReceived(input, bufferSize);
	
	//RUX's volume code, used on y axis of squares 
	memcpy(inputBufferCopy, input, bufferSize*nChannels*sizeof(float));
    volume = 0;
    for(int i = 0; i < bufferSize * nChannels; i++){
        volume += input[i] * input[i];  // add the square value to eliminate any negative values
    }
    volume /= bufferSize * nChannels;   // get the average of all square values
    volume = sqrt(volume);  
}


void testApp::update() {
    bd.updateFFT();
	cout << "X: " << midi_x << " / Y: " << midi_y << endl;
}

void testApp::draw()
{
	
	
	ofFill();
	squareSize=110;
	int yLoop = (900/squareSize) + 0; 
	int xLoop = (1200/squareSize) - 1; 
	for(int yy=0;yy<yLoop;yy++){ 
		for(int xx=0;xx<xLoop;xx++){ 
			
			int xpos = (xx*squareSize)+(12*xx)+12;
			int ypos = (yy*squareSize)+(12*yy)+12;
			
			//streak code
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			
			//shift array right after counter
			if(ofGetElapsedTimeMillis()-streak_timer>200){
				for(int j=6;j>0;j--){ //skip case 0
					streak[j][0]=streak[j-1][0];	
					streak[j][1]=streak[j-1][1];
				}	
				streak_timer = ofGetElapsedTimeMillis();	
			}
			//add square at current location
			if((mx>=xpos-6)&&(mx<xpos+squareSize+6)&&(my>=ypos-6)&&(my<ypos+squareSize+6)){
				streak[0][0] = xpos;
				streak[0][1] = ypos;
			}
			//draw streak
			for(int j=0;j<7;j++){
				//as we get further away, color fades
				if(color_mode==1){ //red
					ofSetColor(20*(7-j), 20, 20); 
				}
				else if(color_mode==2){ //green
					ofSetColor(20, 20*(7-j), 20); 
				}
				else{ //blue
					ofSetColor(20, 20, 20*(7-j));
				}
				ofRect(streak[j][0]+5,streak[j][1]+5,squareSize-10,squareSize-10); //draw streak
			}
			
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			
			drawSquareOutline(xpos,ypos,squareSize); //call function to draw square outline
			
		}
	}
	
	//beat detect for color change
	if(bd.isBeatRange(12,18,4)){
		if(ofGetElapsedTimeMillis()-beat_toggle_timer>500){
			if(beat_toggle){
				color_mode++;
			}
			else{
				color_mode++;
			}
			beat_toggle =! beat_toggle;
			beat_toggle_timer = ofGetElapsedTimeMillis();
			if(color_mode>3){
				color_mode=1;
			}
		}
	}
	
	//ofBackground(20);	
}

void testApp::keyPressed(int key){
	color_mode++;
	if(color_mode>3){
		color_mode=1;
	}
}

void testApp::keyReleased(int key){
	
}

void testApp::mouseMoved(int x, int y){
	//mx = x;
	//my = y;
}

void testApp::mouseDragged(int x, int y, int button){
	
}

void testApp::mousePressed(int x, int y, int button){
	
}

void testApp::mouseReleased(int x, int y, int button){
	
}

void testApp::windowResized(int w, int h){
	
}
void testApp::newMidiMessage(ofxMidiEventArgs& eventArgs) {
	
	// store some data from midi message in variables
	valueOne = eventArgs.byteOne;
	valueTwo = eventArgs.byteTwo;
	id = eventArgs.channel;
	port = eventArgs.port;
	timestamp = eventArgs.timestamp;
	
	if(valueOne==12){ //x direction
		midi_x = valueTwo;
		midi_x = ofMap(midi_x,0,127,6,ofGetWidth()-squareSize); 
		mx = midi_x;
	}
	else if(valueOne==13){ //y direction
		midi_y = valueTwo;
		midi_y = ofMap(midi_y,0,127,ofGetHeight()-squareSize,6);
		my = midi_y;
	}
	else{
		//some other message
	}
	
}

void testApp::drawSquareOutline(int x, int y, int size){	
	
	ofNoFill();
	ofSetLineWidth(2);
	
	if((mx>=x-6)&&(mx<x+squareSize+6)&&(my>=y-6)&&(my<y+squareSize+6)){
		if(color_mode==1){ //red
			ofSetColor(140, 20, 20); 
		}
		else if(color_mode==2){ //green
			ofSetColor(20, 140, 20); 
		}
		else{ //blue
			ofSetColor(20, 20, 140);
		}
	}
	else{
		if(color_mode==1){ //red
			ofSetColor(90, 40, 40); 
		}
		else if(color_mode==2){ //green
			ofSetColor(30, 80, 30); 
		}
		else{ //blue
			ofSetColor(50, 50, 100);
		}
	}
	
	//draw bottom line - FFT reverse
	ofBeginShape();
    for(int i = 0; i < size; i++){
		int backwards = size-i;
        float sample = ofMap(bd.magnitude[backwards], 0, 1, 0, 5);  
		ofVertex(x + i, y - sample + size);
    }
    ofEndShape();
	
	//draw top line - FFT
	ofBeginShape();
    for(int i = 0; i < size; i++){
        float sample = ofMap(bd.magnitude[i], 0, 1, 0, 5); 
		ofVertex(x + i, y + sample);
    }
    ofEndShape();
	
	//draw left line - volume
	ofBeginShape();
    for(int i = 0; i < size; i++){
        float sample = ofMap(inputBufferCopy[i*1], -1, 1, 0, size*2); 
		ofVertex(x + sample - size, y + i);
    }
    ofEndShape();
	
	//draw right line - volume
	ofBeginShape();
    for(int i = 0; i < size; i++){
        float sample = ofMap(inputBufferCopy[i*1], -1, 1, 0, size*2);  
		ofVertex(x + sample, y + i);
    }
    ofEndShape();	
	
}
