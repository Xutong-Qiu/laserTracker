#include <opencv4/opencv.hpp>
#include <core/libcamera_app.hpp>
#include <core/options.hpp>
#include <libcamera/framebuffer.h>
#include <sys/mman.h>
#include <chrono>
#include "background.h"
#include "util.h"
#include "myble.h"
#include "constants.h"

using namespace std;
using namespace std::placeholders;
using namespace cv;


static void event_loop(LibcameraApp &app, SimpleBLE::Peripheral* mouse_controller){
	Background bg(BACKGROUND_IMG_NUM);
    
    Mat frame;
    int frame_count = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

	cout<<"Enter w to set up the screen positions or s to skip:"<<endl;
	char key = cin.get();

	Options const *options = app.GetOptions();

	app.OpenCamera();
	app.ConfigureVideo();
	app.StartCamera();
	
	if(key == 'w'){
		for (unsigned int count = 0; ; count++){
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

			// frame_count++;
			// auto now = std::chrono::high_resolution_clock::now();
			// auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime);
			// if(duration >=std::chrono::seconds(1)){
			// 	startTime = std::chrono::high_resolution_clock::now();
			// 	cout<<frame_count<<endl;
			// 	frame_count=0;
			// }

			CompletedRequestPtr &completed_request = std::get<CompletedRequestPtr>(msg.payload);
			libcamera::Request::BufferMap &buffers = completed_request->buffers;
			const libcamera::FrameBuffer *fb = buffers[app.VideoStream()];
			cv::Mat frame = frameBufferToCvMat(app, *fb);
			if (frame.empty()) {
				std::cerr << "Error: Blank frame captured." << std::endl;
				break;
			}
			cv::resize(frame, frame, cv::Size(MAT_WIDTH,MAT_HEIGHT));
			cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0);
			cv::imshow("test", frame);
			if (cv::waitKey(1) == 'q' || bg.pinpointScreen(frame)) {
				break;
			}
		}
    }else{
		bg.screen.push_back({100,50});
		bg.screen.push_back({950,50});
		bg.screen.push_back({950,620});
		bg.screen.push_back({100,620});
	}

	cout<<"Screen setup completed! The corner locations are: "<<endl;
	for(auto [x,y]:bg.screen){
		cout<<x<<" "<<y<<endl;
	}
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


		//auto start = std::chrono::high_resolution_clock::now();
		CompletedRequestPtr &completed_request = std::get<CompletedRequestPtr>(msg.payload);
		libcamera::Request::BufferMap &buffers = completed_request->buffers;
		const libcamera::FrameBuffer *fb = buffers[app.VideoStream()];
		cv::Mat frame = frameBufferToCvMat(app, *fb);
		if (frame.empty()) {
            std::cerr << "Error: Blank frame captured." << std::endl;
            break;
        }
		//cout<<"retrieving frame takes: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count()<<endl;
		
        //tracking
        cv::resize(frame, frame, cv::Size(MAT_WIDTH,MAT_HEIGHT));
        cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0);
        cv::Mat processing_frame = frame.clone();
        auto [x,y] = oneStepTracker(bg, processing_frame);
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
		//start = std::chrono::high_resolution_clock::now();	
		static uint16_t each_other = 0;
		static double cache_x = 0.0, cache_y = 0.0;
		each_other++;
		if(each_other==2){
			each_other = 0;
			norm_x += cache_x;
			norm_x/=2;
			norm_y += cache_y;
			norm_y/=2;
			sendLocation((uint16_t)(norm_x*CLIENT_WIDTH), (uint16_t)(norm_y*CLIENT_HEIGHT));
		}else{
			cache_x = norm_x;
			cache_y = norm_y;
		}
		//cout<<"sending takes: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count()<<endl;
		//cout<<(uint16_t)(norm_x*CLIENT_WIDTH)<<" "<<(uint16_t)(norm_y*CLIENT_HEIGHT)<<endl;
        if(cv::waitKey(1) == 'q'){
			mouse_controller->disconnect();
            break;
        }
		//app.ShowPreview(completed_request, app.ViewfinderStream());
		
	}
}

int main(int argc, char *argv[])
{
	//connecting esp32
	std::vector<SimpleBLE::Adapter> adapters = SimpleBLE::Adapter::get_adapters();

   // Get the first adapter
   SimpleBLE::Adapter adapter = adapters[0];
   adapter.scan_for(5000);

   // Get the list of peripherals found
   std::vector<SimpleBLE::Peripheral> peripherals = adapter.scan_get_results();
   SimpleBLE::Peripheral* mouse_controller = NULL;
   for (auto& peripheral : peripherals) {     
		//cout<<peripheral.address()<<peripheral.identifier()<<endl;
		if(peripheral.address() == "08:D1:F9:26:B5:E6"){//"C7:51:97:E1:67:11" nano33 address
			peripheral.connect();
			mouse_controller = &peripheral;
			break;
		}
   }
   if(!mouse_controller){
		peripherals = adapter.get_paired_peripherals();
		 for (auto& peripheral : peripherals) {     
				//cout<<peripheral.address()<<peripheral.identifier()<<endl;
			if(peripheral.address() == "08:D1:F9:26:B5:E6"){//"C7:51:97:E1:67:11" nano33 address
				peripheral.connect();
				mouse_controller = &peripheral;
				break;
			}
		}
		while(!mouse_controller){
			adapter.scan_for(5000);
			cout<<"Waiting for mouse controller..."<<endl;
			peripherals = adapter.get_paired_peripherals();
			for (auto& peripheral : peripherals) {     
					//cout<<peripheral.address()<<peripheral.identifier()<<endl;
				if(peripheral.address() == "08:D1:F9:26:B5:E6"){//"C7:51:97:E1:67:11" nano33 address
					peripheral.connect();
					mouse_controller = &peripheral;
					break;
				}
			}
		}
   }
   for(auto& service:mouse_controller->services()){
		if(service.uuid()==SimpleBLE::BluetoothUUID("19b10010-e8f2-537e-4f6c-d104768a1214")){
			cout<<"found service!"<<endl;
			for(auto& charc: service.characteristics()){
				if (charc.uuid() == SimpleBLE::BluetoothUUID("19b10010-e8f2-537e-4f6c-d104768a1214")) {
					cout << "Found characteristic!" << endl;
					initSending(mouse_controller, service, charc);
					break; 
				}
			}
			break;
      }
   }
//    while(1){
//    	sendLocation((uint16_t)1234, (uint16_t)1234,0);
// 	std::this_thread::sleep_for(std::chrono::seconds(2));
//    }
// 	nano33->disconnect();
// 	return 0;
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
			event_loop(app, mouse_controller);
		}
	}
	catch (std::exception const &e)
	{
		LOG_ERROR("ERROR: *** " << e.what() << " ***");
		return -1;
	}
	return 0;
}
