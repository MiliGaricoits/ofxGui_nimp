//
//  ofxTextInput.h
//  nimp
//
//  Created by Mili Garicoits on 1/24/16.
//
//

#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

class ofxTextInput: public ofxBaseGui {
public:
    ofxTextInput(){};
    ofxTextInput(ofParameter<string> _label, float width = defaultWidth, float height = defaultHeight);
    ~ofxTextInput();
    
    ofxTextInput * setup(ofParameter<string> _label, float width = defaultWidth, float height = defaultHeight);
    ofxTextInput * setup(string labelName, ofParameter<string> label, float width = defaultWidth, float height = defaultHeight);
    
    // Abstract methods we must implement, but have no need for!
    virtual bool mouseMoved(ofMouseEventArgs & args);
    virtual bool mouseDragged(ofMouseEventArgs & args);
    virtual bool mousePressed(ofMouseEventArgs & args);
    virtual bool mouseReleased(ofMouseEventArgs & args);
    bool keyPressed(ofKeyEventArgs &args);
    bool keyReleased(ofKeyEventArgs &args){return false;};
    
    template<class ListenerClass, typename ListenerMethod>
    void addListener(ListenerClass * listener, ListenerMethod method){
        value.addListener(listener,method);
    }
    
    template<class ListenerClass, typename ListenerMethod>
    void removeListener(ListenerClass * listener, ListenerMethod method){
        value.removeListener(listener,method);
    }
    
    
    string operator=(string v) { value = v; return v; }
    operator const string & () { return value; }
    
    ofAbstractParameter& getParameter();
    
protected:
    void render();
    bool setValue(float mx, float my, string s);
    bool setValue(float mx, float my, bool bCheckBounds){return false;}
    void generateDraw();
    void valueChanged(string & value);
    
    int PADDING = 3;
    
    ofParameter<string> value;
    string text;
    string displayText;
    string name;
    ofPath bg;
    ofVboMesh textMesh;
    bool bGuiActive;
    float lastTimeCursorMoved;
    unsigned int cursorPosition, cursorPositionDisplay, cursorPositionInit, firstVisibleCharacterIndex;
};

