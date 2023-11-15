#include "background.h"
#include <opencv4/opencv.hpp>
#include "util.h"
#include <algorithm>
using namespace std;
using namespace cv;


bool comp(const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b) {
    return std::get<2>(a) > std::get<2>(b);
}

cv::Mat findDiffImg(const cv::Mat& img1,const cv::Mat& img2, int radius){
    radius = 2 * std::max(2, radius / 2) + 1; // Ensure radius is odd
    cv::Mat diff;
    cv::absdiff(img1, img2, diff);
    return diff;
}

std::tuple<cv::Mat, int, std::pair<int, int>> diff_max(cv::Mat gray1, cv::Mat gray2, int radius = 2) {

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

cv::Mat frameBufferToCvMat(LibcameraApp& app, const libcamera::FrameBuffer &buffer){
    // Retrieve the planes from the buffer
    const std::vector<libcamera::FrameBuffer::Plane> &planes = buffer.planes();
    if(planes.empty())
        return cv::Mat(); 

    const libcamera::FrameBuffer::Plane &plane = planes[0];
    // Get the file descriptor
    int fileDescriptor = plane.fd.get();  // Using the get() function for SharedFD

    void *data = mmap(NULL, plane.length, PROT_READ, MAP_SHARED, fileDescriptor, plane.offset);
    if (data == MAP_FAILED) 
    {
        return cv::Mat();
    }
	unsigned char *byteData = static_cast<unsigned char *>(data);
    cv::Mat image(972, 1296, CV_8UC1, byteData, app.GetStreamInfo(app.VideoStream()).stride); 
	cv::Mat frame = image.clone();
	munmap(data, plane.length);
    return frame;
}

std::pair<double, double> getLaserLocationNormalized(
    const std::vector<std::pair<int, int>>& screenCorners,
    const std::pair<int, int>& laserLocation) {
    static bool hasTrMatrix = false;
    static  cv::Mat transformationMatrix;
    // Check if the input is valid
    if (screenCorners.size() != 4) {
        throw std::invalid_argument("There must be exactly four screen corners provided.");
    }
    if(!hasTrMatrix){
        // Convert screen corners to OpenCV points
        std::vector<cv::Point2f> cvScreenCorners(4);
        for (size_t i = 0; i < screenCorners.size(); ++i) {
            cvScreenCorners[i] = cv::Point2f(static_cast<float>(screenCorners[i].first),
                                            static_cast<float>(screenCorners[i].second));
        }

        // Define the points in the projector's coordinate system
        std::vector<cv::Point2f> projectorCorners = {
            cv::Point2f(0.0f, 0.0f), // Top-left
            cv::Point2f(1.0f, 0.0f), // Top-right
            cv::Point2f(1.0f, 1.0f), // Bottom-right
            cv::Point2f(0.0f, 1.0f)  // Bottom-left
        };

        // Calculate the perspective transformation matrix
        transformationMatrix = cv::getPerspectiveTransform(cvScreenCorners, projectorCorners);
        hasTrMatrix = true;
    }
    // Convert laser location to OpenCV point
    cv::Point2f cvLaserLocation(static_cast<float>(laserLocation.first),
                                static_cast<float>(laserLocation.second));
    // Apply the transformation to the laser point
    std::vector<cv::Point2f> laserPoints = {cvLaserLocation};
    std::vector<cv::Point2f> transformedLaserPoints;
    cv::perspectiveTransform(laserPoints, transformedLaserPoints, transformationMatrix);

    // Return the transformed point as a pair of doubles
    return {static_cast<double>(transformedLaserPoints[0].x), static_cast<double>(transformedLaserPoints[0].y)};
}
