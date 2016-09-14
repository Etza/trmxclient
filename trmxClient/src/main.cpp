#include "ofMain.h"
#include "ofApp.h"

//========================================================================




int main(){

// this sets colour processing to be done on GPU when OF_PIXELS_NATIVE - otherwise will be black and white - 
// however it also cuts 200 px off the right hand side of the screen ! comment for b&w or set to RGB

	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
	ofCreateWindow(settings);
	return ofRunApp(new ofApp);
//}

//int main( ){

	// setup the GL context
	ofSetupOpenGL(1280, 720, OF_WINDOW);	
	// hide cursor
	ofHideCursor();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
