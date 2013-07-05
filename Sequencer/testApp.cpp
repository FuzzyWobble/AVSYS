#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    ofEnableAlphaBlending();
	ofBackground(100); //set background color
    atimer = ofGetElapsedTimeMillis(); // set the current time
    bpm = 60; // start at 120 bpm (use '+' and '-' to increase/decrease bpm's in multiples of 10)
    // set the time interval between ticks - calculate how many miliseconds we need to wait to move to the next step based on bpm
    intervalTime = (60.0/bpm/4)*1000;
    nTicks = 2;
    currentChange = 0;

    // SAMPLE FILES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // yikes we should load these from the dir next time
    strarray = {
        "1.wav", "2.wav", "3.wav", "4.wav", "5.wav",
        "6.wav", "7.wav", "8.wav", "9.wav", "10.wav",
        "11.wav", "12.wav", "13.wav", "14.wav", "15.wav",
        "16.wav", "17.wav", "18.wav", "19.wav", "20.wav",
        "21.wav", "22.wav", "23.wav", "24.wav", "25.wav",
        "26.wav", "27.wav", "28.wav", "29.wav", "30.wav",
        "31.wav", "32.wav", "33.wav", "34.wav", "35.wav",
        "36.wav", "37.wav", "38.wav", "39.wav", "40.wav",
        "41.wav", "42.wav", "43.wav", "44.wav", "45.wav",
        "46.wav", "47.wav", "48.wav", "49.wav", "50.wav",
        "51.wav", "52.wav", "53.wav", "54.wav", "55.wav",
        "56.wav", "57.wav", "58.wav", "59.wav", "60.wav",
        "61.wav", "62.wav", "63.wav", "64.wav", "65.wav",
        "66.wav", "67.wav", "68.wav"
        };

    sampleVec = vector<string>(strarray, strarray + NUMSAMPLES);//making a vector of strings
    int jump = 0;
    for(int i=0;i<sampleVec.size();i++){
        if((i%18==0)&&(i!=0)){
            jump++;
        }
    sampleList[i].set(50*(i%18),50*jump,50,50);
    }

    //load current samples into beat array
    for(int jj=0;jj<GRIDY;jj++){
        beatArray[jj].loadSound(sampleVec[jj]); //load first samples
        beatArray[jj].setLoop(false);
    }
    //load ALL samples intop beat array full
    for(int jj=0;jj<NUMSAMPLES;jj++){
        beatArrayFull[jj].loadSound(sampleVec[jj]); //load all samples
        beatArrayFull[jj].setLoop(false);
    }

    selectSampleMode = false; //start off in play mode

    //calculate width and height of matrix based of X/Y input (GRIDX/GRIDY)
    wid = ofGetWidth()/(GRIDX+2);
    hei = (ofGetHeight()-30)/GRIDY;

    //create sample select rectangles
    for(int jj=0;jj<GRIDY;jj++){
        loadClipButton[jj].set(wid,hei*jj,wid,hei);
    }

    //create volume select rectangles
    for(int jj=0;jj<GRIDY;jj++){
        volumeSlider[jj].set(0,hei*jj,wid,hei);
        volumeSliderVal[jj] = 0.7;
    }

    //create matrix
    for(int jj=0;jj<GRIDY;jj++){
        for(int ii=0;ii<GRIDX;ii++){
            ofRectangle tempRectangle((wid*ii)+(wid*2),hei*jj,wid,hei);
            ssArray[jj][ii].setup(tempRectangle);
        }
    }

}

//--------------------------------------------------------------
void testApp::update(){

    if(selectSampleMode==true){
        //nothing to do here
    }
    else{ //playback mode
        if(ofGetElapsedTimeMillis()-atimer>intervalTime){
            nTicks++;
            if(nTicks>GRIDX+1){
                nTicks = 2;
            }
            for(int jj=0;jj<GRIDY;jj++){
                if(ssArray[jj][nTicks-2].bPressed){
                    beatArray[jj].play();
                }
            }
            atimer = ofGetElapsedTimeMillis(); //reset timer
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){

    if(selectSampleMode==true){ //this is the mode where you select your samples
        for(int i=0;i<sampleVec.size();i++){
            if(sampleListHover[i]==true){ //hover effect
                ofFill();
                ofSetColor(35,35,35);
            }
            else{
                ofFill();
                ofSetColor(55, 55, 55);
            }
            ofRect(sampleList[i]);
        }
        for(int i=0;i<sampleVec.size();i++){ //redraw outlines, annoying
            ofNoFill();
            ofSetColor(190,190,190);
            ofRect(sampleList[i]);
        }
    }
    else{ //this is the playback mode
        //draw matrix
        for(int jj=0;jj<GRIDY;jj++){
            for(int ii=0;ii<GRIDX;ii++){
                ofRectangle tempRectangle((wid*ii)+wid,hei*jj,wid,hei);
                ssArray[jj][ii].draw();
            }
        }
        //draw sample select with hover effects
        for(int jj=0;jj<GRIDY;jj++){
            if(loadClipHover[jj]==true){
                ofFill();
                ofSetColor(35,35,35);
            }
            else{
                ofFill();
                ofSetColor(55, 55, 55);
            }
            ofRect(loadClipButton[jj]);
        }
        //draw volume slider
        for(int jj=0;jj<GRIDY;jj++){
            ofFill();
            ofSetColor(55, 55, 55);
            ofRect(volumeSlider[jj]);
        }
        for(int jj=0;jj<GRIDY;jj++){//redraw outlines, annoying
            ofSetColor(25,25,25);
            float yv = (jj*hei) + (hei - (volumeSliderVal[jj]*hei));
            ofRect(0,yv,wid,4);
        }
        for(int jj=0;jj<GRIDY;jj++){//redraw outlines, annoying
            ofNoFill();
            ofSetColor(190,190,190);
            ofRect(0,hei*jj,wid,hei);
            ofRect(wid,hei*jj,wid,hei);
        }

        ofFill();
        ofSetColor(160, 160, 160, 150);
        ofRect(wid*nTicks, 0, wid, ofGetHeight()-30);

    }

    //footer text and instructions
    ofFill();
    ofSetColor(30,30,30);
    ofRect(0, ofGetHeight()-29, ofGetWidth(), 29);
    ofSetColor(200, 200, 200);
    if(selectSampleMode==true){
        ofDrawBitmapString("Left click to sample, right click to replace.", 10, ofGetHeight()-10);
    }
    else{
        ofDrawBitmapString("[+/-] to adjust BPM   [c] to clear", 10, ofGetHeight()-10);
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case '+':
            bpm+=5;// increase bpm by 5
            intervalTime = (60.0/bpm/4)*1000;
            break;
        case '-':
            bpm-=5;// decrease bpm by 5
            intervalTime = (60.0/bpm/4)*1000;
            break;
        case 'c':
            for(int jj=0;jj<GRIDY;jj++){
                for(int ii=0;ii<GRIDX;ii++){
                    ssArray[jj][ii].bPressed=false;
                }
            }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //toggle hover modes when mouse is moved
    for(int jj=0;jj<GRIDY;jj++){
        if(loadClipButton[jj].inside(x,y)){
            loadClipHover[jj] = true;
        }
        else{
            loadClipHover[jj] = false;
        }
    }
    if(selectSampleMode==true){
        for(int i=0;i<sampleVec.size();i++){
            if(sampleList[i].inside(x,y)){
                sampleListHover[i] = true;
            }
            else{
                sampleListHover[i] = false;
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
//everything that happens on mousepressed
void testApp::mousePressed(int x, int y, int button){
    if(selectSampleMode==true){ //we are in sample select mode
        if(button==0){ //click left mouse
            for(int i=0;i<sampleVec.size();i++){
                if(sampleList[i].inside(x,y)){
                    beatArrayFull[i].play(); //playback sample which was clicked
                }
            }
        }
        if(button==2){ //click right mouse
            for(int i=0;i<sampleVec.size();i++){
                if(sampleList[i].inside(x,y)){
                    beatArray[currentChange].loadSound(sampleVec[i]); //load this sample!
                    beatArray[currentChange].setLoop(false);
                    selectSampleMode = false;
                }
            }
        }

    }
    else{ //playback mode
        //turn on/off matrix
        for(int jj=0;jj<GRIDY;jj++){
            for(int ii=0;ii<GRIDX;ii++){
                ssArray[jj][ii].mouseDown(x,y);
            }
        }
        //switch to sample mode
        for(int jj=0;jj<GRIDY;jj++){
            if(loadClipButton[jj].inside(x,y)){
                currentChange = jj;
                selectSampleMode = true;
            }
        }
        //adjust volume
        for(int jj=0;jj<GRIDY;jj++){ //volume adjust
            if(volumeSlider[jj].inside(x,y)){
                for(int kk=0;kk<GRIDY;kk++){
                    if((y>(kk*hei))&&(y<((kk+1)*hei))){
                        float trya = y-(kk*hei);
                        float volVal = 1.0f*((trya)/hei)*1.0f;
                        volumeSliderVal[jj] = 1.0-volVal;
                        beatArray[jj].setVolume(1.0-volVal);
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

