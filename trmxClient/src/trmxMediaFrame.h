#include "ofMain.h"

class trmxMediaFrame {
    
public:
    trmxMediaFrame();

    // Draw Controls
    void update();
    void draw();

    // Media Handling
    void loadMedia(string fileename);
    bool isMediaImage();
	bool isMediaVideo();

	// Movie Controls
	void play();
	void pause();
	void stop();
	void nextFrame();
	void previousFrame();
	void setFrame(int frame);
	void setSpeed(float speed);
	void setVolume(float volume);
	int getTotalNumFrames();
    int getCurrentFrame();

    // Fade Controls
    void fadeIn(int millisecondsToFade);
    void fadeOut(int millisecondsToFade);
    void fadeTo(int targetAlpha, int millisecondsToFade);

    // Setters
    void setNativeSize(bool nS);
    void setLoopVideo(bool lV);
    void setPosition(int x, int y);
    void setColor(int r, int g, int b);
    void setAlpha(int a);
    void setMaxFadeToAlpha(int mA);
    void setMinFadeToAlpha(int mA);

    int getAlpha();

 //    // Pixel Relacement Controls
 //    void setTargetColor(int r, int g, int b, int a);
 //    void setReplacementColor(int r, int g, int b, int a);

private:
	// Media Handling
	void loadImage();
    void loadVideo();
    //Fade Controls
    void alphaFade();
    void calculateAlphaFade(int targetAlpha, int millisecondsToFade);
	//Media Params
    string filePath;
    ofImage image;
    ofVideoPlayer videoPlayer;
    bool isVideo;
    bool isImage;
    bool topFrame; // if true display this frame on top.
    bool nativeSize; // true:display media in original width and height, false: fit to display
    bool loopVideo; // If this is a video, should we loop?
    // Draw Params
    int xPos;
    int yPos;
    int red;
    int green;
    int blue;
    int alpha;
    int minFadeToAlpha;
    int maxFadeToAlpha;
    // Fade Params
    bool fadeUp;
    int targetAlpha;
    int millisPerAlphaValue;
    int millisecondsToFade; // Number of seconds to fade image in or out.
    float startTime; // store when we start the timer
    float switchTime; // The time slice is complete, the time to switch the alpha value.

};