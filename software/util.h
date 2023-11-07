#pragma once
#include "background.h"
#include <opencv4/opencv.hpp>

const int PIXEL_CHANGED_THRESHOLD = 7;
const double BACKGROUND_STABLE_THRESHOLD = 0.01;
const int BACKGROUND_IMG_NUM = 5;


std::tuple<cv::Mat, int, std::pair<int, int>> diff_max(cv::Mat img1, cv::Mat img2, int radius);
cv::Mat findDiffImg(const cv::Mat& img1,const cv::Mat& img2, int radius);
std::vector<std::tuple<int,int,int>> maxValPos(const cv::Mat& grayImg, int threshold, int num_pts);
double globalMotion(const cv::Mat& diffImg, int threshold);
std::pair<int,int> oneStepTracker(Background& bg, cv::Mat& img);