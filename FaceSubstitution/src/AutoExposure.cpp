/*
 * AutoExposure.cpp
 *
 *  Created on: 03/03/2012
 *      Author: arturo
 */

#include "AutoExposure.h"
#include "ofxCv.h"

AutoExposure::AutoExposure() {
	// TODO Auto-generated constructor stub

}

AutoExposure::~AutoExposure() {
	// TODO Auto-generated destructor stub
}

bool AutoExposure::setup(int device, int w, int h){
#ifdef TARGET_LINUX
	if(!settings.setup("/dev/video" + ofToString(device))){
		return false;
	}
#endif
	grayPixels.allocate(w,h,1);
	grayPixelsMask.allocate(w,h,1);

#ifdef TARGET_LINUX
	settings["Exposure, Auto"] = 1;
#endif
	//settings["Exposure (Absolute)"] = 1033;

	// thinkpad
	/*minExposure = 800;
	maxExposure = 1400;*/

	// logitech
	minExposure = 200;
	maxExposure = 300;

	return true;
}

void AutoExposure::update(ofPixels & frame, ofPixels & mask){
	ofxCv::convertColor(frame,grayPixels,CV_RGB2GRAY);
	cv::Scalar mean;
	if(mask.getNumChannels()==3){
		ofxCv::convertColor(mask,grayPixelsMask,CV_RGB2GRAY);
		mean = cv::mean(ofxCv::toCv(grayPixels),ofxCv::toCv(grayPixelsMask));
	}else if(mask.getNumChannels()==1){
		mean = cv::mean(ofxCv::toCv(grayPixels),ofxCv::toCv(mask));

	}

	int exposure = ofMap(mean.val[0],40,200,maxExposure,minExposure);

#ifdef TARGET_LINUX
	settings["Exposure (Absolute)"] = exposure;
#endif
}
