#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){

	// Load Settings
	if( settings.loadFile("settings.xml") ){
		ofLog(OF_LOG_NOTICE, "settings.xml loaded!");
	}else{
		ofLog(OF_LOG_NOTICE, "unable to load settings.xml check data/ folder");
	}

	// Set Config vars
	clientName = settings.getValue("settings:clientName", "clientName");
	serverIpAddress = settings.getValue("settings:serverIpAddress", "0.0.0.0");
	sendPort = settings.getValue("settings:sendPort", 0);
	recvPort = settings.getValue("settings:recvPort", 0);
	mediaDirPath = settings.getValue("settings:mediaDirPath", "/");
	frameRate = settings.getValue("settings:frameRate", 0);

	ofLog(OF_LOG_NOTICE, "serverIpAddress "+ serverIpAddress);
	ofLog(OF_LOG_NOTICE, "sendPort %d", sendPort);
	ofLog(OF_LOG_NOTICE, "recvPort %d", recvPort);
	ofLog(OF_LOG_NOTICE, "mediaDirPath " + mediaDirPath);

	// Set up window
	ofSetWindowTitle("Transmestizx Client");

	ofSetFrameRate(frameRate); // run at 60 fps
	ofSetVerticalSync(true);

	// set bg color to black
	bkRed = 0;
	bkGreen = 0;
	bkBlue = 0;
	ofBackground(bkRed, bkGreen, bkBlue);

	// load fonts to display stuff
	font.loadFont("futura_book.otf", 8);
	titleFont.loadFont("futura_book.otf", 16);

	displayInfo = false;
	pingFinity = true;

	// ofToggleFullscreen();

	// Enable some logging information
	//ofSetLogLevel(OF_LOG_VERBOSE);

	// Setup port for sending messages
	sender.setup(serverIpAddress, sendPort);

	// Setup port for receiving messages
	receiver.setup(recvPort);

	// Set up the media frames
	initializeMediaFrames();

	ofLog(OF_LOG_NOTICE, "__setup complete");

	mediaFrames[0].setAlpha(0);
//	mediaFrames[0].fadeTo(0, 5);

	mediaFrames[1].setAlpha(0);
//	mediaFrames[1].fadeTo(0, 5);

	mediaFrames[2].setAlpha(0);
//  mediaFrames[2].fadeTo(0, 5);

	mediaFrames[3].setAlpha(0);
//  mediaFrames[3].fadeTo(0, 5);

}

//--------------------------------------------------------------
void ofApp::update(){

	// OSC receiver queues up new messages, so you need to iterate
	// through waiting messages to get each incoming message

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		// Process the osc message
		processOSCMessage(&m);

	}

	//this is purely workaround for a mysterious OSCpack bug on 64bit linux
	// after startup, reinit the receivers
	// must be a timing problem, though - in debug, stepping through, it works.
	if(ofGetFrameNum() == 60){
		receiver.setup(recvPort);
	}

	//Update Media Frames
	
	for (int i = 0; i < (int)mediaFrames.size(); ++i){
		mediaFrames[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	// Reset
    ofSetColor(255,255,255,0);

    // Draw bottom MediaFrames, ie. all but the top media frame
	//	ofLog(OF_LOG_NOTICE, (char)mediaFrames.size);

	for (int i = 0; i < (int)mediaFrames.size(); ++i){
		if(topMediaFrameIndex!=i){
			mediaFrames[i].draw();
		}
	}

	// Draw Top MediaFrame. as long as it exists


	if(topMediaFrameIndex<(int)mediaFrames.size()){
		mediaFrames[topMediaFrameIndex].draw();
	}


	if(displayInfo){

		ofSetColor(ofColor::white);
		//Display some information about the client on the screen with the font you loaded
		 string debug = "Transmestizx Client "+ clientName
		 			   
		 			   + "\n Sending messages to " + string(serverIpAddress) + ":"
					   + ofToString(sendPort) + "\n Receiving messages on port " + ofToString(recvPort)
					   + "\n PingFinity " + ofToString(pingFinity)
					   +"\n media directory path: " + mediaDirPath;
		 titleFont.drawString(debug, 10, 37);


		string mediaFrameStats = "";
		for(int i=0; i<(int)mediaFrames.size(); i++){

			mediaFrameStats += "\n\n Media Frame ch" + ofToString(i+1) 
							+ "\n  Current Frame " + ofToString(mediaFrames[i].getCurrentFrame())
							+ "\n  Alpha " + ofToString(mediaFrames[i].getAlpha());

							if(topMediaFrameIndex == i){
								mediaFrameStats += "\n is the top Frame";
							}
		}

 		titleFont.drawString(mediaFrameStats, 10, 137);

		// Draw the directory contents

		for(int i = 0; i < (int)dir.size(); i++){
			string fileInfo = "file " + ofToString(i + 1) + " = " + dir.getName(i);
			ofDrawBitmapString(fileInfo, 50,i * 20 + 400);
		}

	}

}

void ofApp::initializeMediaFrames(){

	topMediaFrameIndex = 0;

	// // Build directory of media files.
	readMediaDir();

	ofLog(OF_LOG_NOTICE, "__initialize first frame");

	//Set up four Media Frame objects, with defautl images
	mediaFrames.push_back(trmxMediaFrame());
	mediaFrames.back().loadMedia("media/negro.jpg");

	ofLog(OF_LOG_NOTICE, "__initialize second frame");
	mediaFrames.push_back(trmxMediaFrame());
	mediaFrames.back().loadMedia("media/negro.jpg");

	ofLog(OF_LOG_NOTICE, "__initialize third frame");
    mediaFrames.push_back(trmxMediaFrame());
    mediaFrames.back().loadMedia("media/negro.jpg");

	ofLog(OF_LOG_NOTICE, "__initialize fourth frame");
    mediaFrames.push_back(trmxMediaFrame());
    mediaFrames.back().loadMedia("media/negro.jpg");

}


void ofApp::parseOSCAddress(string oscAddress){

	requestAddress = oscAddress;
	requestMediaFrameChannel = "";
	requestAction = "";

	vector<string> addressTokens = explode("/", requestAddress);

	// ofLog(OF_LOG_NOTICE, "number of address tokens %d", addressTokens.size());
	// for(int i=0; i< addressTokens.size(); i++){
	// 	ofLog(OF_LOG_NOTICE, "token: " + addressTokens[i]);
	// }

	if(addressTokens[0].size()!=0){
		ofLog(OF_LOG_NOTICE, "__Address is missing the leading /");
	}

	//Global Address Actions
	if(addressTokens.size()==2){
		requestAction = addressTokens[1];
		ofLog(OF_LOG_NOTICE, "__Global Address Action");
		ofLog(OF_LOG_NOTICE, "requestAction: " + requestAction);
	}

	// Channel Address Actions
	if(addressTokens.size()==3){
		
		requestMediaFrameChannel = addressTokens[1];
		//printfn(addressTokens[1]);
		requestAction = addressTokens[2];


		ofLog(OF_LOG_NOTICE, "__Channel Address Action");
		ofLog(OF_LOG_NOTICE, "mediaFrameChannel: " + ofToString(requestMediaFrameChannel));
		ofLog(OF_LOG_NOTICE, "requestAction: " + requestAction);
	}
}

void ofApp::processOSCMessage(ofxOscMessage* oscMessage){

	//ofLogNotice("Just received a message");
	//ofLogNotice(" to address " + oscMessage.getAddress());

	// Get the requested osc address
	requestAddress = oscMessage->getAddress();

	// Parse requested address for channel and/or requested action
	parseOSCAddress(requestAddress);

	// Is this a Global request action?  Check if we have a request media frame channel
	if(requestMediaFrameChannel.size()==0){

		// Global Requests

			// If you get a ping, send a ping
			if(requestAction == "ping"){
				ofLog(OF_LOG_NOTICE, "ping received");

				if(pingFinity){
					ofLog(OF_LOG_NOTICE, "~~~~PINGFINITY~~~~");
					// PingFinity: If you get a ping, send a ping
					// wait three secs
					//ofSleepMillis(3000);
					sendPing();
				}
			}

			// Toggle the information display
			if(requestAction == "toggle_display"){
				ofLog(OF_LOG_NOTICE, "toggle_display");
				displayInfo = !displayInfo;
			}

			// Toggle Fullscreen
			if(requestAction == "toggle_fullscreen"){
				ofLog(OF_LOG_NOTICE, "toggle_fullscreen");
				ofToggleFullscreen();
			}

			if(requestAction == "request_dir_list"){
				ofLog(OF_LOG_NOTICE, "request_dir_list");
				sendDirList();

			}

			// If you get a ping, send a ping
			if(requestAction == "set_background_color"){
				ofLog(OF_LOG_NOTICE, "set_background_color");

			if(oscMessage->getNumArgs() == 3 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32 &&
				oscMessage->getArgType(1) == OFXOSC_TYPE_INT32 && oscMessage->getArgType(2) == OFXOSC_TYPE_INT32 ){

				bkRed = oscMessage->getArgAsInt32(0);
				bkGreen = oscMessage->getArgAsInt32(1);
				bkBlue = oscMessage->getArgAsInt32(2);
				ofLog(OF_LOG_NOTICE, "bkRed %d", bkRed);
				ofLog(OF_LOG_NOTICE, "bkGreen %d", bkBlue);
				ofLog(OF_LOG_NOTICE, "bkBlue %d", bkBlue);
				ofBackground(bkRed, bkGreen, bkBlue);

			}else{
				ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
			}


			}

	}else{

		// Channel Specific Requests
			int mediaIndex = 0;


		// Select the media frame.
			if(requestMediaFrameChannel == "ch1"){
				ofLog(OF_LOG_NOTICE, "__Channel 1 Selected");
				mediaIndex = 0;

			}else if(requestMediaFrameChannel == "ch2"){
				ofLog(OF_LOG_NOTICE, "+++__Channel 2 Selected");
				mediaIndex = 1;

			}else if(requestMediaFrameChannel == "ch3"){
                                ofLog(OF_LOG_NOTICE, "__Channel 3 Selected");
                                mediaIndex = 2;

			}else if(requestMediaFrameChannel == "ch4"){
                                ofLog(OF_LOG_NOTICE, "__Channel 4 Selected");
                                mediaIndex = 3;


			}else{

				mediaIndex = 0;
			}

		
			if( mediaIndex < (int)mediaFrames.size() ){

				
					// @param int requestedMediaNumber
					if(requestAction == "set_media"){
						ofLog(OF_LOG_NOTICE, "** set_media");

						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32){

							int requestedMediaNumber = oscMessage->getArgAsInt32(0);
							ofLog(OF_LOG_NOTICE, "Requested Media Number " + ofToString(requestedMediaNumber));

							if(requestedMediaNumber > 0){
								int requestedMediaIndex = requestedMediaNumber - 1;
								loadMediaFromDir(mediaIndex, requestedMediaIndex);
							}else{
								ofLog(OF_LOG_NOTICE, "!! error - media number must be bigger than 0.");
							}

						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// Movie Controls
					if(requestAction == "play"){
						ofLog(OF_LOG_NOTICE, "play");
						mediaFrames[mediaIndex].play();
					}

					
					if(requestAction == "pause"){
						ofLog(OF_LOG_NOTICE, "pause");
						mediaFrames[mediaIndex].pause();
					}  

					
					if(requestAction == "stop"){
						ofLog(OF_LOG_NOTICE, "stop");
						mediaFrames[mediaIndex].stop();
					}

					
					if(requestAction == "next_frame"){
						ofLog(OF_LOG_NOTICE, "next_frame");
						mediaFrames[mediaIndex].nextFrame();
					}


					if(requestAction == "previous_frame"){
						ofLog(OF_LOG_NOTICE, "previous_frame");
						mediaFrames[mediaIndex].previousFrame();
					}

					// @param int frame
					if(requestAction == "set_frame"){
						ofLog(OF_LOG_NOTICE, "** set_frame");

						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32){
							int frame = oscMessage->getArgAsInt32(0);
							ofLog(OF_LOG_NOTICE, "frame " + ofToString(frame));
							mediaFrames[mediaIndex].setFrame(frame);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param float speed
					if(requestAction == "set_speed"){
						ofLog(OF_LOG_NOTICE, "** set_speed");

						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_FLOAT){

							float speed = oscMessage->getArgAsFloat(0);
							ofLog(OF_LOG_NOTICE, "speed " + ofToString(speed));
							mediaFrames[mediaIndex].setSpeed(speed);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param float volume
					if(requestAction == "set_volume"){
						ofLog(OF_LOG_NOTICE, "** set_volume");

						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_FLOAT){
							float volume = oscMessage->getArgAsFloat(0);
							ofLog(OF_LOG_NOTICE, "volume " + ofToString(volume));
							mediaFrames[mediaIndex].setVolume(volume);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}


					// @return int totalNumFrames
					if(requestAction == "get_total_num_frames"){
						ofLog(OF_LOG_NOTICE, "get_total_num_frames");

						int numFrames = mediaFrames[mediaIndex].getTotalNumFrames();

						// Send response

						sendTotalNumFrames(requestMediaFrameChannel, numFrames);
					}

					// @return in currentFrame
						if(requestAction == "get_current_frame"){
						ofLog(OF_LOG_NOTICE, "get_current_frame");

						int currentFrame = mediaFrames[mediaIndex].getCurrentFrame();

						// Send response

						sendCurrentFrame(requestMediaFrameChannel, currentFrame);
					}

				 	// Fade Controls

					// @param int millisecondsToFade
					if(requestAction == "fade_in"){
						ofLog(OF_LOG_NOTICE, "** fade_in");

						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32){
							int msToFade = oscMessage->getArgAsInt32(0);
							//ofLog(OF_LOG_NOTICE, "millisecondsToFade " + ofToString(msToFade));
							mediaFrames[mediaIndex].fadeIn(msToFade);

						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param int millisecondsToFade
					if(requestAction == "fade_out"){
						ofLog(OF_LOG_NOTICE, "** fade_out");

						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32){

							int msToFade = oscMessage->getArgAsInt32(0);
							//ofLog(OF_LOG_NOTICE, "millisecondsToFade " + ofToString(msToFade));
							mediaFrames[mediaIndex].fadeOut(msToFade);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param int targetAlpha [0-255]
					// @param int millisecondsToFade
					if(requestAction == "fade_to"){
						ofLog(OF_LOG_NOTICE, "** fade_to");

						if(oscMessage->getNumArgs() == 2 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32 &&
							oscMessage->getArgType(1) == OFXOSC_TYPE_INT32 ) {
							int tA = oscMessage->getArgAsInt32(0);
							int  msToFade = oscMessage->getArgAsInt32(1);
							//ofLog(OF_LOG_NOTICE, "targetAlpha %d", targetAlpha);
							//ofLog(OF_LOG_NOTICE, "millisecondsToFade " + ofToString(msToFade));
							mediaFrames[mediaIndex].fadeTo(tA,  msToFade);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}  


				    // Setters

				    // @param int topFrame [1|0]
					if(requestAction == "set_top_frame"){
						ofLog(OF_LOG_NOTICE, "** set_top_frame");
						topMediaFrameIndex = mediaIndex;
					}

					// @param int nativeSize [1|0]
					if(requestAction == "set_native_size"){
						ofLog(OF_LOG_NOTICE, "** set_native_size");
						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32){
							bool nativeSize = (bool)oscMessage->getArgAsInt32(0);
							mediaFrames[mediaIndex].setNativeSize(nativeSize);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param int loopVideo  [1|0]
					if(requestAction == "set_loop_video"){
						ofLog(OF_LOG_NOTICE, "** set_loop_video");

						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32){
							// ofLog(OF_LOG_NOTICE, "loopVideo %d", oscMessage->getArgAsInt32(0));
							bool loopVideo = (bool)oscMessage->getArgAsInt32(0);
							mediaFrames[mediaIndex].setLoopVideo(loopVideo);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param int x
					// @param int y
					if(requestAction == "set_position"){
						ofLog(OF_LOG_NOTICE, "** set_position");

						if(oscMessage->getNumArgs() == 2 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32 &&
							oscMessage->getArgType(1) == OFXOSC_TYPE_INT32 ) {
							int x = oscMessage->getArgAsInt32(0);
							int y = oscMessage->getArgAsInt32(1);
							ofLog(OF_LOG_NOTICE, "x %d", x);
							ofLog(OF_LOG_NOTICE, "y %d", y);
							mediaFrames[mediaIndex].setPosition(x, y);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param int red [0-255]
					// @param int green [0-255]
					// @param int blue [0-255]
					if(requestAction == "set_color"){
						ofLog(OF_LOG_NOTICE, "** set_color");

						if(oscMessage->getNumArgs() == 3 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32 &&
							oscMessage->getArgType(1) == OFXOSC_TYPE_INT32 && oscMessage->getArgType(2) == OFXOSC_TYPE_INT32 ){

							int red = oscMessage->getArgAsInt32(0);
							int green = oscMessage->getArgAsInt32(1);
							int blue = oscMessage->getArgAsInt32(2);
							ofLog(OF_LOG_NOTICE, "red %d", red);
							ofLog(OF_LOG_NOTICE, "green %d", green);
							ofLog(OF_LOG_NOTICE, "blue %d", blue);
							mediaFrames[mediaIndex].setColor(red, green, blue);

						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param int alpha [0-255]
					if(requestAction == "set_alpha"){
						ofLog(OF_LOG_NOTICE, "** set_alpha");

						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32){
							int alpha = oscMessage->getArgAsInt32(0);
							ofLog(OF_LOG_NOTICE, "alpha %d", alpha);
							mediaFrames[mediaIndex].setAlpha(alpha);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param int maxAlpha [0-255]
					if(requestAction == "set_max_fade_to_alpha"){
						ofLog(OF_LOG_NOTICE, "** set_max_fade_to_alpha");

						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32){
							int maxFadeToAlpha = oscMessage->getArgAsInt32(0);
							ofLog(OF_LOG_NOTICE, "maxFadeToAlpha %d", maxFadeToAlpha);
					 		mediaFrames[mediaIndex].setMaxFadeToAlpha(maxFadeToAlpha);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

					// @param int minAlpha [0-255]
					if(requestAction == "set_min_fade_to_alpha"){

						ofLog(OF_LOG_NOTICE, "** set_min_fade_to_alpha");
						if(oscMessage->getNumArgs() == 1 && oscMessage->getArgType(0) == OFXOSC_TYPE_INT32){
							int minFadeToAlpha = oscMessage->getArgAsInt32(0);
							ofLog(OF_LOG_NOTICE, "minFadeToAlpha %d", minFadeToAlpha);
							mediaFrames[mediaIndex].setMinFadeToAlpha(minFadeToAlpha);
						}else{
							ofLog(OF_LOG_NOTICE, "!! error - incorrect params");
						}
					}

		}

	}
}

void ofApp::loadMediaFromDir(int mediaIndex, int requestedMediaIndex){

	ofLog(OF_LOG_NOTICE, "**** mediaIndex %d", mediaIndex);
	ofLog(OF_LOG_NOTICE, "**** requestedMediaIndex %d", requestedMediaIndex);

	// for(int i = 0; i < dir.numFiles(); i++){
	//     ofLogNotice(dir.getPath(i));
	//     ofLog(OF_LOG_NOTICE, ofToString(i + 1) + "=" + dir.getName(i) + "$");
	// }

	//Check if media frame exists
	if((int)mediaFrames.size()> mediaIndex && mediaIndex >= 0 ){
		// Check if media file exists
		if( (int)dir.numFiles() > requestedMediaIndex){
		 	string requestedMediaPath = dir.getPath(requestedMediaIndex);
		 	ofLog(OF_LOG_NOTICE, "**** requestedMediaPath" + requestedMediaPath);
			mediaFrames[mediaIndex].loadMedia(requestedMediaPath);

		}else{
			ofLog(OF_LOG_NOTICE, "!! error - requested media does not exist");
		}

	}else{
		ofLog(OF_LOG_NOTICE, "!! error - media frame does not exist");
	}
}

void ofApp::getOscParams(ofxOscMessage* oscMessage){

	if(oscMessage->getNumArgs() > 0){
		if(oscMessage->getArgType(0) == OFXOSC_TYPE_STRING){

			string dirList = oscMessage->getArgAsString(0);
			ofLog(OF_LOG_NOTICE,  dirList);
			vector<string> v = explode("$", dirList);
//			ofLog(OF_LOG_NOTICE,  "^^^^^^^^^");
//			ofLog(OF_LOG_NOTICE,  v);





			// Parse dirList string, and get file index and names
			// Populate array.
		    for(int i=0; i<(int)v.size(); i++){
		    	cout <<i << " ["<< v[i] <<"] " <<endl;
		    	vector<string> fileV = explode("=", v[i]);

		    	cout <<0 << " ["<< fileV[0] <<"] " <<endl;
		    	cout <<1 << " ["<< fileV[1] <<"] " <<endl;
		    	cout <<2 << " ["<< fileV[2] <<"] " <<endl;
		    	cout <<3 << " ["<< fileV[3] <<"] " <<endl;

		    	// fileindex.push_back(fileV[0]);
		    	// filenames.push_back(fileV[1]);
		    }
		}
	}
}


void ofApp::sendTotalNumFrames(string channel, int totalNumFrames){
	 ofLog(OF_LOG_NOTICE, "send total frames to channel " + channel );
	// Send number of frames
	ofxOscMessage m;
	//need to add channel to address
	m.setAddress("/" + channel + "/receive_num_frames");
	m.addIntArg(totalNumFrames);
	sender.sendMessage(m);
}

void ofApp::sendCurrentFrame(string channel, int currentFrame){
	 ofLog(OF_LOG_NOTICE, "send current frame to channel " + channel );
	// Send current frame number
	ofxOscMessage m;
	//need to add channel to address
	m.setAddress("/" + channel + "/receive_current_frame");
	m.addIntArg(currentFrame);
	sender.sendMessage(m);
}






//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	// ofLog(OF_LOG_NOTICE, "key %d", key);
	// Send a message everytime a key is pressed.

	// switch(key){
	// 	case 'f':
	// 		ofToggleFullscreen();
	// 		break;
	// 	case 's':
	// 		sendPing();
	// 		break;
	// 	case 'p':
	// 		pingFinity = !pingFinity;
	// 		ofLog(OF_LOG_NOTICE, "Toggled pingFinity to %d", pingFinity);
	// 		break;
	// 	case 'd':
	// 		sendDirList();
	// 		break;
	// }

	switch(key){
    	// Movie Controls
    	case 'q':
			mediaFrames[0].play();
    		break;
    	case 'w':
			mediaFrames[0].pause();
    		break;
    	case 'e':
			mediaFrames[0].stop();
    		break;
    	case 'r':
			mediaFrames[0].nextFrame();
    		break;
    	case 't':
			mediaFrames[0].previousFrame();
    		break;
    	case 'y':
			mediaFrames[0].setFrame(50);
    		break;
    	case 'u':
			mediaFrames[0].setSpeed(0.05);
    		break;
    	case 'i':
    		mediaFrames[0].setSpeed(-1.0);
			//mediaFrames[0].setVolume(0);
    		break;
    	case 'o':
			ofLog(OF_LOG_NOTICE, "Total num frames %d", mediaFrames[0].getTotalNumFrames());
    		break;
    	case 'p':
    		sendPing();
    		break;
    	case 'a':
    		mediaFrames[0].fadeOut(100000);
    		mediaFrames[1].fadeOut(100000);
			// mediaFrames[0].setAlpha(0);
			// mediaFrames[1].setAlpha(255);
    		break;
    	case 's':
    		mediaFrames[0].fadeIn(100000);
    		mediaFrames[1].fadeIn(100000);
    		// mediaFrames[0].setAlpha(150);
    		// mediaFrames[1].setAlpha(150);
    		break;
    	case 'd':
			// mediaFrames[0].setAlpha(255);
			// mediaFrames[1].setAlpha(0);
    		break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'g':
            displayInfo = !displayInfo;
            break;

        case 'z':
            loadMediaFromDir(0, 0);
            mediaFrames[0].play();
            break;
        case 'x':
            loadMediaFromDir(0, 2);
            break;
        case 'c':
            loadMediaFromDir(0, 3);
            break;
        case 'v':
            loadMediaFromDir(1, 1);
            break;
		case 'b':
            loadMediaFromDir(1, 2);
            break;
        case 'n':
            loadMediaFromDir(1, 3);
            break;
        case 'm':
            loadMediaFromDir(1, 8);
            mediaFrames[1].play();
            break;
    }

}

void ofApp::sendPing(){
	ofLog(OF_LOG_NOTICE, "send ping");
	ofxOscMessage m;
	m.setAddress("/ping");
	m.addStringArg("ping");
	sender.sendMessage(m);
}

void ofApp::sendDirList(){
	ofLog(OF_LOG_NOTICE, "send dir list");

	// Generate dir list as formatted string
	string dirList  = "";
	//go through and print out all the paths
	for(int i = 0; i < dir.numFiles(); i++){
	    ofLogNotice(dir.getPath(i));
	    dirList += ofToString(i + 1) + "=" + dir.getName(i) + "$";
	}
	dirList += "#";
	//ofLog(OF_LOG_NOTICE, "Sending dirList string %d", dirList);
	ofLog(OF_LOG_NOTICE, dirList);

	// Send dir lists
	ofxOscMessage m;
	m.setAddress("/receive_dir_list");
	m.addStringArg(dirList);
	sender.sendMessage(m);
}

void ofApp::readMediaDir(){

	//[index number1]=[filename 1]$[index number2]=[filename 2]$...$[index numbern]=[filename n]$#
	ofLog(OF_LOG_NOTICE, "read media dir:" + mediaDirPath);
	
	//only show some files formats
	dir.allowExt("png");
	dir.allowExt("jpg");
	dir.allowExt("mov");
	dir.allowExt("mp4");
	//populate the directory object
	dir.listDir(mediaDirPath);

}

//--------------------------------------------------------------
string ofApp::getOscMsgAsString(ofxOscMessage m){
	string msg_string;
	msg_string = m.getAddress();
	msg_string += ":";
	for(int i = 0; i < m.getNumArgs(); i++){
		// get the argument type
		msg_string += " " + m.getArgTypeName(i);
		msg_string += ":";
		// display the argument - make sure we get the right type
		if(m.getArgType(i) == OFXOSC_TYPE_INT32){
			msg_string += ofToString(m.getArgAsInt32(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
			msg_string += ofToString(m.getArgAsFloat(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
			msg_string += m.getArgAsString(i);
		}
		else{
			msg_string += "unknown";
		}
	}
	return msg_string;
}

vector<string> ofApp::explode( const string &delimiter, const string &str)
{
    ofLog(OF_LOG_NOTICE, "A");
    ofLog(OF_LOG_NOTICE, delimiter);
    ofLog(OF_LOG_NOTICE, str);
    ofLog(OF_LOG_NOTICE, "Z");

    vector<string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0;
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
    return arr;
}
