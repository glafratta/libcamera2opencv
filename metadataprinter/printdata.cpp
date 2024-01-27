#include "libcam2opencv.h"
#include <libcamera/libcamera.h>

#include <unistd.h>

struct MyCallback : Libcam2OpenCV::Callback {
    virtual void hasFrame(const cv::Mat &, const libcamera::ControlList &metadata) {
	for (const auto &ctrl : metadata) {
	    const libcamera::ControlId *id = libcamera::controls::controls.at(ctrl.first);
	    const libcamera::ControlValue &value = ctrl.second;
	    
	    std::cout << "\t" << id->name() << " = " << value.toString()
		      << std::endl;
	}
	std::cout << std::endl;
    }
};

// Main program
int main(int argc, char *argv[]) {
    Libcam2OpenCV camera;
    MyCallback myCallback;
    camera.registerCallback(&myCallback);
    Libcam2OpenCVSettings settings;
    settings.framerate = 30;
    camera.start();
    getchar();
    camera.stop();
    printf("\n");
}
