//
//  ofxOSCGuiEvent.h
//  nimp
//
//  Created by Mili Garicoits on 3/22/16.
//
//

#ifndef ofxOSCGuiEvent_h
#define ofxOSCGuiEvent_h

class ofxOSCGuiEvent : ofEventArgs {
    
public:
    int    oscNodeId;
    int    nodeId;
    string paramName;
    bool   add;
};

#endif /* ofxOSCGuiEvent_h */
