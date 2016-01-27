#pragma once

#include "ofxToggle.h"
#include "ofParameter.h"

class ofxButton : public ofxToggle{
	friend class ofPanel;
	
public:
	ofxButton();
	~ofxButton();
	ofxButton* setup(string toggleName, float width = defaultWidth, float height = defaultHeight);

	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
    
    virtual void keyPressed(ofKeyEventArgs &args){return false;}
    virtual void keyReleased(ofKeyEventArgs &args){return false;}

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		ofAddListener(triggerEvent,listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		ofRemoveListener(triggerEvent,listener,method);
	}

private:
	ofEvent<void> triggerEvent;
	void valueChanged(bool & v);

};
