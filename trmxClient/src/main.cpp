#include "ofMain.h"
#include "ofApp.h"

//========================================================================


// this sets colour processing to be done on GPU - otherwise will be black and white

int main(){
	
	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
	ofCreateWindow(settings);
	return ofRunApp(settings);
}


int main( ){

	ofSetupOpenGL(1280, 780, OF_WINDOW);			// <-------- setup the GL context
	ofHideCursor();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
