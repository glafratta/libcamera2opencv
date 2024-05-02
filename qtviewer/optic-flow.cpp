#include "window.h"

#include <QApplication>
struct MyCallback : Libcam2OpenCV::Callback{
    Window* window = nullptr;


    void hasFrame(const cv::Mat &frame, const libcamera::ControlList &){
        //featureparams (shi-tomasi corner detection) : these are fed to goodfeatures to track
        //lk params

        //crop frame x2: L/R

        //convert to grey
        cv::Mat frame_grey;
        cv::cvtColor(frame, frame_grey, cv::COLOR_BGR2GRAY);

        //goodfeatures to track

        //while(image is there)
            //crop and convert to grey
            //calcopticlalflowpyrLK
        if (nullptr != window) {
			window->updateImage(frame_grey);
	    }
    }
};

// Main program
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	// create the window
	Window window;
	window.show();

	// execute the application
	return app.exec();
}
