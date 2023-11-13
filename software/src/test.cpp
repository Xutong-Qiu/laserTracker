#include <opencv4/opencv.hpp>
#include <core/libcamera_app.hpp>
#include <core/options.hpp>
#include <libcamera/framebuffer.h>
#include <sys/mman.h>
#include <chrono>
#include "background.h"
#include "util.h"
#include "myble.h"

using namespace std;
using namespace std::placeholders;
using namespace cv;


static void event_loop(LibcameraApp &app, SimpleBLE::Peripheral* nano33){
	Options const *options = app.GetOptions();

	app.OpenCamera();
	app.ConfigureVideo();
	app.StartCamera();
	
	Background bg(BACKGROUND_IMG_NUM);
    
    Mat frame;
    int frame_count = 0;
    auto startTime = std::chrono::high_resolution_clock::now();


	// for (unsigned int count = 0; ; count++){
	// 	LibcameraApp::Msg msg = app.Wait();
    //     if (msg.type == LibcameraApp::MsgType::Timeout)
	// 	{
	// 		LOG_ERROR("ERROR: Device timeout detected, attempting a restart!!!");
	// 		app.StopCamera();
	// 		app.StartCamera();
	// 		continue;
	// 	}
	// 	if (msg.type == LibcameraApp::MsgType::Quit)
	// 		return;
	// 	else if (msg.type != LibcameraApp::MsgType::RequestComplete)
	// 		throw std::runtime_error("unrecognised message!");
	// 	frame_count++;
    //     auto now = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime);
    //     if(duration >=std::chrono::seconds(1)){
    //         startTime = std::chrono::high_resolution_clock::now();
    //         cout<<frame_count<<endl;
    //         frame_count=0;
    //     }
	// 	CompletedRequestPtr &completed_request = std::get<CompletedRequestPtr>(msg.payload);

	// 	libcamera::Request::BufferMap &buffers = completed_request->buffers;
	// 	const libcamera::FrameBuffer *fb = buffers[app.VideoStream()];
	// 	cv::Mat frame = frameBufferToCvMat(app, *fb);
	// 	cv::resize(frame, frame, cv::Size(972,648));
    //     if (frame.empty()) {
    //         std::cerr << "Error: Blank frame captured." << std::endl;
    //         break;
    //     }
    //     bg.pinpointScreen(frame);
        
    //     cv::imshow("test", frame);
    //     if (cv::waitKey(1) == 'q' || bg.screen.size()==4) {
    //         break;
    //     }
    // }
	cout<<"Screen setup completed!"<<endl;
	bg.screen.push_back({300,600});
	bg.screen.push_back({300,100});
	bg.screen.push_back({900,100});
	bg.screen.push_back({900,600});
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
		const libcamera::FrameBuffer *fb = buffers[app.VideoStream()];
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
		//draw laser circle
        cv::circle(frame, cv::Point(x, y), 8, cv::Scalar(255), 2);
		//cout<<x<<" "<<y<<endl;

		//draw screen
		for(int i=0;i<4;i++){
            auto [s_x,s_y] = bg.screen[i];
            cv::circle(frame, cv::Point(s_x, s_y), 8, cv::Scalar(255), 2);
        }
		auto [norm_x,norm_y] = getLaserLocationNormalized(bg.screen,{x,y});
		cv::imshow("test", frame);		
		//send loc
		for(auto& service:nano33->services()){
			if(service.uuid()==SimpleBLE::BluetoothUUID("19b10010-e8f2-537e-4f6c-d104768a1214")){
				for(auto& charc: service.characteristics()){
					sendLocation(nano33, service, charc, (uint16_t)(norm_x*1000), (uint16_t)(norm_y*1000));
					break;
				}
				break;
			}
		}
		cout<<(uint16_t)(norm_x*1000)<<" "<<(uint16_t)(norm_y*1000)<<endl;
        if(cv::waitKey(1) == 'q'){
			nano33->disconnect();
            break;
        }
		//app.ShowPreview(completed_request, app.ViewfinderStream());
		
	}
}

int main(int argc, char *argv[])
{
	//connect nano33
	std::vector<SimpleBLE::Adapter> adapters = SimpleBLE::Adapter::get_adapters();

   // Get the first adapter
   SimpleBLE::Adapter adapter = adapters[0];

   // Scan for peripherals for 5000 milliseconds
   adapter.scan_for(5000);

   // Get the list of peripherals found
   std::vector<SimpleBLE::Peripheral> peripherals = adapter.scan_get_results();
   SimpleBLE::Peripheral* nano33 = NULL;
   // Print the identifier of each peripheral
   for (auto peripheral : peripherals) {     
      if(peripheral.address() == "C7:51:97:E1:67:11"){
         peripheral.connect();
         nano33 = &peripheral;
         break;
      }
   }
   if(!nano33){
      std::cout<<"Not connected!"<<endl;
      return 0;
   }
   for(auto& service:nano33->services()){
      if(service.uuid()==SimpleBLE::BluetoothUUID("19b10010-e8f2-537e-4f6c-d104768a1214")){
         cout<<"foundd service!"<<endl;
         for(auto& charc: service.characteristics()){
            sendLocation(nano33, service, charc, 123, 456);
            break;
         }
         break;
      }
   }
	//nano33->disconnect();
	//return 0;
   //start camera
	try
	{
		LibcameraApp app;
		Options *options = app.GetOptions();
		// char* new_argv[argc + 2];
		// new_argv[argc] = "-n";
    	// int new_argc = argc + 1;
		if (options->Parse(argc, argv))
		{
			options->Print();
			event_loop(app, nano33);
		}
	}
	catch (std::exception const &e)
	{
		LOG_ERROR("ERROR: *** " << e.what() << " ***");
		return -1;
	}
	return 0;
}
