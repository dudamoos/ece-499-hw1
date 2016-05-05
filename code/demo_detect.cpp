#include <iostream>
#include <vector>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "raspicam_cv/RaspiCamCV.h"

RaspiCamCvCapture* InitCapture() {
	RASPIVID_CONFIG* config = new RASPIVID_CONFIG;
	config->width = 640; config->height = 480; config->framerate = 30;
	config->bitrate = 0; config->monochrome = 0;
	RaspiCamCvCapture* capture = raspiCamCvCreateCameraCapture2(0, config);
	delete config;
	return capture;
}

// Return ship center - ship extends 24 pixels sideways and 10 pixels up/down
cv::Point FindShip(cv::Mat3b& frame) {
	static const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(2, 2));
	static const cv::Vec3b hsv_max = cv::Vec3b(30 + 30, 125, 255);
	static const cv::Vec3b hsv_min = cv::Vec3b(30 - 30,  25, 150);
	static cv::Mat3b hsv = cv::Mat3b(63, 640);
	static cv::Mat1b mask = cv::Mat1b(63, 640);
	
	cv::cvtColor(frame.rowRange(417, 480), hsv, CV_RGB2HSV);
	cv::inRange(hsv, hsv_min, hsv_max, mask);
	cv::erode(mask, mask, kernel); cv::dilate(mask, mask, kernel);
	cv::Moments moments = cv::moments(mask, true);
	return cv::Point(moments.m10 / moments.m00, 417 + moments.m01 / moments.m00);
}

cv::Point FindShot(cv::Mat3b& frame) {
	//static const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
	static const cv::Vec3b rgb_max = cv::Vec3b(255, 255, 255); // Yes really
	static const cv::Vec3b rgb_min = cv::Vec3b(255, 255, 255);
	static cv::Mat1b mask = cv::Mat1b(134, 640);
	
	cv::inRange(frame.rowRange(346, 480), rgb_min, rgb_max, mask);
	//cv::erode(mask, mask, kernel); cv::dilate(mask, mask, kernel);
	cv::Moments moments = cv::moments(mask, true);
	return cv::Point(moments.m10 / moments.m00, 346 + moments.m01 / moments.m00);
}

void FindEnemies(cv::Mat3b& frame, std::vector<cv::Point>& dest) {
	static const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(2, 2));
	static const cv::Vec3b rgb_max = cv::Vec3b(150, 150, 150);
	static const cv::Vec3b rgb_min = cv::Vec3b(  0,   0,   0);
	static cv::Mat1b mask = cv::Mat1b(280, 640);
	static std::vector<std::vector<cv::Point>> contours(5 * 11);
	
	cv::inRange(frame.rowRange(70, 350), rgb_min, rgb_max, mask);
	cv::bitwise_not(mask, mask); // Want NON-black pixels
	cv::erode(mask, mask, kernel); cv::dilate(mask, mask, kernel);
	
	dest.clear(); contours.clear(); // TODO CV_RETR_EXTERNAL or CV_RETR_LIST ?
	cv::findContours(mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE/*, cv::Point(70, 0)*/);
	for (std::vector<cv::Point> contour : contours) {
		cv::Moments moments = cv::moments(contour);
		dest.emplace_back(moments.m10 / moments.m00, 70 + moments.m01 / moments.m00);
	}
}

int main(int argc, char** argv) {
	RaspiCamCvCapture* capture = InitCapture();
	cv::namedWindow("Raw Camera");
	cv::namedWindow("Detectors");
	
	bool exit = false;
	cv::Mat3b showFrame(480, 640);
	std::vector<cv::Point> enemies; enemies.reserve(5 * 11); // 55 total enemies
	do {
		cv::Mat3b frame = cv::Mat(raspiCamCvQueryFrame(capture));
		cv::imshow("Raw Camera", frame);
		
		showFrame.setTo(cv::Scalar(0, 0, 0));
		cv::Point ship = FindShip(frame);
		cv::rectangle(showFrame, ship - cv::Point(24, 10), ship + cv::Point(25, 11), cv::Scalar(255, 128,  64), CV_FILLED);
		cv::Point shot = FindShot(frame);
		cv::rectangle(showFrame, shot - cv::Point( 1,  2), shot + cv::Point( 2,  3), cv::Scalar(255, 255, 255), CV_FILLED);
		FindEnemies(frame, enemies);
		for (const cv::Point p : enemies)
			cv::rectangle(showFrame, p - cv::Point(4, 4), p + cv::Point(5, 5), cv::Scalar(0, 0, 255), CV_FILLED);
		cv::putText(showFrame, cv::format("Enemies remaining: %zu", enemies.size()), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0));
		
		int direction = 0;
		if (shot.x > 0 && ship.x > 0) {
			int diff = ship.x - shot.x;
			if (std::abs(diff) > 24) cv::rectangle(showFrame, ship - cv::Point(2,2), ship + cv::Point(3,3), cv::Scalar(0,255,0), CV_FILLED);
			else {
				direction = ((diff >> 31) & 0xFFFFFFFE) + 1;
				cv::line(showFrame, ship - cv::Point(10*direction,0), ship + cv::Point(10*direction,0), cv::Scalar(0,255,255));
				cv::rectangle(showFrame, ship + cv::Point(8*direction,-2), ship + cv::Point(13*direction,3), cv::Scalar(0,255,255));
			}
		}
		
		cv::imshow("Detectors", showFrame);
		if (cv::waitKey(10) == 27) exit = true;
	} while (!exit);
	
	cv::destroyWindow("Raw Camera");
	cv::destroyWindow("Detectors");
	raspiCamCvReleaseCapture(&capture);
	return 0;
}