#include "libcam2opencv.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

struct MyCallback : Libcam2OpenCV::Callback {
        virtual void hasFrame(const cv::Mat &frame, const ControlList &) {
	    printf(".");
	    cv::imshow("Video",frame);
        }
    };

int main(int, char**) {
    cv::namedWindow("Video", cv::WINDOW_NORMAL);

    Libcam2OpenCV camera;
    MyCallback myCallback;
    camera.registerCallback(&myCallback);
    camera.start();

    while (true) {
	if (cv::waitKey() == 27) break;
    }

    camera.stop();
    return 0;
}
