#ifndef WINDOW_H
#define WINDOW_H

#include <qwt/qwt_thermo.h>

#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "libcam2opencv.h"
#include <opencv2/core.hpp>
#include <vector>

// class definition 'Window'
class Window : public QWidget
{
    // must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
    Q_OBJECT

public:
    Window();
    
    ~Window();
    void updateImage(const cv::Mat &mat, QImage::Format f=QImage::Format_RGB888);

    QwtThermo    *thermo;
    QHBoxLayout  *hLayout;  // horizontal layout
    QLabel       *image;

    struct MyCallback : Libcam2OpenCV::Callback {
	Window* window = nullptr;
    int it=0;
    int MAX_CORNERS=100;
    float QUALITY_LEVEL=0.5;
    int MIN_DISTANCE=70;
    int BLOCK_SIZE=7;
    float RADIUS=5;

	virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
        it++;
        cv::Mat frame_grey;
        std::vector <cv::Point2f> corners;
        cv::cvtColor(frame, frame_grey, cv::COLOR_RGB2GRAY);
        QImage::Format f= QImage::Format_Grayscale8;
        cv::goodFeaturesToTrack(frame_grey, corners, MAX_CORNERS, QUALITY_LEVEL, MIN_DISTANCE,cv::noArray(), BLOCK_SIZE);

        if (it==1){
            // printf("frame col %i and rows %i, channels %i, size channel:%i, size elem: %i\n", 
            // frame.cols, frame.rows, frame.channels(), frame.elemSize1(), frame.elemSize());
            // printf("frame_grey col %i and rows %i, channels %i, size channel:%i, size elem: %i\n", 
            // frame_grey.cols, frame_grey.rows, frame_grey.channels(), frame_grey.elemSize1(), frame_grey.elemSize());
            for (cv::Point2f p:corners){
                cv::circle(frame, p, RADIUS, cv::Scalar(0,0,255));
            }
            cv::imwrite("sample.jpeg", frame);

        }

	    if (nullptr != window) {
		window->updateImage(frame_grey, f);
	    }
	}
    };

    Libcam2OpenCV camera;
    MyCallback myCallback;
};

#endif // WINDOW_H
