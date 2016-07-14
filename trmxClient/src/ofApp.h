#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "trmxMediaFrame.h"

/***************************************************************

 oscCommunicatorClient

 - it sends messages to a server running oscCommunicatorServer app
 - get the IP address of the server computer

 - in ofApp.cpp inside the setup() function, change the value of
 clientDestination to the other IP as shown in the comment.

 Read the readme.txt

 ***************************************************************/

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);	

		void processOSCMessage(ofxOscMessage* oscMessage);
		void getOscParams(ofxOscMessage* oscMessage);
		void parseOSCAddress(string oscAddress);

		void sendPing();
		void sendDirList();
		void sendTotalNumFrames(string channel, int totalNumFrames);
		void sendCurrentFrame(string channel, int currentFrame);

		void initializeMediaFrames();
		void readMediaDir();
		void loadMediaFromDir(int mediaIndex, int requestedMediaIndex);

		bool displayInfo;
		bool pingFinity;

		string clientName;
		string mediaDirPath;

		ofTrueTypeFont font; // standard font
		ofTrueTypeFont titleFont; // font for some info in the title line

		int frameRate;

		//----------------------------------------
		ofxOscSender sender; // all-important ofxOscSender object
		string serverIpAddress; // IP address we're sending to
		int sendPort; // port we're sending to

		ofxOscReceiver receiver; // OSC receiver
		int recvPort; // port where we listen for messages
		string messages; // string containing the received messages for display

		// Parse an OscMessage into a string for easy logging
		string getOscMsgAsString(ofxOscMessage m);

		vector<string> explode( const string &delimiter, const string &str);

		ofDirectory dir;
		ofxXmlSettings settings;

	private:
		vector <trmxMediaFrame> mediaFrames;
    	int numMediaFrames;
    	int topMediaFrameIndex;

    	string requestAddress;
		string requestMediaFrameChannel;
		string requestAction;

		int bkRed;
		int bkGreen;
		int bkBlue;
};
