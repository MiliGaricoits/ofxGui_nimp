#pragma once

#include "ofxGuiGroup.h"

class ofxGuiGroup;

class ofxPanel : public ofxGuiGroup {
public:
	ofxPanel();
	ofxPanel(const ofParameterGroup & parameters, string filename="settings.xml", float x = 10, float y = 10);
	~ofxPanel();

	ofxPanel * setup(string collectionName="", string filename="settings.xml", float x = 10, float y = 10);
	ofxPanel * setup(const ofParameterGroup & parameters, string filename="settings.xml", float x = 10, float y = 10);

	bool mouseReleased(ofMouseEventArgs & args);
    void setMidiLearnActive(bool active_);
    void setEditAudioInActive(bool active_);

	ofEvent<void> loadPressedE;
	ofEvent<void> savePressedE;
    
    //custom
    vector<string> getAttributesForMidiLearn();
    vector<string> getAttributesForAudioIn();
    
protected:
	void render();
	bool setValue(float mx, float my, bool bCheck);
	void generateDraw();
	void loadIcons();
private:
	ofRectangle loadBox, saveBox;
	static ofImage loadIcon, saveIcon;
    
    ofPoint grabPt;
	bool bGrabbed;
};
