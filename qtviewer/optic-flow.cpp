#include "window.h"

#include <QApplication>
// struct CameraCallback : public Window::MyCallback{
//     Window* window = nullptr;


//     virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &){
//         //featureparams (shi-tomasi corner detection) : these are fed to goodfeatures to track
//         //lk params

//         //crop frame x2: L/R

//         //convert to grey
//         cv::Mat frame_grey;
//         cv::cvtColor(frame, frame_grey, cv::COLOR_BGR2GRAY);

//         //goodfeatures to track

//         //while(image is there)
//             //crop and convert to grey
//             //calcopticlalflowpyrLK
// 		printf("oi ");
//         if (nullptr != window) {
// 			printf("hey ");
// 			window->updateImage(frame_grey);
// 	    }
//     }
// };

// Main program
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	// create the window
	Window window;
	//window.myCallback=cameraCallback;
	window.show();

	// execute the application
	return app.exec();
}
