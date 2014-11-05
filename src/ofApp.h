#pragma once

#include "ofMain.h"
#include "ofxAVFVideoPlayer.h"
#include "textObject.h"

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    void exit();
    void letsSave();
    void vidSetup();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //  ofQTKitPlayer vid1;
    ofxAVFVideoPlayer video;
    
    int volume;
    bool                frameByframe;
    bool paused;
    float barWidth;
    ofTrueTypeFont	font;
    int currFrame;
    
    //Text object
    textObject** theTexts;
    int textCount;
    bool createNew;
    bool useCommKeys;

    
    //Select vid
    bool vidSelected;
    // we will have a dynamic number of images, based on the content of a directory:
    ofDirectory dir;
    ofDirectory txtDir;//This is to see if there is a txt already there;
    bool displayError;
    
    int currentImage;
    
    //Saving content
    string content;
    vector<string> timeStartEnd;
    vector<string> sPosX; //saves the positions
    vector<string> sPosY;
    bool doIWantToSave;
    bool doIWantToQuit;
    bool yesIDo;
    
    //Loading content
    bool doIWantToLoad;
    bool attempt;
    
    //timer
    float timer;
    float startTime; // store when we start time timer
};

