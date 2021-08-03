#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	// kinect
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	// gui
	selBodg.addListener(this, &ofApp::changeBodySel);
	gui.setup();
	gui.add(lhg.setup("left  hand", { 0, 0, 0 }, { -2, -2, -2 }, { 2, 2, 2 }));
	gui.add(rhg.setup("right hand", { 0, 0, 0 }, { -2, -2, -2 }, { 2, 2, 2 }));
	gui.add(selBodg.setup("body", 0, 0, 16));

	osc.setup("localhost", 9991);
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	auto bodies = kinect.getBodySource()->getBodies();

	selBodg = selBodg % bodies.size();
	selBodg.setMax(bodies.size() - 1);

	const ofVec3f leftHandPos = bodies[selBodg].joints[JointType_HandLeft].getPosition();
	const ofVec3f rightHandPos = bodies[selBodg].joints[JointType_HandRight].getPosition();
	const auto rts = bodies[selBodg].rightHandState;
	const auto lts = bodies[selBodg].leftHandState;

	lhg = leftHandPos;
	rhg = rightHandPos;

	// draw kinect camera feed
	const auto w = ofGetWidth();
	const auto h = ofGetHeight();
	kinect.getColorSource()->draw(0, 0, w, h);
	kinect.getBodySource()->drawProjected(0, 0, w, h);

	gui.draw();

	auto mkMsg = [&](string s, float v) {
		ofxOscMessage msg;
		msg.setAddress("/k" + s);
		msg.addFloatArg(v);
		return msg;
	};

	std::vector<ofxOscMessage> msgs;
	msgs.push_back(mkMsg("/l/x", leftHandPos.x));
	msgs.push_back(mkMsg("/l/y", leftHandPos.y));
	msgs.push_back(mkMsg("/l/z", leftHandPos.z));
	msgs.push_back(mkMsg("/l/open", lts));

	msgs.push_back(mkMsg("/r/x", rightHandPos.x));
	msgs.push_back(mkMsg("/r/y", rightHandPos.y));
	msgs.push_back(mkMsg("/r/z", rightHandPos.z));
	msgs.push_back(mkMsg("/r/open", rts));

	for (const auto &msg : msgs) {
		osc.sendMessage(msg);
		//std::cout << msg << '\n';
	}

}

//--------------------------------------------------------------
void ofApp::changeBodySel(int &b) {
	selBodg = b;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'n':
		selBodg = selBodg + 1;
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	}
}
