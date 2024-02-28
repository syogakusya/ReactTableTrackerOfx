#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

#include "ReactTableFollower.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofVideoGrabber cam;//�J�����f���擾
	cv::Mat img;//�K���}�␳�pMat
	ofxCv::ContourFinder contourFinder;
	ofColor targetColor;
	ofxCv::RectTrackerFollower<ReactTableFollower> tracker;


	//Gui�����p�����[�^�[
	ofxPanel gui;
	ofParameter<float> threshold;
	ofParameter<bool> trackHs;
	ofParameter<bool> holes;
	ofParameter<float> minAreaRadius;
	ofParameter<float> maxAreaRadius;
	ofParameter<float> gamma;
};
