#include <opencv4/opencv.hpp>

#include <chrono>

#include <core/libcamera_app.hpp>
#include <core/options.hpp>

using namespace std;
using namespace std::placeholders;
using namespace cv;

#include <opencv4/opencv.hpp>
#include <sys/mman.h>
#include <libcamera/framebuffer.h>
#include "background.h"
#include "util.h"


cv::Mat frameBufferToCvMat(LibcameraApp& app, const libcamera::FrameBuffer &buffer)
{
    // Retrieve the planes from the buffer
    const std::vector<libcamera::FrameBuffer::Plane> &planes = buffer.planes();

	//cout<<"\n\n\n\n\n\n\n\n"<<planes.size()<<"\n\n\n\n\n\n\n\n";
    if(planes.empty())
        return cv::Mat();  // Return an empty Mat if no planes are present

    // In real scenarios, you might need to handle multiple planes differently.
    const libcamera::FrameBuffer::Plane &plane = planes[0];
    // Get the file descriptor
    int fileDescriptor = plane.fd.get();  // Using the get() function for SharedFD

    void *data = mmap(NULL, plane.length, PROT_READ, MAP_SHARED, fileDescriptor, plane.offset);
    if (data == MAP_FAILED) 
    {
        return cv::Mat();
    }
	unsigned char *byteData = static_cast<unsigned char *>(data);
	// for (size_t i = 0; i < plane.length && i < 100; ++i) { // Printing first 100 bytes as an example
	// 	printf("%02x ", byteData[i]);
	// 	if ((i + 1) % 16 == 0) {
	// 		printf("\n");
	// 	}
	// }
    cv::Mat image(972, 1296, CV_8UC1, byteData, app.GetStreamInfo(app.ViewfinderStream()).stride); 
    //cv::Mat image(1296, 972, CV_8UC1, byteData); 
	cv::Mat frame = image.clone();
	//cv::imshow("test",image);
	// for (int i = 0; i < image.rows; ++i) {
    //     for (int j = 0; j < image.cols; ++j) {
    //         // Print the pixel value at (i, j)
    //         std::cout << static_cast<int>(image.at<uchar>(i, j))<<" ";
            
    //     }
	// 	cout<<endl;
    // }
	munmap(data, plane.length);
    return frame;
}


static void event_loop(LibcameraApp &app)
{
	Options const *options = app.GetOptions();

	app.OpenCamera();
	app.ConfigureViewfinder();
	app.StartCamera();
	
	Background bg(BACKGROUND_IMG_NUM);
    
    Mat frame;
    int frame_count = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
	for (unsigned int count = 0; ; count++)
	{


		LibcameraApp::Msg msg = app.Wait();
		if (msg.type == LibcameraApp::MsgType::Timeout)
		{
			LOG_ERROR("ERROR: Device timeout detected, attempting a restart!!!");
			app.StopCamera();
			app.StartCamera();
			continue;
		}
		if (msg.type == LibcameraApp::MsgType::Quit)
			return;
		else if (msg.type != LibcameraApp::MsgType::RequestComplete)
			throw std::runtime_error("unrecognised message!");

		LOG(2, "Viewfinder frame " << count);
		frame_count++;
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime);
        if(duration >=std::chrono::seconds(1)){
            startTime = std::chrono::high_resolution_clock::now();
            cout<<frame_count<<endl;
            frame_count=0;
        }
		CompletedRequestPtr &completed_request = std::get<CompletedRequestPtr>(msg.payload);

		libcamera::Request::BufferMap &buffers = completed_request->buffers;
		const libcamera::FrameBuffer *fb = buffers[app.ViewfinderStream()];
		cv::Mat frame = frameBufferToCvMat(app, *fb);
		if (frame.empty()) {
            std::cerr << "Error: Blank frame captured." << std::endl;
            break;
        }
        //tracking
        cv::resize(frame, frame, cv::Size(972,648));
        cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0);
        cv::Mat processing_frame = frame.clone();
        auto [x,y] = oneStepTracker(bg, processing_frame);
        cv::circle(frame, cv::Point(x, y), 8, cv::Scalar(255), 2);
		//cout<<x<<" "<<y<<endl;

		cv::imshow("test", frame);
        if(cv::waitKey(1) == 'q'){
            break;
        }
		//app.ShowPreview(completed_request, app.ViewfinderStream());
		
	}
}

int main(int argc, char *argv[])
{
	try
	{
		LibcameraApp app;
		Options *options = app.GetOptions();
		if (options->Parse(argc, argv))
		{
			if (options->verbose >= 2)
				options->Print();

			event_loop(app);
		}
	}
	catch (std::exception const &e)
	{
		LOG_ERROR("ERROR: *** " << e.what() << " ***");
		return -1;
	}
	return 0;
}
