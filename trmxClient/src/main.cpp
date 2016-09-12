#include "ofMain.h"
#include "ofApp.h"

//========================================================================




int main(){

// this sets colour processing to be done on GPU - otherwise will be black and white

	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
	ofCreateWindow(settings);
	return ofRunApp(settings);
//}

//int main( ){

	// setup the GL context
	ofSetupOpenGL(1280, 780, OF_WINDOW);	
	// hide cursor
	ofHideCursor();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
