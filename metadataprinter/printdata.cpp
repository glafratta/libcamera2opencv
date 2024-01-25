#include "libcam2opencv.h"

#include <unistd.h>

struct MyCallback : Libcam2OpenCV::Callback {
    virtual void hasFrame(const cv::Mat &, const ControlList &metadata) {
	for (const auto &ctrl : metadata) {
	    const ControlId *id = controls::controls.at(ctrl.first);
	    const ControlValue &value = ctrl.second;
	    
	    std::cout << "\t" << id->name() << " = " << value.toString()
		      << std::endl;
	}
    }
};

// Main program
int main(int argc, char *argv[]) {
    Libcam2OpenCV camera;
    MyCallback myCallback;
    camera.registerCallback(&myCallback);
    camera.start();
    sleep(10);
    camera.stop();
    printf("\n");
}
