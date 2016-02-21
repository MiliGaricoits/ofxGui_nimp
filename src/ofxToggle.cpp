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
        (!midiLearnActive && !editLeftAudioInActive && !editRightAudioInActive)) {
        if(setValue(args.x, args.y, true)){
            return true;
        }
    }
    else if(b.inside(ofPoint(args.x,args.y))) {                                     // if midi learn is active and i'm pressing this toggle
        clicked = !clicked;
        if(editLeftAudioInActive) {
            selectedForLeftAudio = !selectedForLeftAudio;
        }
        if(editRightAudioInActive) {
            selectedForRightAudio = !selectedForRightAudio;
        }
        return true;
    }
    else if ((midiLearnActive || editLeftAudioInActive || editRightAudioInActive)   // if midi learn is active, i was clicked, and command is beign pressed
             && commandPressed && clicked) {                                        // i shouldn't change myself
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
        (editRightAudioInActive && value.getName() == "Edit Right FFT Inputs")) {
        cross.setStrokeColor(ofColor(0));
    }
    else {
        cross.setStrokeColor(thisTextColor);
    }
	cross.setStrokeColor(thisTextColor);
	cross.setStrokeWidth(1);
	cross.setFilled(false);
	cross.moveTo(b.getPosition()+checkboxRect.getTopLeft());
	cross.lineTo(b.getPosition()+checkboxRect.getBottomRight());
	cross.moveTo(b.getPosition()+checkboxRect.getTopRight());
	cross.lineTo(b.getPosition()+checkboxRect.getBottomLeft());

	textMesh = getTextMesh(getName(), b.x+textPadding + checkboxRect.width, b.y+b.height / 2 + 4);
    
    if (((clicked && midiLearnActive) || (selectedForRightAudio && editRightAudioInActive) || (selectedForLeftAudio && editLeftAudioInActive)) &&
        (value.getName() != "Edit Left FFT Inputs" && value.getName() != "Edit Right FFT Inputs")) {
        
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
        border.setFilled(true);
        border.rectangle(b.x -1, b.y -1, b.width +2, b.height +2);
        clicked = true;
    }
}

void ofxToggle::render(){
    
    if (((clicked && midiLearnActive) || (selectedForRightAudio && editRightAudioInActive) || (selectedForLeftAudio && editLeftAudioInActive)) &&
        (value.getName() != "Edit Right FFT Inputs") && (value.getName() != "Edit Left FFT Inputs")) {
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
