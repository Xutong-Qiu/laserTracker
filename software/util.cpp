#include "background.h"
#include <opencv4/opencv.hpp>
#include "util.h"
using namespace std;
using namespace cv;


cv::Mat findDiffImg(const cv::Mat& img1,const cv::Mat& img2, int radius){
    radius = 2 * std::max(2, radius / 2) + 1; // Ensure radius is odd
    //cv::GaussianBlur(img1, img1, cv::Size(radius, radius), 0);
    //cv::GaussianBlur(img2, img2, cv::Size(radius, radius), 0);
    cv::Mat diff;
    cv::absdiff(img1, img2, diff);
    return diff;
}

std::tuple<cv::Mat, int, std::pair<int, int>> diff_max(cv::Mat gray1, cv::Mat gray2, int radius = 2) {
    //cv::Mat gray1, gray2;
    //cv::cvtColor(img1, gray1, cv::COLOR_BGR2GRAY);
    //cv::cvtColor(img2, gray2, cv::COLOR_BGR2GRAY);

    // We blur the images by radius at least 3. For 640x480 webcam, 3 seems to
    // substantially improve laser detection.
    radius = 2 * std::max(2, radius / 2) + 1; // Ensure radius is odd
    cv::GaussianBlur(gray1, gray1, cv::Size(radius, radius), 0);
    cv::GaussianBlur(gray2, gray2, cv::Size(radius, radius), 0);

    cv::Mat diff;
    cv::absdiff(gray1, gray2, diff);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(diff, &minVal, &maxVal, &minLoc, &maxLoc);

    std::pair<int, int> max_location(maxLoc.x, maxLoc.y);

    return std::make_tuple(diff, maxVal, max_location);
}


bool comp(const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b) {
    return std::get<2>(a) > std::get<2>(b);
}

vector<tuple<int,int,int>> maxValPos(const cv::Mat& grayImg, int threshold, int num_pts) {
    // Create a binary mask where values are >= threshold
    cv::Mat mask = (grayImg >= threshold);
    // Find non-zero locations (x, y) where mask is true
    std::vector<cv::Point> nonzeroPoints;
    cv::findNonZero(mask, nonzeroPoints);
    vector<tuple<int,int,int>> res;
    for (int i = 0; i < nonzeroPoints.size(); ++i) {
        res.push_back(tuple<int,int,int>(nonzeroPoints[i].x,nonzeroPoints[i].y, grayImg.at<uchar>(nonzeroPoints[i].y, nonzeroPoints[i].x)));
    }
    sort(res.begin(),res.end(),comp);
    if (nonzeroPoints.size() <= num_pts) {
        return res;
    }else{
        return vector<tuple<int,int,int>>(res.begin(), res.begin() + num_pts);
    }

}

double globalMotion(const cv::Mat& diffImg, int threshold) {
    cv::Mat mask = diffImg > threshold;
    int movedPixels = cv::countNonZero(mask);
    int totalPixels = diffImg.rows * diffImg.cols;
    //cv::imshow("Camera", mask);
    return static_cast<double>(movedPixels) / totalPixels;
}

pair<int,int> oneStepTracker(Background& bg, cv::Mat& img){
    pair<int,int> res =pair{0,0};
    if(!bg.isReady()){
        bg.addImg(img);
        return res;
    }
    cv::Mat diffImg = findDiffImg(bg.getModel(), img, 2);
    //cv::imshow("Camera", bg.getModel());
    //cout<<globalMotion(diffImg, PIXEL_CHANGED_THRESHOLD)<<endl;
    if(globalMotion(diffImg, PIXEL_CHANGED_THRESHOLD)<3*BACKGROUND_STABLE_THRESHOLD){
        vector<tuple<int,int,int>> candidates = maxValPos(diffImg, PIXEL_CHANGED_THRESHOLD, 10);
        if(candidates.size()==0){
            return {0,0};
        }
        auto [x,y,val] = candidates[0];
        
        // cv::Mat mask = diffImg > PIXEL_CHANGED_THRESHOLD;
        // cv::imshow("move", mask);
        
        return {x, y};
    }else{
        //cout<<"Background changed. Remodeling..."<<endl;
        bg.addImg(img);
    }
    return res;
    
}

