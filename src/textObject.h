//
//  textObject.h
//  videoAnnotater
//
//  Created by Marty on 25/07/14.
//
//

#ifndef videoAnnotater_textObject_h
#define videoAnnotater_textObject_h

#include <iostream>
#include "ofMain.h"
#include "textInput.h"


class textObject {
    
public:
    textObject(int _startTime, int _totalFrame);
    void update(int mX, int mY, int currFrame);
    void draw(ofTrueTypeFont font);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

	//void keyPressed  (int key);
    
    //These functions help find the boundaries of the objects
    
    
    bool amIHere;// This will be to show the text at the right time
    bool newText; //This is to check if the text is new
    bool isSelected; //When the mouse has clicked on it
    
    ofTrueTypeFont verdana14;
    textInput textBox;
	
    /////
    //for the purpose of this example
    vector<string> textEntries;
    void addText(string& s);
    void drawEntries();
    //
    /////Timer
    int endTime;
    int startTime;
    int tPosX;
    int tPosY;
    int lastFrame;
    int totalFrame;
    
    //Record position
    vector<int> posX;
    vector<int> posY;
private:
};

#endif
