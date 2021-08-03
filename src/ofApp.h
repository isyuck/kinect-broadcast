#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxKinectForWindows2.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void changeBodySel(int &b);

		void keyPressed(int key);

		ofxKFW2::Device kinect;
		ofxPanel gui;
		ofxVec3Slider lhg;
		ofxVec3Slider rhg;
		ofxIntSlider selBodg;

		ofxOscSender osc;
};
