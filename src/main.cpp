#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofSetupOpenGL(1920*.7,1080*.7, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}

// The program will find the files in the movies folder and allow you to select one.
// Make sure to choose a video.
// If there is a txt file with the same name it will ask if you want to load already made annotations.
// Press TAB to create  annotaion. Press again to END it.
// It should type most characters except the ones above the numbers.
// Mouse click with the annotation selected to place it. Mouse click to SELECT it, while selected it follows the mouse and will record that movement for the video playback.
// It will stay in its position until you END it or the video runs out.
// ENTER pauses video. You can still create annotations while paused.
// UP DOWN ARROWS move the video back or forth 10 frames.
// Mouse click on the time bar to skip to a certain part.
// Only 1000 annotations can be made.
// You cannot delete an annotation once it has been made.
// Press ESC to exit, you will be asked if you want to save the annotations as a txt file. If there was a txt file already there it will overide it.

// If you find any problems or have improved it in some way please send me a message with the code to mjmartin.work@gmail.com.
// Enjoy!