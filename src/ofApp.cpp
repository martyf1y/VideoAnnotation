#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255,255,255);
    ofSetVerticalSync(true);
    frameByframe = false;
    vidSelected = false;
    
    barWidth = ofGetWidth();
    theTexts = new textObject*[1000];
    font.loadFont("verdana.ttf", 14);
    font.setLetterSpacing(1.037);
    
    //This will be to select the video
    dir.listDir("movies/");
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    txtDir.listDir("texts/");
    txtDir.sort();
    currentImage = 0;
    
    createNew = true;
    useCommKeys = true;
    textCount = 0;
    displayError = false;
    doIWantToQuit = false;
    doIWantToSave = false;
    doIWantToLoad = false;
    yesIDo  = false; //This is to check if the person wants to quit
    ofSetEscapeQuitsApp(false);
    volume = 1;
    attempt = false;

}

void ofApp::vidSetup()
{
    if(doIWantToLoad)
    {
        ofDrawBitmapStringHighlight("HOWDY, There is an annotation file with the same name. Do you want to load it?", ofGetWidth()/2 - 300, ofGetHeight()/2.5);
       
            ofDrawBitmapStringHighlight("Load Annotations", ofGetWidth()/2 - 100, ofGetHeight()/2, ofColor::black, (yesIDo?ofColor::red:ofColor::white));
            ofDrawBitmapStringHighlight("Don't Load Annotations", ofGetWidth()/2 + 100, ofGetHeight()/2, ofColor::black, (yesIDo?ofColor::white:ofColor::red));
    }
    
    else if (dir.size() > 0){
        ofSetColor(ofColor::white);
        //images[currentImage].draw(300,50);
        
        ofSetColor(ofColor::gray);
        string pathInfo = dir.getName(currentImage) + " " + dir.getPath(currentImage) + "\n\n" +
        "Please select the film you want to annotate \n\n" +
        "Many thanks to Matthew Martin for the annotation software";
        ofDrawBitmapStringHighlight(pathInfo, 300, ofGetHeight()/2);
    }
    
    ofSetColor(ofColor::gray);
    for(int i = 0; i < (int)dir.size(); i++){
        bool thisOne = false;
        if(i == currentImage) {thisOne = true;}
        
        string fileInfo = "file " + ofToString(i + 1) + " = " + dir.getName(i);
        ofDrawBitmapStringHighlight(fileInfo, 50,i * 20 + 50, ofColor::black, (thisOne?ofColor::red:ofColor::white));
    }
    if(displayError)
    {
        string error = "Sorry couldn't load video/txt please try another video/txt";
        ofDrawBitmapStringHighlight(error, 300, ofGetHeight()/1.5, ofColor::black, ofColor::red);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if(!vidSelected)
    {
        vidSetup();//Sets up the video
        video.setLoopState(OF_LOOP_NORMAL);
        video.update();
        //cout <<   video.isLoaded() << endl;
        if(attempt)
        {
            if(video.isLoaded())
            {
                displayError = false;
                attempt = false;
                // vid1.setSynchronousSeeking(false);
                string txtName = dir.getName(currentImage);
                txtName = txtName.substr(0, txtName.size() - 4);
                txtName = txtName.append(".txt");//This is to check that there is a txt file
                
                for(int i = 0; i < (int)txtDir.size(); i++)
                {
                    if(txtDir.getName(i) == txtName)
                    {
                        cout << "AHA!" << endl;
                        doIWantToLoad = true;
                    }
                }
                if(!doIWantToLoad)
                {
                    video.play();
                    
                    vidSelected = true;
                }
            }
            else
            {
                displayError = true;
            }
            
        }
    }
    if(vidSelected)
    {
        //vid1.update();
       // cout << "AHA3!" << endl;
        video.update();
       // cout << "AHA2!" << endl;
        currFrame = video.getCurrentFrame();
        int checkAllTexts = 0;
        for(int i = 0; i < textCount; i++)
        {
            if(currFrame - theTexts[i]->startTime >= 0)//This makes sure it doesnt update outside the limit
            {
                theTexts[i]->update(mouseX, mouseY, currFrame);
            }
            
            if(theTexts[i]->isSelected)
            {
                checkAllTexts ++;
            }
            if(checkAllTexts == 0)
            {useCommKeys = true;}
            else {useCommKeys = false;}
        }
      
        int pauseTime;
        if(doIWantToQuit)
        {
            startTime = ofGetElapsedTimeMillis() - timer;
        }
        else
        {
             timer = ofGetElapsedTimeMillis() - startTime;
        }
        
        if(timer >= 60000)//every minute
        {
            video.setPaused(true);
            ofDrawBitmapStringHighlight("SAVING", 0, ofGetHeight()-15, ofColor::black, ofColor::red);
            letsSave();
            video.setPaused(false);
            startTime = ofGetElapsedTimeMillis();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(!vidSelected)
    {
        vidSetup();
    }
    if(vidSelected)
    {
        ofSetHexColor(0xFFFFFF);
        
        if (video.isLoaded())
        {
            video.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
        
        
        // the background to the progress bar
        ofSetColor(100);
       //ofRect(0, ofGetHeight() - 20, barWidth, 15);
        
        // get the percantage of the timer
        float pct = ofMap(video.getCurrentFrame(), 0.0, video.getTotalNumFrames(), 0.0, 1.0, true);
        ofSetHexColor(0xf02589);
        ofRect(0, ofGetHeight() - 25, barWidth*pct, 15);
        
        // Draw a timeline at the bottom of the screen.
        ofNoFill();
        ofSetColor(255);
        ofRect(0, ofGetHeight() -35, ofGetWidth(), 35);
        float playheadX = video.getPosition() * ofGetWidth();
        ofLine(playheadX, ofGetHeight() - 50, playheadX, ofGetHeight());
        
        ofDrawBitmapStringHighlight(ofToString(video.getCurrentFrame()) + " / " + ofToString(video.getTotalNumFrames()), playheadX + 10, ofGetHeight() - 40);
        
        ofFill();
        ofDrawBitmapStringHighlight("Rate: " + ofToString(video.getSpeed(), 2) + "\n" +
                                   // "Time: " + ofToString(video.getCurrentTime(), 3) + " / " + ofToString(video.getDuration(), 3) + "\n" +
                                    "Frames: " + ofToString(currFrame) + " / " + ofToString(video.getTotalNumFrames()) + "\n" +
                                    "Position: " + ofToString(video.getPosition() * 100, 1) + "% \n" +
                                    "Volume: " + ofToString(video.getVolume(), 2) + "\n" +
                                    "Total Texts: " + ofToString(textCount) + " / 1000 \n " +
                                    "\n" +
                                    "TAB: Create text / End text \n" +
                                    "[a]/[s]: Play / Stop \n" +
                                    "[space]: Pause \n" +
                                    "[up]/[down]: Adjust Speed \n" +
                                    "[-]/[=]: Volume \n" +
                                    "Saving in: " + ofToString(60 - timer/1000) +  "\n" +
                                    "useCommKeys: " + (useCommKeys?"YES":"NO") + "\n",
                                    10, 20, ofColor(0,0,0, 130), ofColor::white);
               // ofDrawBitmapString("duration: " + ofToString(video.getPosition()*video.getDuration(),2) + "/"+ofToString(video.getDuration(),2),20,60);
       // ofDrawBitmapString("Total Texts: " + ofToString(textCount),20,100);
        
        if(video.getIsMovieDone()){
            ofSetHexColor(0xFF0000);
            ofDrawBitmapStringHighlight("end of movie",10,290);
        }
        
        
        for(int i = 0; i < textCount; i++)
        {
            if((currFrame <= theTexts[i]->endTime
                && currFrame >= theTexts[i]->startTime)
               || theTexts[i]->isSelected == true)
            {

                theTexts[i]->draw(font);
            }
            else if(theTexts[i]->isSelected == false)
            {
                theTexts[i]->textBox.localSelected = false;//STOP BEING SELECTED
            }
        }
    }
    
    if(doIWantToSave)
    {
       // void ofDrawBitmapStringHighlight(string text, int x, int y, const ofColor &background=ofColor::black, const ofColor &foreground=ofColor::white)
        
        ofDrawBitmapStringHighlight("Save Annotations", ofGetWidth()/2 - 100, ofGetHeight()/2, ofColor::black, (yesIDo?ofColor::red:ofColor::white));
       
        ofDrawBitmapStringHighlight("Don't Save Annotations", ofGetWidth()/2 + 100, ofGetHeight()/2, ofColor::black, (yesIDo?ofColor::white:ofColor::red));
    }
    
    else if(doIWantToQuit)
    {
            ofDrawBitmapStringHighlight("YES LET'S LEAVE", ofGetWidth()/2 - 100, ofGetHeight()/2, ofColor::black, (yesIDo?ofColor::red:ofColor::white));
            ofDrawBitmapStringHighlight("NOT YET", ofGetWidth()/2 + 100, ofGetHeight()/2, ofColor::black, (yesIDo?ofColor::white:ofColor::red));
    }
}

void ofApp::letsSave()
{
    //Gather all the objects and save them
    string textFileName = dir.getName(currentImage);
    textFileName = textFileName.substr(0, textFileName.size() -4);
    ofBuffer theTextFile; //Saves the file
    
    for(int i = 0; i < textCount; i++)
    {
        theTextFile.append(theTexts[i]->textBox.text + "~~<0>~~"); //Saves as name, put in unlikely character to separate types
        theTextFile.append(ofToString(theTexts[i]->startTime) + "START");//Saves start time
        theTextFile.append(ofToString(theTexts[i]->endTime) + "END"); //Saves end time
        int lastPosX = 0;// These will make sure the text doesn't go to 0
        int lastPosY = 0;
        for(int s = 0; s < theTexts[i]->posX.size(); s++)//This is to get all pos, posx and posy should be the same
        {
            
            if (theTexts[i]->posX[s] == 0 || theTexts[i]->posY[s] == 0)//If not selected stay where it is
            {
                //This will make sure the annotations that have no end stay where they are
                theTextFile.append(ofToString(lastPosX) + "&X");
                theTextFile.append(ofToString(lastPosY) + "&Y");
            }
            else
            {
                theTextFile.append(ofToString(theTexts[i]->posX[s]) + "&X");
                theTextFile.append(ofToString(theTexts[i]->posY[s]) + "&Y");
                lastPosX = theTexts[i]->posX[s];
                lastPosY = theTexts[i]->posY[s];
                
            }
            cout << "ALL X POS " << theTexts[i]->posX[s] << endl;
            cout << "ALL Y POS " << theTexts[i]->posY[s] << endl;
        }
        theTextFile.append("|<|>1<|>|");//This is the end of the object
    }
    
    ofFile newFile(ofToDataPath("texts/" + textFileName + ".txt"), ofFile::WriteOnly); //file doesn't exist yet
    newFile.create();
    bool fileWritten = ofBufferToFile("texts/" + textFileName + ".txt", theTextFile);
}
void ofApp::exit()
{
    
    if(vidSelected && doIWantToSave)
    {
        letsSave();
    }
    
    std::exit(1);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    if(vidSelected && useCommKeys)//allow commands when not typing
    {
        switch(key){
            case OF_KEY_UP:
                //vid1.previousFrame();
                video.setSpeed(video.getSpeed() + 0.05);
                break;
            case OF_KEY_DOWN:
                //vid1.nextFrame();
                video.setSpeed(video.getSpeed() - 0.05);
                break;
                
            case '0':
                video.firstFrame();
                break;
            case 'a':
                video.play();
                break;
            case 's':
                video.stop();
                break;
            case '-':
                volume--;
                video.setVolume(volume*.1);
                break;
            case '=':
                volume ++;
                video.setVolume(volume*.1);
                break;
            case ' ':
                if (video.isPaused()) video.setPaused(false);
                else video.setPaused(true);
                break;

        }
    }
    
    switch(key)
    {
        case OF_KEY_ESC:
            doIWantToQuit = true;
            if(vidSelected)
            {
                video.setPaused(true);
            }
            //vid1.nextFrame();
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch(key)
    {
        case OF_KEY_UP:
            if(!vidSelected)
            {
                if (dir.size() > 0){
                    currentImage--;
                    currentImage %= dir.size();
                }
            }
            break;
        case OF_KEY_DOWN:
            if(!vidSelected)
            {
                if (dir.size() > 0){
                    currentImage++;
                    currentImage %= dir.size();
                }
            }
            break;
        case OF_KEY_LEFT:
            if(doIWantToSave || doIWantToQuit || doIWantToLoad)
            {
                yesIDo = !yesIDo;
            }
            break;
        case OF_KEY_RIGHT:
            if(doIWantToSave || doIWantToQuit || doIWantToLoad)
            {
                yesIDo = !yesIDo;
            }
            break;
        case OF_KEY_RETURN:
            if(doIWantToSave)
            {
                if(yesIDo)
                {
                    // Exit with doIWantToSave as true
                }
                else
                {
                    doIWantToSave = false;
                }
                exit();
            }
            else if(doIWantToQuit)
            {
                if(yesIDo)
                {
                    if(vidSelected)
                    {
                        doIWantToSave = true;
                    }
                    else
                    {
                        std::exit(1);
                    }
                }
                else
                {
                    doIWantToQuit = false;
                    if(vidSelected)
                    {
                        video.setPaused(false);
                    }
                }
            }
            else if (vidSelected && !useCommKeys)//Alternative to pausing
                     {
                         if (video.isPaused()) video.setPaused(false);
                         else video.setPaused(true);
                     }
            else if(doIWantToLoad && !vidSelected)
            {
                if(yesIDo)
                {
                    try
                    {
                        string txtLoad = dir.getName(currentImage);
                        txtLoad = txtLoad.substr(0, txtLoad.size() - 4);
                        txtLoad = txtLoad.append(".txt");//This is to check that there is a txt file
                        ofBuffer loadFile = ofBufferFromFile("texts/" + txtLoad);
                        vector<string> objectInfo;//This will contain all answers in an array
                        cout << "Or Here?" << endl;
                        objectInfo = ofSplitString(loadFile, "|<|>1<|>|");
                        for(int i = 0; i < objectInfo.size() - 1; i++)//Creating the objects
                        {
                            cout << "Here? " << i << endl;
                            
                            theTexts[textCount] = new textObject(currFrame, video.getTotalNumFrames());
                            // Remember once text is created it becomes selected
                            //We want the text that is selected
                            vector<string> objectData;//This will contain all answers in an array
                            objectData = ofSplitString(objectInfo[i], "~~<0>~~");
                            theTexts[i]->textBox.text = objectData[0];//Here is the content
                            
                            objectData = ofSplitString(objectData[1], "START");
                            theTexts[i]->startTime = ofToInt(objectData[0]);//Here is the start time
                            
                            objectData = ofSplitString(objectData[1], "END");
                            theTexts[i]->endTime = ofToInt(objectData[0]);//Here is the end time
                            
                            vector<string> objectPos;//This will contain all positions in an array
                            objectPos = ofSplitString(objectData[1], "&Y");
                            for(int s = 0; s < objectPos.size() - 1; s++)//Creating the objects
                            {
                                vector<string> objectXY;//This will contain just XY in an array
                                objectXY = ofSplitString(objectPos[s], "&X");
                                theTexts[i]->posX[s] = ofToInt(objectXY[0]);//Here is the x value
                                theTexts[i]->posY[s] = ofToInt(objectXY[1]);//Here is the y value
                            }
                            
                            theTexts[i]->newText = false;
                            theTexts[i]->isSelected = false;
                            theTexts[i]->textBox.localSelected = false;//This is how I stop things from drawing on others
                            textCount ++;
                            video.play();
                            vidSelected = true;
                        }
                        
                    }
                    catch (exception &e)
                    {
                        displayError = true;
                    }
                }
                else
                {
                    video.play();
                    vidSelected = true;
                }
            }
            else if(!vidSelected && !doIWantToLoad)
            {
                video.setPixelFormat(OF_PIXELS_RGB);
                // Texture only is fastest, but no pixel access allowed.
                //ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
                //cout << "Arrr!" << endl;
                video.loadMovie(dir.getPath(currentImage));
                video.update();
                attempt = true;
            }
            
            break;
        case OF_KEY_TAB:
            if(vidSelected)
            {
                for(int i = 0; i < textCount; i++)
                {
                    int countSelect = 0;
                    if(theTexts[i]->isSelected)//This makes sure you cant have selected and create new text
                    {
                        createNew = false;
                        //We wend the text that is selected
                        if(video.isPaused())
                        {
                            theTexts[i]->endTime = currFrame + 62;//Make sure that when the the user places text while paused it is stil seen
                            theTexts[i]->textBox.localSelected = false;//This is how I stop things from drawing on others
                        }
                        else
                        {
                            theTexts[i]->endTime = currFrame;
                        }
                        theTexts[i]->posX.erase(theTexts[i]->posX.begin() + currFrame - theTexts[i]->startTime, theTexts[i]->posX.begin() + theTexts[i]->posX.size());
                        theTexts[i]->posY.erase(theTexts[i]->posY.begin() + currFrame - theTexts[i]->startTime, theTexts[i]->posY.begin() + theTexts[i]->posY.size());
                        
                        theTexts[i]->newText = false;
                        theTexts[i]->isSelected = false;
                    }
                }
                if(createNew && video.getSpeed() >= 0)
                {
                    cout << "Vid amount " << video.getTotalNumFrames() << endl;
                    cout << "text frame " << currFrame << endl;
                    theTexts[textCount] = new textObject(currFrame, video.getTotalNumFrames());
                    // Remember once tect is created it becomes selected
                    textCount ++;
                }
                else
                {
                    createNew = true;// Put it back in to allow new text
                }
            }
            break;
            
            
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(vidSelected)
    {
        if(mouseX > 0 && mouseX < barWidth &&
           mouseY > ofGetHeight() -50 && mouseY < ofGetHeight())
        {
            //video.setFrame(ofMap(mouseX, 0, barWidth, 0, video.getTotalNumFrames()));
            video.setPosition(ofMap(mouseX, 0.00, barWidth, 0.00, 1.00));
            
        }
    }
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if(vidSelected)
    {
        for(int i = 0; i < textCount; i++)
        {
            if(currFrame <= theTexts[i]->endTime
               && currFrame >= theTexts[i]->startTime)
            {
                theTexts[i]->mouseReleased(mouseX, mouseY, button);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
