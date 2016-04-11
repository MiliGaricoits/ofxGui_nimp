#include "ofxToggle.h"
#include "ofGraphics.h"

ofxToggle::ofxToggle(ofParameter<bool> _bVal, float width, float height){
	setup(_bVal,width,height);
}

ofxToggle::~ofxToggle(){
	value.removeListener(this,&ofxToggle::valueChanged);
}

ofxToggle * ofxToggle::setup(ofParameter<bool> _bVal, float width, float height){
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value.makeReferenceTo(_bVal);
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	value.addListener(this,&ofxToggle::valueChanged);
	registerMouseEvents();
    registerKeyEvents();
	generateDraw();

	return this;

}

ofxToggle * ofxToggle::setup(string toggleName, bool _bVal, float width, float height){
	value.set(toggleName,_bVal);
	return setup(value,width,height);
}


bool ofxToggle::mouseMoved(ofMouseEventArgs & args){
	if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxToggle::mousePressed(ofMouseEventArgs & args){
    
    if (value.getName() == "Edit Left FFT Inputs" ||
        value.getName() == "Edit Right FFT Inputs" ||                               // if midi learn is inactive, do normal slider stuff
        value.getName() == "Edit OSC Inputs" ||
        (!midiLearnActive && !editLeftAudioInActive && !editRightAudioInActive && !editOSCActive)) {
            
        if(setValue(args.x, args.y, true)){
            return true;
        }
    }
    else if(b.inside(ofPoint(args.x,args.y))) {                                     // if midi learn is active and i'm pressing this toggle
        clicked = !clicked;
        if(editLeftAudioInActive) {
            selectedForLeftAudio = !selectedForLeftAudio;
            selectedForNodeId = selectedForLeftAudio ? editNodeId : 0;
        }
        if(editRightAudioInActive) {
            selectedForRightAudio = !selectedForRightAudio;
            selectedForNodeId = selectedForRightAudio ? editNodeId : 0;
        }
        if(editOSCActive) {
            selectedForOSC = !selectedForOSC;
            selectedForNodeId = selectedForOSC ? editNodeId : 0;
            
            ofxOSCGuiEvent ev;
            ev.oscNodeId = editNodeId;
            ev.paramName = getName();
            ev.add = selectedForOSC;
            
            ofNotifyEvent(addOrRemoveOSCInputBaseGui, ev);
        }
        return true;
    }
    else if ((midiLearnActive && clicked && commandPressed) // if midi learn or edit audio in are active, i was clicked
             || editLeftAudioInActive                       // and command is beign pressed, i shouldn't change myself
             || editRightAudioInActive
             || editOSCActive) {
        
        return true;
    }
    else {
        clicked = false;
        return false;
    }
}

bool ofxToggle::mouseDragged(ofMouseEventArgs & args){
	if(bGuiActive && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxToggle::mouseReleased(ofMouseEventArgs & args){
	bool wasGuiActive = bGuiActive;
	bGuiActive = false;
	if(wasGuiActive && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxToggle::keyPressed(ofKeyEventArgs &args) {
    commandPressed = (args.key == OF_KEY_LEFT_COMMAND || args.key == OF_KEY_RIGHT_COMMAND);
    return false;
}

bool ofxToggle::keyReleased(ofKeyEventArgs &args) {
    commandPressed = false;
    return false;
}

void ofxToggle::generateDraw(){
	bg.clear();
	bg.setFillColor(thisBackgroundColor);
	bg.rectangle(b);

	fg.clear();
	if(value){
		fg.setFilled(true);
        if (editLeftAudioInActive && value.getName() == "Edit Left FFT Inputs") {
            fg.setFillColor(thisLeftAudioInColor);
        }
        else if (editRightAudioInActive && value.getName() == "Edit Right FFT Inputs") {
            fg.setFillColor(thisRightAudioInColor);
        }
        else if (editOSCActive && value.getName() == "Edit OSC Inputs") {
            fg.setFillColor(thisOSCColor);
        }
        else {
            fg.setFillColor(thisFillColor);
        }
	}else{
		fg.setFilled(false);
		fg.setStrokeWidth(1);
		fg.setStrokeColor(thisFillColor);
	}
	fg.rectangle(b.getPosition()+checkboxRect.getTopLeft(),checkboxRect.width,checkboxRect.height);

	cross.clear();
    if ((editLeftAudioInActive && value.getName() == "Edit Left FFT Inputs") ||
        (editRightAudioInActive && value.getName() == "Edit Right FFT Inputs") ||
        (editRightAudioInActive && value.getName() == "Edit OSC Inputs")) {
        cross.setStrokeColor(ofColor(0));
    }
    else {
        cross.setStrokeColor(thisTextColor);
    }
	cross.setStrokeWidth(1);
	cross.setFilled(false);
	cross.moveTo(b.getPosition()+checkboxRect.getTopLeft());
	cross.lineTo(b.getPosition()+checkboxRect.getBottomRight());
	cross.moveTo(b.getPosition()+checkboxRect.getTopRight());
	cross.lineTo(b.getPosition()+checkboxRect.getBottomLeft());

	textMesh = getTextMesh(getName(), b.x+textPadding + checkboxRect.width, b.y+b.height / 2 + 4);
    
    if (((clicked && midiLearnActive)
        || (midiLearnActive && midiControlActive != -1 && (midiControlActive == selectedForMidiControl))
        || (((selectedForRightAudio && editRightAudioInActive)
        || (selectedForLeftAudio && editLeftAudioInActive)
        || (selectedForOSC && editOSCActive))
        && (selectedForNodeId == editNodeId)))
        && (value.getName() != "Edit Right FFT Inputs")
        && (value.getName() != "Edit Left FFT Inputs")
        && (value.getName() != "Edit OSC Inputs"))  {
        
        border.clear();
        if (midiLearnActive){
            border.setFillColor(ofColor(thisMidiLearnColor, 255));
        }
        else if (selectedForRightAudio && editRightAudioInActive) {
            border.setFillColor(ofColor(thisRightAudioInColor, 255));
        }
        else if (selectedForLeftAudio && editLeftAudioInActive) {
            border.setFillColor(ofColor(thisLeftAudioInColor, 255));
        }
        else if (selectedForOSC && editOSCActive) {
            border.setFillColor(ofColor(thisOSCColor, 255));
        }
        border.setFilled(true);
        border.rectangle(b.x -1, b.y -1, b.width +2, b.height +2);
        
        (midiLearnActive && midiControlActive != -1 && (midiControlActive == selectedForMidiControl)) ? clicked = false : clicked = true;
    }
}

void ofxToggle::render(){
    
    if (((clicked && midiLearnActive)
        || (midiLearnActive && midiControlActive != -1 && (midiControlActive == selectedForMidiControl))
        || (((selectedForRightAudio && editRightAudioInActive)
        || (selectedForLeftAudio && editLeftAudioInActive)
        || (selectedForOSC && editOSCActive))
        && (selectedForNodeId == editNodeId)))
        && (value.getName() != "Edit Right FFT Inputs")
        && (value.getName() != "Edit Left FFT Inputs")
        && (value.getName() != "Edit OSC Inputs")) {
        border.draw();
    }
    
	bg.draw();
	fg.draw();

	if( value ){
		cross.draw();
	}

	ofColor c = ofGetStyle().color;
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(thisTextColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

bool ofxToggle::operator=(bool v){
	value = v;
	return v;
}

ofxToggle::operator const bool & (){
	return value;
}

bool ofxToggle::setValue(float mx, float my, bool bCheck){

	if( !isGuiDrawing() ){
		bGuiActive = false;
		return false;
	}
	if( bCheck ){
		ofRectangle checkRect = checkboxRect;
		checkRect.x += b.x;
		checkRect.y += b.y;

		if( checkRect.inside(mx, my) ){
			bGuiActive = true;
		}else{
			bGuiActive = false;

		}
	}
	if( bGuiActive ){
		value = !value;
		return true;
	}
	return false;
}

ofAbstractParameter & ofxToggle::getParameter(){
	return value;
}

void ofxToggle::valueChanged(bool & value){
	generateDraw();
}
