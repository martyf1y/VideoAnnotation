//
//  textInput.cpp
//  textInput
//
//  Created by Elliot Woods on 12/09/2011.
//  Copyright 2011 Kimchi and Chips.
//	MIT license
//	http://www.opensource.org/licenses/mit-license.php
//

#include "textInput.h"

textInput::textInput() {
	text="";
	position=0;
	cursorx=0;
	cursory=0;
    localSelected = true;
    debugCursor = 0;
}

void textInput::init() {
    ofAddListener(ofEvents().keyPressed, this, &textInput::keyPressedEvent);
    
}

void textInput::draw(int tPoX, int tPosY, ofTrueTypeFont font, bool isSelected)
const {
    //draw text
    (isSelected?ofSetColor(0, 0, 0, 180):ofSetColor(255, 255, 255, 180));
    
    ofRect(tPoX - (text.length()*8+10)/2, tPosY - 15, text.length()*8+10, 15);
    ofSetColor(255, 0, 0);

	ofDrawBitmapString(text, tPoX - (text.length()*8)/2, tPosY-3);

    ofSetColor(255, 255, 255, 180);
	//draw cursor line
	ofPushStyle();
	float timeFrac = 0.5 * sin(6.0f * ofGetElapsedTimef()) + 0.5;
	
	ofColor col = ofGetStyle().color;

    if(isSelected)
    {
    ofSetColor(255, 255, 255, col.b * timeFrac);
	ofSetLineWidth(2.0f);
	ofLine((cursorx*8)/2 + tPoX, 13.7*cursory + tPosY, (cursorx*8)/2 + tPoX, 13.7*cursory + tPosY - 15);
	}
    ofPopStyle();
}

void textInput::keyPressed(int key) {
    
    //add charachter (non unicode sorry!)
	if (key >=32 && key <=126) {
		text.insert(text.begin()+position, key);
		position++;
	}
	
	//if (key==OF_KEY_RETURN) {
	//	ofNotifyEvent(evtEnter, text, this);
	//	if (evtEnter.empty()) {
	//		text.insert(text.begin()+position, '\n');
	//		position++;
	//	}
//	}
	
	if (key==OF_KEY_BACKSPACE) {
		if (position>0) {
			text.erase(text.begin()+position-1);
			--position;
		}
	}
	
	if (key==OF_KEY_DEL) {
		if (text.size() > position) {
			text.erase(text.begin()+position);
		}
	}
	
	if (key==OF_KEY_LEFT){
		if (position>0)
        {
			--position;
            debugCursor --;
        }
    }
	
	if (key==OF_KEY_RIGHT){
		if (position<text.size()+1)
        {
			++position;
            debugCursor ++;
        }
	}
    
    
	//for multiline:
	cursorx = cursory = 0;
	for (int i=0; i<position; ++i)
    {
		if (*(text.begin()+i) == '\n')
        {
			++cursory;
			cursorx = 0;
		}
        else
        {
			cursorx++;
		}
	}
    cursorx += debugCursor;
    
}

void textInput::clear() {
	text.clear();
	position=0;
}

void textInput::keyPressedEvent(ofKeyEventArgs &a) {
    if(localSelected)
    {
	keyPressed(a.key);
    }
    
}