//
//  textObject.cpp
//  videoAnnotater
//
//  Created by Marty on 25/07/14.
//
//

#include "textObject.h"

//--------------------------------------------------------------
textObject::textObject(int _startTime, int _totalFrame){
    startTime = _startTime;
    endTime = _totalFrame - 1; //This makes sure it only plays to the end of vid
    totalFrame = _totalFrame;
	//this sets up the key pressed event
	//we could also do this ourselves manually
    
	textBox.init();
	
	ofAddListener(textBox.evtEnter, this, &textObject::addText);
    newText = true;
    isSelected = true;
    lastFrame = 0;
    posX.resize(totalFrame - startTime);//Initial size
    posY.resize(totalFrame - startTime);//Initial size
    std::fill(posX.begin(), posX.end(), 0);
    std::fill(posY.begin(), posY.end(), 0);

}

//--------------------------------------------------------------
void textObject::update(int mX, int mY, int currFrame){
    
    if(lastFrame != currFrame)//makes sure things only happen when a frame passes
    {
        
    if(isSelected)
    {
        if(currFrame > endTime )//Makes sure the selected doesn't go away after end frame
        {
            if(endTime < totalFrame - 105) //Makes sure it doesn't go over the limit
            {
                endTime = currFrame + 100;
            }
            else
            {
                endTime = totalFrame - 1;
            }
        }
        if(currFrame >= totalFrame - 5)//Bit of leeway to make sure the item is deselected
        {
            isSelected = false;
            newText = false;
        }
    }
    else if (posX[currFrame - startTime - 1] == 0 || posY[currFrame - startTime - 1] == 0)//If not selected stay where it is
    {
        //Do nothing to the position
        //This is just to make sure the text stays where it is otherwise
    }
    else //If it has already gone through once move it to the old spot
    {
        tPosX = posX[currFrame - startTime - 1];
        tPosY = posY[currFrame - startTime - 1];

    }
        
        ofDrawBitmapStringHighlight("drawing2", 300, 400);

    if((currFrame < endTime
        && currFrame > startTime)
        || isSelected == true)
        {
            posX[currFrame - startTime - 1] = tPosX;
            posY[currFrame - startTime - 1] = tPosY;
          //  cout << "Vector pos" << currFrame - startTime  << endl;
          //  cout << "Saved X " << posX[currFrame - startTime - 1] << " AT " << tPosX << endl;
          //  cout << "Saved Y " << posY[currFrame - startTime - 1] << " AT " << tPosY << endl;

        }
        
        lastFrame = currFrame;

  }
  //
        //cout << "Updating " << tPosX << endl;
//if(lastFrame == currFrame)
//{
        if(isSelected)
        {
            cout << "yes" << endl;
            tPosX = mX; //When selected follow mouse
            tPosY = mY;
        }
  //  }
    
}

//--------------------------------------------------------------
void textObject::draw(ofTrueTypeFont font){
    
    //cout << tPosX << endl;
    //I've left the draw call as manual
	// but this could also be event driven
	// like textInput::keyPressed
	ofPushMatrix();
	//ofScale(2,2);

	textBox.draw(tPosX, tPosY, font, isSelected);
    
	ofPopMatrix();
	
	drawEntries();
}

void textObject::mouseReleased(int mX, int mY, int button)
{
    if(isSelected == true)
    {
        isSelected = false;
    }
    else if(mX > tPosX - (textBox.cursorx*8+10)/2
            && mX < (tPosX - (textBox.cursorx*8+10)/2) + textBox.cursorx*8+10
            && mY > tPosY - 15 && mY < tPosY
            && !isSelected)
    {
        isSelected = true;
    }
    textBox.localSelected = isSelected;//This is how I stop things from drawing on others
}
//--------------------------------------------------------------
/*
 void testApp::keyPressed(int key){
 //if we don't setup the event using
 //textInput::init(), then we can
 //send key events ourselves here
 //manually. like so:
 
 textBox.keyPressed(key);
 }*/

//--------------------------------------------------------------
void textObject::addText(string &s) {
	textEntries.push_back(s);
    
	textBox.clear();
}
//--------------------------------------------------------------
void textObject::drawEntries() {
   
	for (int i=0; i<textEntries.size(); ++i) {
		ofDrawBitmapString(textEntries[i], 50, 100+10*i);
         
	}
}
