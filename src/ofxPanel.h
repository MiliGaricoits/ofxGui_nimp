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


	ofEvent<void> loadPressedE;
	ofEvent<void> savePressedE;
    
    //custom
    void setMidiLearnActive(bool active_);
    void setMidiControlActive(int control_);
    void resetMidiControlActive(int control_);
    void setEditLeftAudioInActive(bool active_, int node_);
    void setEditRightAudioInActive(bool active_, int node_);
    void setEditOSCActive(bool active_, int node_);
    virtual void setSelectedForLeftAudio(bool active_, int node_, string name_);
    virtual void setSelectedForRightAudio(bool active_, int node_, string name_);
    virtual void setSelectedForOSC(bool active_, int node_, string name_);
    virtual void setSelectedForMIDI(bool active_, int control_, string name_);
    vector<string> getAttributesClicked();
//    vector<string> getAttributesForMidiLearn();
//    vector<string> getAttributesForAudioIn();
    virtual ofxBaseGui* find(string name);
    
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
