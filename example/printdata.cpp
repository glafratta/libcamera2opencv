#include "libcam2opencv.h"

#include <unistd.h>

struct MyCallback : Libcam2OpenCV::Callback {
    virtual void hasFrame(const cv::Mat &frame) {
	printf(".");
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
