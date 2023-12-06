#pragma once
#include <opencv4/opencv.hpp>

class Background {
public:
    int hasScreen;
    std::vector<std::pair<int,int>> screen;
    bool ready;
    Background(int length);
    void clear();
    void addImg(const cv::Mat& image);
    bool isReady();
    bool pinpointScreen(cv::Mat& frame);
    cv::Rect detectScreen();
    cv::Mat getModel();
    std::deque<cv::Mat> images;
    cv::Mat model;
private:
    int maxImg;
    int loc_count;
    std::pair<int,int> temp_loc;
    std::deque<cv::Mat> images2;
    double distance(const std::pair<int, int>& p1, const std::pair<int, int>& p2);
    cv::Mat model2;
    bool flip;
};