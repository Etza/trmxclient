#include "ofMain.h"
#include "ofApp.h"

//========================================================================




int main(){

// using programmable renderer GLES2 with OF_PIXELS_NATIVE sets colour processing to be done on GPU - otherwise will be black and white
    
	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
    settings.width = 1280;
    settings.height = 720;
   
    ofCreateWindow(settings);
    ofHideCursor();
	ofRunApp(new ofApp);

	// setup the GL context
	//ofSetupOpenGL(1280, 720, OF_WINDOW);
	// hide cursor
	//ofHideCursor();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//ofRunApp( new ofApp());

}
