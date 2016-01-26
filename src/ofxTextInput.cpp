//
//  ofxTextInput.cpp
//  nimp
//
//  Created by Mili Garicoits on 1/24/16.
//
//

#include "ofxTextInput.h"
#include "ofGraphics.h"

ofxTextInput::ofxTextInput(ofParameter<string> _value, float width, float height){
    setup(_value,width,height);
}

ofxTextInput::~ofxTextInput(){
    value.removeListener(this,&ofxTextInput::valueChanged);
    ofRemoveListener(ofEvents().keyPressed, this, &ofxTextInput::keyPressed);
    unregisterMouseEvents();
    ofUnregisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
}

ofxTextInput* ofxTextInput::setup(ofParameter<string> _value, float width, float height) {
    value.makeReferenceTo(_value);
    text = _value;
    b.width  = width;
    b.height = height;
    cursorPosition = text.length();
    lastTimeCursorMoved = ofGetElapsedTimef();
    clicked = false;
    
    generateDraw();
    registerMouseEvents();
    ofRegisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
    value.addListener(this,&ofxTextInput::valueChanged);
    return this;
}

ofxTextInput* ofxTextInput::setup(string labelName, ofParameter<string> _value, float width, float height) {
    value.set(labelName,_value);
    return setup(_value,width,height);
}

void ofxTextInput::generateDraw(){
    bg.clear();
    
    bg.setFillColor(thisBackgroundColor);
    bg.setFilled(true);
    bg.rectangle(b);
    
    string name;
    if(!getName().empty()){
        name = getName() + ": ";
    }
    
    textMesh = getTextMesh(name + text, b.x + textPadding, b.y + b.height / 2 + 4);
}

void ofxTextInput::render() {
    ofColor c = ofGetStyle().color;
    
    bg.draw();
    
    ofBlendMode blendMode = ofGetStyle().blendingMode;
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableAlphaBlending();
    }
    ofSetColor(textColor);
    
    bindFontTexture();
    textMesh.draw();
    unbindFontTexture();
    
    if (clicked) {
        ofPushStyle();
        float timeFrac = 0.5 * ofClamp(cos(6.0f * (ofGetElapsedTimef()-lastTimeCursorMoved))*4, -1, 1) + 0.5;
        ofColor col = thisBorderColor;
        ofSetColor(col.r * timeFrac, col.g * timeFrac, col.b * timeFrac);
        
        ofLine(b.x + PADDING + cursorPosition*8, b.y,
               b.x + PADDING + cursorPosition*8, b.y + b.height);
        ofPopStyle();
    }
    
    ofSetColor(c);
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableBlendMode(blendMode);
    }
}

ofAbstractParameter & ofxTextInput::getParameter(){
    return value;
}

void ofxTextInput::valueChanged(string & value){
    generateDraw();
}


bool ofxTextInput::mouseMoved(ofMouseEventArgs & args){
    if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
        return true;
    }else{
        return false;
    }
}

bool ofxTextInput::mouseDragged(ofMouseEventArgs & args){
    if(bGuiActive && b.inside(ofPoint(args.x,args.y))){
        return true;
    }else{
        return false;
    }
}

bool ofxTextInput::mousePressed(ofMouseEventArgs & args){
    if (!isGuiDrawing()){
        bGuiActive = false;
        return false;
    }
    if (b.inside(ofPoint(args.x,args.y))){
        bGuiActive = true;
        clicked    = true;
        setBorderColor(ofColor(255,0,127));
        lastTimeCursorMoved = ofGetElapsedTimef();
        return true;
    } else {
        bGuiActive = false;
        clicked    = false;
        setBorderColor(borderColor);
        return false;
    }
}

bool ofxTextInput::mouseReleased(ofMouseEventArgs & args){
    bool wasGuiActive = bGuiActive;
    bGuiActive = false;
    if (wasGuiActive && b.inside(ofPoint(args.x,args.y))){
        return true;
    } else {
        return false;
    }
}

void ofxTextInput::keyPressed(ofKeyEventArgs &args) {
    
    if(clicked)
    {
        lastTimeCursorMoved = ofGetElapsedTimef();
        switch (args.key)
        {
            case OF_KEY_BACKSPACE:
                if (text.size() > 0 && cursorPosition > 0) {
                    
                    cursorPosition--;
                    (text.erase(cursorPosition, 1));
                    
                    // when we're deleting the first visible character, shift the string to the right
                    if(firstVisibleCharacterIndex == cursorPosition)
                        firstVisibleCharacterIndex = 0;
                    //recalculateDisplayString();
                }
                break;
                
            case OF_KEY_DEL:
                if (text.size() > 0 && cursorPosition < text.length()) {
                    
                    (text.erase(cursorPosition, 1));
                    //recalculateDisplayString();
                }
                break;
                
            case OF_KEY_RETURN:
                break;
                
            case OF_KEY_RIGHT:
            case OF_KEY_DOWN:
                if(cursorPosition < text.length()) {
                    
                    cursorPosition++;
                    //recalculateDisplayString();
                }
                break;
                
            case OF_KEY_LEFT:
            case OF_KEY_UP:
                if(cursorPosition > 0) {
                    
                    cursorPosition--;
                    //recalculateDisplayString();
                }
                break;
                
#if (OF_VERSION_MINOR > 7)
            case OF_KEY_TAB:
            case OF_KEY_COMMAND:
            case OF_KEY_CONTROL:
            case OF_KEY_LEFT_SHIFT:
            case OF_KEY_RIGHT_SHIFT:
            case OF_KEY_LEFT_CONTROL:
            case OF_KEY_RIGHT_CONTROL:
            case OF_KEY_LEFT_ALT:
            case OF_KEY_RIGHT_ALT:
            case OF_KEY_LEFT_SUPER:
            case OF_KEY_RIGHT_SUPER:
#endif
            case OF_KEY_F1:
            case OF_KEY_F2:
            case OF_KEY_F3:
            case OF_KEY_F4:
            case OF_KEY_F5:
            case OF_KEY_F6:
            case OF_KEY_F7:
            case OF_KEY_F8:
            case OF_KEY_F9:
            case OF_KEY_F10:
            case OF_KEY_F11:
            case OF_KEY_F12:
            case OF_KEY_PAGE_UP:
            case OF_KEY_PAGE_DOWN:
            case OF_KEY_HOME:
            case OF_KEY_END:
            case OF_KEY_INSERT:
            case OF_KEY_ALT:
            case OF_KEY_SHIFT:
                break;
                
            default: {
                text.insert(cursorPosition, 1, args.key);
                cursorPosition++;
                //recalculateDisplayString();
            }
            break;
        }
        
        value.set(text);
    }
}

bool ofxTextInput::setValue(float mx, float my, string s){

}
