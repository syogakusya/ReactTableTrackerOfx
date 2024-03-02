#include "touchTableTracker.h"

const float dyingTime = 1;

void TouchTableTracker::setup(const cv::Rect& track) {
	color.setHsb(ofRandom(0, 255), 255, 255);
	cur = ofxCv::toOf(track).getCenter();
	smooth = cur;
}

void TouchTableTracker::update(const cv::Rect& track) {
	cur = ofxCv::toOf(track).getCenter();
	smooth.interpolate(cur, .5);
	all.addVertex(smooth);
}

void TouchTableTracker::kill() {
	float curTime = ofGetElapsedTimef();
	if (startedDying == 0) {
		startedDying = curTime;
	}
	else if (curTime - startedDying > dyingTime) {
		dead = true;
	}
}

void TouchTableTracker ::draw() {
	ofPushStyle();
	float size = 16;
	ofSetColor(255);
	if (startedDying) {
		ofSetColor(ofColor::red);
		size = ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, size, 0, true);
	}
	ofNoFill();
	ofDrawCircle(cur, size);
	ofSetColor(color);
	all.draw();
	ofSetColor(255);
	ofDrawBitmapString(ofToString(label), cur);
	ofPopStyle();
}

//--public method--------------------------------------------------------------
void TouchTableThread ::adjustGamma(cv::Mat& img, float gamma) {
	/*cv::Mat lookUpTable(1, 256, CV_8U);
	unsigned char* p = lookUpTable.ptr();
	for (int i = 0; i < 256; i++) {
		p[i] = cv::saturate_cast<unsigned char>(pow(i / 255.0, gamma) * 255.0);
	}*/
	uchar LUT[256];
	for (int i = 0; i < 256; i++) {
		LUT[i] = (int)(pow((double)i / 255.0, gamma) * 255.0);
	}
	cv::Mat lookUpTable = cv::Mat(1, 256, CV_8UC1, LUT);
	cv::LUT(img, lookUpTable, img);
}

void TouchTableThread::getCamera(ofVideoGrabber* cam) {
	camera = cam;
}


void TouchTableThread::draw() {
	lock();
	ofxCv::drawMat(gray, 0, 0);
	
	ofSetColor(255);//�`��F�𔒂ɐݒ�
	ofSetLineWidth(2);//�`�悷����̑�����ݒ�
	contourFinder_->draw();//�֊s�`��

	//ofNoFill();//�`����A�E�g���C���݂̂ɐݒ�
	//int n = contourFinder.size();//���m���������m��
	//for (int i = 0; i < n; i++) {

	//}

	vector<TouchTableTracker>& followers = tracker_->getFollowers();
	for (int i = 0; i < followers.size(); i++) {
		followers[i].draw();
	}
	unlock();
}

void TouchTableThread::setParam(
	float minAR, 
	float maxAR, 
	float th, 
	float gm) 
{
	minAreaRadius = minAR;
	maxAreaRadius = maxAR;
	threshold = th;
	gamma = gm;
	//��l��
	lock();
	contourFinder_->setThreshold(threshold);
	contourFinder_->setMinAreaRadius(minAreaRadius);
	contourFinder_->setMaxAreaRadius(maxAreaRadius);
	tracker_->setPersistence(15);
	tracker_->setMaximumDistance(32);
	unlock();
}

ofColor TouchTableThread::getTargetColor(int x, int y) {
	targetColor = camera->getPixels().getColor(x, y);
	return targetColor;
}

//--private method-------------------------------------------------
void TouchTableThread::threadedFunction() {
	while (isThreadRunning()) {
		if (camera != nullptr) {
			if (camera->isFrameNew()) {
				lock();
				img = ofxCv::toCv(*camera);
				gray = img.clone();
				adjustGamma(gray, gamma);
				//�O���C�X�P�[����
				contourFinder_->setTargetColor(targetColor, false ? ofxCv::TRACK_COLOR_HS : ofxCv::TRACK_COLOR_RGB);
				////�����m
				//contourFinder_->setFindHoles(holes);

				
				//�֊s���m�J�n
				contourFinder_->findContours(gray);

				tracker_->track(contourFinder_->getBoundingRects());
				unlock();
			}
			Sleep(2);
		}
	}
}