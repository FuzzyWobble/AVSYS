#include "../include/samplesquare.h" //code blocks, why you put my .h file here!

// -----------------------------------
void samplesquare::setup(ofRectangle _rect){
    rect = _rect;
    color.r = 190;
    color.g = 190;
    color.b = 190;
    bPressed = false;
}

// -----------------------------------
void samplesquare::draw(){
    if(bPressed == true){
        color.set(165, 227, 231);
        ofFill();
    } else {
        color.set(190, 190, 190);
        ofNoFill();
    }
    ofSetColor(color.r, color.g, color.b);
    ofRect(rect);
}


// -----------------------------------
void samplesquare::mouseDown(int x, int y){
    if(rect.inside(x, y) == true){
        bPressed =! bPressed;
    }
}
