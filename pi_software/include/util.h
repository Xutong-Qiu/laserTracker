#pragma once
#include "background.h"
#include <opencv4/opencv.hpp>
#include <core/libcamera_app.hpp>
#include <libcamera/framebuffer.h>

std::tuple<cv::Mat, int, std::pair<int, int>> diff_max(cv::Mat img1, cv::Mat img2, int radius);
cv::Mat findDiffImg(const cv::Mat& img1,const cv::Mat& img2, int radius);
double globalMotion(const cv::Mat& diffImg, int threshold);
std::pair<int,int> oneStepTracker(Background& bg, cv::Mat& img);
std::vector<std::tuple<int,int,int>> maxValPos(const cv::Mat& grayImg, int threshold, int num_pts);
std::pair<double, double> getLaserLocationNormalized(
    const std::vector<std::pair<int, int>>& screenCorners,
    const std::pair<int, int>& laserLocation);
cv::Mat frameBufferToCvMat(LibcameraApp& app, const libcamera::FrameBuffer &buffer);