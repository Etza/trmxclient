// trmxMediaFrame.cpp

#include "trmxMediaFrame.h"

trmxMediaFrame::trmxMediaFrame(){

    setPosition(0,0);
    setColor(255,255,255);
    //Default Alpha: 0, Tranaparent
    setAlpha(0);

    isImage = false;
    isVideo = false;

	// By default, 
	// Scale media to screen size
    setNativeSize(false);
    
	// Loop videos
	setLoopVideo(true);
	setMinFadeToAlpha(0);
	setMaxFadeToAlpha(255);

    //activeFading = false;
    targetAlpha = 0;
    millisPerAlphaValue = 0;
    millisecondsToFade = 0;
    startTime = ofGetElapsedTimeMillis();
    switchTime = 0;
    
    videoPlayer = ofVideoPlayer();
    videoPlayer.setPixelFormat(OF_PIXELS_NATIVE);
}

void trmxMediaFrame::update(){
	
	if(isVideo){
		videoPlayer.update();
	}
}

void trmxMediaFrame::draw(){

    ofPushMatrix();
    alphaFade();
	ofSetColor(red, green, blue, alpha);

	if(isImage){
		if(nativeSize){
			image.draw(xPos, yPos);
		}else{
			image.draw(xPos, yPos, ofGetWidth(), ofGetHeight());
		}
	}else if(isVideo){
		if(nativeSize){
			videoPlayer.draw(xPos, yPos);
		}else{
			videoPlayer.draw(xPos, yPos, ofGetWidth(), ofGetHeight());
		}
	}
	ofPopMatrix();
}

// Media Handling
void trmxMediaFrame::loadMedia(string filename){

	filePath = ofToDataPath(filename, true);

	// Check if the file exists
	ofFile file;
	file.open(filePath);

	if(file.exists()){

		// If there is a video playing, stop it.
		if(isVideo){
			stop();
		}

		// Reset media flags
		isImage = false;
		isVideo = false;

		// Specify accepted media extensions
		std::vector<std::string> imageExt;
		imageExt.push_back("jpg");
		imageExt.push_back("png");

		std::vector<std::string> videoExt;
		videoExt.push_back("mov");
		videoExt.push_back("mp4");

		// Parse the filename.
		string fileExt = filename.substr(filename.find_last_of(".") + 1);
		ofLog(OF_LOG_NOTICE, fileExt);

		// Is it an accepted file extension?
		// Determine the media type by extension

		// Is it an image?
		for (int i = 0; i < (int)imageExt.size(); ++i){
			//ofLog(OF_LOG_NOTICE, "fileExt: " + fileExt);
			//ofLog(OF_LOG_NOTICE, "imageExt[i]: " + imageExt[i]);
			if( fileExt == imageExt[i] ){
				isImage = true;
			}
		}

		// Is it a video?
		for (int i = 0; i < (int)videoExt.size(); ++i){
			//ofLog(OF_LOG_NOTICE, "fileExt: " + fileExt);
			//ofLog(OF_LOG_NOTICE, "videoExt[i]: " + videoExt[i]);
			if( fileExt == videoExt[i] ){
				isVideo = true;
			}
		}

		if(isImage){
			ofLog(OF_LOG_NOTICE, "is image");
			loadImage();
		}else if(isVideo){
			ofLog(OF_LOG_NOTICE, "is video");
			loadVideo();
		}
	}
}
void trmxMediaFrame::loadImage(){
    image = ofImage();
    image.loadImage(filePath);
}
void trmxMediaFrame::loadVideo(){
	videoPlayer.loadMovie(filePath);
}
bool trmxMediaFrame::isMediaImage(){
	return isImage;
}
bool trmxMediaFrame::isMediaVideo(){
	return isVideo;
}
void trmxMediaFrame::setLoopVideo(bool lV){
   	loopVideo = lV;
}
void trmxMediaFrame::setNativeSize(bool nS){
   	nativeSize = nS;
}


// Video Controls
void trmxMediaFrame::play(){
	if(isVideo){
		videoPlayer.play();
		//Must call loop state after play.
		if(loopVideo){
			videoPlayer.setLoopState(OF_LOOP_NORMAL);
		}
	}
}
void trmxMediaFrame::pause(){
	if(isVideo){
		videoPlayer.setPaused(!videoPlayer.isPaused());
	}
}
void trmxMediaFrame::stop(){
	if(isVideo){
		videoPlayer.stop();
	}
}
void trmxMediaFrame::nextFrame(){
	if(isVideo && !videoPlayer.isPlaying()){
		videoPlayer.nextFrame();
	}
}
void trmxMediaFrame::previousFrame(){
	if(isVideo && !videoPlayer.isPlaying()){
		videoPlayer.previousFrame();
	}
}
void trmxMediaFrame::setFrame(int frame){
	if(isVideo){

		bool unpauseAfter =  false;

		// If it is playing, pause it.
		if(videoPlayer.isPlaying()){
			unpauseAfter =  true;
			pause();
		}

		videoPlayer.setFrame(frame);

		// Unpause, if it was playing when we started
		if(unpauseAfter){
			pause();
		}
	}
}
void trmxMediaFrame::setSpeed(float speed){


	if(isVideo){

		bool unpauseAfter =  false;

		// If it is playing, pause it.
		if(videoPlayer.isPlaying()){
			unpauseAfter =  true;
			pause();
		}

		//1 = normal, 2 = 2x as fast, 0 = stopped, -1 = backwards, etc;
		videoPlayer.setSpeed(speed);

		// Unpause, if it was playing when we started
		if(unpauseAfter){
			pause();
		}
	}
}
void trmxMediaFrame::setVolume(float volume){
	if(isVideo){
		videoPlayer.setVolume(volume);
		
	}
}
int trmxMediaFrame::getTotalNumFrames(){
	if(isVideo){
		int numFrames = videoPlayer.getTotalNumFrames();
		ofLog(OF_LOG_NOTICE, "media frame sending numFrames %d", numFrames);
		return numFrames;
	}

	return 0;
}
int trmxMediaFrame::getCurrentFrame(){
	if(isVideo){
		return videoPlayer.getCurrentFrame();
	}

	return 0;	
}

// Draw Controls
void trmxMediaFrame::setPosition(int x, int y){
	xPos = x;
	yPos = y;
}

void trmxMediaFrame::setColor(int r, int g, int b){
	red = r;
    green = g;
    blue = b;
}
void trmxMediaFrame::setAlpha(int a){
   	alpha = a;
   	//Also set targetAlpha, to halt any faing that is happening
	targetAlpha = a;

	ofLog(OF_LOG_NOTICE, "__setAlpha");
   	ofLog(OF_LOG_NOTICE, "alpha " + ofToString(alpha));
	ofLog(OF_LOG_NOTICE, "targetAlpha " + ofToString(targetAlpha));
}
void trmxMediaFrame::setMinFadeToAlpha(int mA){
   	minFadeToAlpha = mA;
}
void trmxMediaFrame::setMaxFadeToAlpha(int mA){
   	maxFadeToAlpha = mA;
}
void trmxMediaFrame::alphaFade(){

	// ofLog(OF_LOG_NOTICE, "alpha " + ofToString(alpha));
	// ofLog(OF_LOG_NOTICE, "targetAlpha " + ofToString(targetAlpha));

	if(alpha != targetAlpha){

		ofLog(OF_LOG_NOTICE, "alpha " + ofToString(alpha));
		ofLog(OF_LOG_NOTICE, "targetAlpha " + ofToString(targetAlpha));

		float elapsedTimeMillis = ofGetElapsedTimeMillis() - startTime;

		// ofLog(OF_LOG_NOTICE, "elapsedTimeMillis " + ofToString(elapsedTimeMillis));
		// ofLog(OF_LOG_NOTICE, "switchTime " + ofToString(switchTime));

		// Is it time to change the alpha
	    if(elapsedTimeMillis >= switchTime && switchTime > 0) {

	        // Reset switch timer
	        switchTime = switchTime + millisPerAlphaValue;

	        if(alpha < targetAlpha && alpha < 255){
	            alpha++;

	        }else if(alpha > targetAlpha && alpha > 0){
	            alpha--;
	        }
	    }

	}

}
void trmxMediaFrame::fadeIn(int mTF){
	fadeTo(maxFadeToAlpha, mTF);
}
void trmxMediaFrame::fadeOut(int mTF){
	fadeTo(minFadeToAlpha, mTF);
}
void trmxMediaFrame::fadeTo(int tA, int mTF){
	calculateAlphaFade(tA, mTF);
}
void trmxMediaFrame::calculateAlphaFade(int tA, int mTF){

	targetAlpha = tA;
	millisecondsToFade = mTF;

	if(targetAlpha != alpha){

		ofLog(OF_LOG_NOTICE, "----------CALCULATE FADE-----------");
		int deltaAlpha = 0;

		// Fading up or down??
		if(targetAlpha > alpha){
			//fadeUp = true;
			// How many alpha values are we shifting?
			deltaAlpha = alpha - targetAlpha;
		}else if(targetAlpha < alpha){
			// How many alpha values are we shifting?
			deltaAlpha = targetAlpha - alpha;
			//fadeUp = false;
		}

		//ofLog(OF_LOG_NOTICE, "fadeUp %d", fadeUp);
		ofLog(OF_LOG_NOTICE, "alpha %d", alpha);
		ofLog(OF_LOG_NOTICE, "deltaAlpha %d", deltaAlpha);
		ofLog(OF_LOG_NOTICE, "targetAlpha %d", targetAlpha);
		ofLog(OF_LOG_NOTICE, "millisecondsToFade %d", millisecondsToFade);

	    // How many milliseconds dp we display each of the 255 alpha values.    
	    millisPerAlphaValue = millisecondsToFade / abs(deltaAlpha); 

	    startTime = ofGetElapsedTimeMillis();  // get the start time
	    switchTime = (float)millisPerAlphaValue;

	    ofLog(OF_LOG_NOTICE, "millisPerAlphaValue %d", millisPerAlphaValue);
	    // ofLog(OF_LOG_NOTICE, "startTime %d", startTime);
	    // ofLog(OF_LOG_NOTICE, "switchTime %d", switchTime);

	    ofLog(OF_LOG_NOTICE, "---------------------");
	}
}

int trmxMediaFrame::getAlpha(){
	return alpha;
}

    // Pixel Relacement Controls
    // void trmxMediaFrame::setTargetColor(int r, int g, int b, int a){}
    // void trmxMediaFrame::setReplacementColor(int r, int g, int b, int a){}
