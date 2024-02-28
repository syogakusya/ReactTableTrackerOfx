#include "ofApp.h"

void adjustGamma(cv::Mat& img, float gamma = 0.5) {
	cv::Mat lookUpTable(1, 256, CV_8U);
	unsigned char* p = lookUpTable.ptr();
	for (int i = 0; i < 256; i++) {
		p[i] = cv::saturate_cast<unsigned char>(pow(i / 255.0, gamma) * 255.0);
	}
	cv::LUT(img, lookUpTable, img);
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);

	cam.setup(640, 480);

	tracker.setPersistence(15);
	tracker.setMaximumDistance(50);

	gui.setup();
	gui.add(minAreaRadius.set("MinAreaRadius", 10.0, 0, 50.0));
	gui.add(maxAreaRadius.set("MaxAreaRadius", 100, 0, 500));
	gui.add(threshold.set("Threshold", 128, 0, 255));
	gui.add(trackHs.set("Track Hue/Saturation", false));
	gui.add(holes.set("Holes", false));
	gui.add(gamma.set("Gamma", 0.5, 0, 10));
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.update();
	if (cam.isFrameNew()) {
		img = ofxCv::toCv(cam);
		adjustGamma(img, gamma);

		contourFinder.setMinAreaRadius(minAreaRadius);
		contourFinder.setMaxAreaRadius(maxAreaRadius);
		//�O���C�X�P�[����
		contourFinder.setTargetColor(targetColor, trackHs ? ofxCv::TRACK_COLOR_HS : ofxCv::TRACK_COLOR_RGB);
		//��l��
		contourFinder.setThreshold(threshold);
		//�����m
		contourFinder.setFindHoles(holes);
		//�֊s���m�J�n
		contourFinder.findContours(img);

		tracker.track(contourFinder.getBoundingRects());
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	//cam.draw(0, 0);
	ofxCv::drawMat(img, 0, 0);

	ofSetColor(255);//�`��F�𔒂ɐݒ�
	ofSetLineWidth(2);//�`�悷����̑�����ݒ�
	contourFinder.draw();//�֊s�`��

	//ofNoFill();//�`����A�E�g���C���݂̂ɐݒ�
	//int n = contourFinder.size();//���m���������m��
	//for (int i = 0; i < n; i++) {

	//}

	vector<ReactTableFollower>& followers = tracker.getFollowers();
	for (int i = 0; i < followers.size(); i++) {
		followers[i].draw();
	}

	gui.draw();

	ofTranslate(220, 10);
	ofFill();
	ofSetColor(0);
	ofDrawRectangle(-3, -3, 32 + 6, 32 + 6);
	ofSetColor(targetColor);
	ofDrawRectangle(0, 0, 32, 32);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	targetColor = cam.getPixels().getColor(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
