#include "background.h"
#include <opencv4/opencv.hpp>
#include "util.h"

using namespace std;
using namespace cv;

Background::Background(int length) : maxImg(length), model(cv::Mat()), ready(false), model2(cv::Mat()), flip(false) {
    temp_loc = {0,0};
    loc_count = 0;
    screen.reserve(4);
}

void Background::addImg(const cv::Mat& frame) {
    cv::Mat image = frame / maxImg;
    if(images.empty()){
        model = image.clone();
        images.push_back(image);
    }else if(images.size()<maxImg){
        images.push_back(image);
        model += images.back();
    }else if(images2.empty()){
        model2 = image.clone();
        images2.push_back(image);
    }else if(images2.size()<maxImg){
        images2.push_back(image);
        model2 += images2.back();
    }else{
        model -= images.front();
        images.pop_front();
        images.push_back(images2.front());
        model += images.back();
        model2 -= images2.front();
        images2.pop_front();
        images2.push_back(image);
        model2 += images2.back();
    }
}

bool comp(std::pair<int, int>& a, std::pair<int, int>& b){
    return a.second < b.second;
}

bool Background::pinpointScreen(cv::Mat& frame){
    if(screen.size()==4){
        //sort(screen.begin(), screen.end(), comp);
        if(screen[0].first > screen[1].first){
            swap(screen[0], screen[1]);
        }
        if(screen[2].first < screen[3].first){
            swap(screen[2], screen[3]);
        }
        return true;
    }
    auto [x,y] = oneStepTracker(*this, frame);
    if(x==0 && y==0){
        loc_count=0;
        temp_loc = {0,0};
        return false;
    }
    if(loc_count>=20 && distance(temp_loc, {x,y})<=30){
        screen.push_back({(temp_loc.first+x)/2, (temp_loc.second+y)/2});
        loc_count = 0;
        temp_loc = {0,0};
        cout<<"Good. next corner!"<<endl;
    }else{
        if(loc_count == 0){
            temp_loc = {x,y};
            loc_count++;
        }else{
            if(distance(temp_loc, {x,y})>30){
                loc_count = 0;
                temp_loc = {0,0};
                cout<<"too far. retry this corner"<<endl;
            }else{
                temp_loc = {(temp_loc.first+x)/2, (temp_loc.second+y)/2};
                loc_count++;
                //cout<<"loc_count: "<<loc_count<<endl;
            }
        }       
    }
    return false;
}

double Background::distance(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
    int deltaX = abs(p2.first - p1.first);
    int deltaY = abs(p2.second - p1.second);
    return std::sqrt(deltaX * deltaX + deltaY * deltaY);
}

bool Background::isReady(){
    if(images.size() == maxImg && images2.size() == maxImg){
        cv::Mat diffImg = findDiffImg(model2, model, 2);
        double diff_rate = globalMotion(diffImg, PIXEL_CHANGED_THRESHOLD);
        //cout<<diff_rate<<endl;
        if(!ready && diff_rate<BACKGROUND_STABLE_THRESHOLD){
            ready = true;
            //cout<<"Model completed!"<<endl;
            return true;
        }else if(ready && diff_rate<3*BACKGROUND_STABLE_THRESHOLD){
            return true;
        }else{
            ready = false;
            return false;
        }
    }else{
        ready = false;
        return false;
    }
    return false;
}


cv::Mat Background::getModel() {
    if(!isReady()){
        std::cout << "Error: background not ready" << std::endl;
        return cv::Mat();
    }else{
        return model;
    }
}

cv::Rect Background::detectScreen(){
    if(!isReady()){
        std::cout << "Error: background not ready" << std::endl;
        return cv::Rect();
    }
    cv::Mat grayModel, edges, grayModel2, edges2;
    cv::GaussianBlur(model, grayModel, cv::Size(5, 5), 0);
    cv::GaussianBlur(model2, grayModel2, cv::Size(5, 5), 0);
    cv::Canny(grayModel, edges, 50, 150);
    cv::Canny(grayModel2, edges2, 50, 150);
    //temporal filtering
    cv::bitwise_or(edges, edges2, edges);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    double largest_area = 0;
    std::vector<cv::Point> largest_contour;
    std::vector<cv::Point> largest_vertices;
    
    cv::Mat frame = grayModel.clone();
    for (const std::vector<cv::Point>& contour : contours) {
        // Approximate the contour to a polygon
        double epsilon = 0.04 * cv::arcLength(contour, true);
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, epsilon, true);
        
        // Determine if the contour is a rectangle based on the number of vertices
        if (approx.size() == 4) {
            cv::rectangle(frame, cv::boundingRect(approx), cv::Scalar(0, 255, 0), 2);
            // Draw a bounding box around the detected rectangle
            double area = cv::contourArea(contour);
            if (area > largest_area) {
                largest_area = area;
                largest_contour = contour;
                largest_vertices = approx;
            }
        }
    }
    //cv::imshow("Camera Test", frame);
    //cv::imshow("edges", edges);
    return cv::boundingRect(largest_vertices);

}

