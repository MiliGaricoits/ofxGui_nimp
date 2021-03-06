#pragma once

#include "ofMain.h"
#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofParameter.h"
#include "ofTrueTypeFont.h"
#include "ofxOSCGuiEvent.h"

class ofxBaseGui : public ofNode {
public:
	ofxBaseGui();
	
	virtual ~ofxBaseGui();
	void draw();
	
	void saveToFile(string filename);
	void loadFromFile(string filename);
	
	void setDefaultSerializer(ofPtr<ofBaseFileSerializer> serializer);

	virtual void saveTo(ofBaseSerializer& serializer);
	virtual void loadFrom(ofBaseSerializer& serializer);
	
	string getName();
	void setName(string name);

	virtual void setPosition(ofPoint p);
	virtual void setPosition(float x, float y);
	virtual void setSize(float w, float h);
	virtual void setShape(ofRectangle r);
	virtual void setShape(float x, float y, float w, float h);

	ofPoint getPosition();
	ofRectangle getShape();
	float getWidth();
	float getHeight();

	ofColor getHeaderBackgroundColor();
	ofColor getBackgroundColor();
	ofColor getBorderColor();
	ofColor getTextColor();
	ofColor getFillColor();

	void setHeaderBackgroundColor(const ofColor & color);
	void setBackgroundColor(const ofColor & color);
	void setBorderColor(const ofColor & color);
	void setTextColor(const ofColor & color);
	void setFillColor(const ofColor & color);
    void setClickedColor(const ofColor & color);

	static void setDefaultHeaderBackgroundColor(const ofColor & color);
	static void setDefaultBackgroundColor(const ofColor & color);
	static void setDefaultBorderColor(const ofColor & color);
	static void setDefaultTextColor(const ofColor & color);
	static void setDefaultFillColor(const ofColor & color);
    static void setDefaultClickedColor(const ofColor & color);

	static void setDefaultTextPadding(int padding);
	static void setDefaultWidth(int width);
	static void setDefaultHeight(int height);

	virtual ofAbstractParameter & getParameter() = 0;
	static void loadFont(string filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, int dpi=0);
	static void setUseTTF(bool bUseTTF);
    
    //custom
    void setDraggable(bool drag_) { draggable = drag_; };
    virtual void setClicked(bool click_) { clicked = click_; };
    virtual bool isClicked() { return clicked; };
    virtual void setMidiLearnActive(bool active_);
    virtual void setMidiControlActive(int control_) { midiControlActive = control_; };
    virtual void resetMidiControlActive(int control_) { if (selectedForMidiControl == control_) selectedForMidiControl = -1; };
    virtual void setEditLeftAudioInActive(bool active_, int node_);
    virtual void setEditRightAudioInActive(bool active_, int node_);
    virtual void setEditOSCActive(bool active_, int node_);
    virtual void setSelectedForLeftAudio(bool active_, int node_, string name_);
    virtual void setSelectedForRightAudio(bool active_, int node_, string name_);
    virtual void setSelectedForOSC(bool active_, int node_, string name_);
    virtual void setSelectedForMIDI(bool active_, int control_, string name_);
    virtual vector<string> getAttributesClicked();
//    virtual vector<string> getAttributesForAudioIn();
//    virtual vector<string> getAttributesForMidiLearn();
    bool         getSelectedForOSC() { return selectedForOSC; };
    virtual ofxBaseGui* find(string name);

    ofEvent<ofxOSCGuiEvent> addOrRemoveOSCInputBaseGui;
    
    void registerMouseEvents();
    void registerKeyEvents();
    void unregisterMouseEvents();
    void unregisterKeyEvents();

	virtual bool mouseMoved(ofMouseEventArgs & args) = 0;
	virtual bool mousePressed(ofMouseEventArgs & args) = 0;
	virtual bool mouseDragged(ofMouseEventArgs & args) = 0;
	virtual bool mouseReleased(ofMouseEventArgs & args) = 0;
    
    virtual bool keyPressed(ofKeyEventArgs & args) = 0;
    virtual bool keyReleased(ofKeyEventArgs & args) = 0;
    
    
    
protected:
	virtual void render()=0;
	bool isGuiDrawing();
	virtual bool setValue(float mx, float my, bool bCheckBounds) = 0;
	void bindFontTexture();
	void unbindFontTexture();
	ofMesh & getTextMesh(const string & text, float x, float y);
	ofRectangle getTextBoundingBox(const string & text,float x, float y);

	ofRectangle b;
	static ofTrueTypeFont font;
	static bool fontLoaded;
	static bool useTTF;
	ofPtr<ofBaseFileSerializer> serializer;

	static ofColor headerBackgroundColor;
	static ofColor backgroundColor;
	static ofColor borderColor;
	static ofColor textColor;
	static ofColor fillColor;
    static ofColor clickedColor;
    static ofColor midiLearnColor;
    static ofColor leftAudioInColor;
    static ofColor rightAudioInColor;
    static ofColor oscColor;

	ofColor thisHeaderBackgroundColor;
	ofColor thisBackgroundColor;
	ofColor thisBorderColor;
	ofColor thisTextColor;
	ofColor thisFillColor;
    ofColor thisClickedColor;
    ofColor thisMidiLearnColor;
    ofColor thisLeftAudioInColor;
    ofColor thisRightAudioInColor;
    ofColor thisOSCColor;

	static int textPadding;
	static int defaultWidth;
	static int defaultHeight;

	static string saveStencilToHex(ofImage& img);
	static void loadStencilFromHex(ofImage& img, unsigned char* data) ;

	virtual void generateDraw(){};
    
    //custom
    bool draggable;
    bool clicked;
    bool selectedForLeftAudio;
    bool selectedForRightAudio;
    int  selectedForNodeId;
    bool selectedForOSC;
    int  selectedForMidiControl;
    bool selectedForMIDI;
    int  midiControlActive;
    bool midiLearnActive;
    bool editLeftAudioInActive;
    bool editRightAudioInActive;
    int  editNodeId;
    bool editOSCActive;
    bool commandPressed;
    ofPath border;

private:
	unsigned long currentFrame;
    bool bRegisteredForMouseEvents;
    bool bRegisteredForKeyEvents;
}; 
