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
    std::vector <cv::Point2f> corners;
    cv::Mat previousFrame_grey;

	virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
        cv::Mat frame_grey;
        std::vector <cv::Point2f> new_corners;
        std::vector <uchar> status;
        std::vector<float> err;
        cv::cvtColor(frame, frame_grey, cv::COLOR_RGB2GRAY);
        //QImage::Format f= QImage::Format_Grayscale8;
        if (it%60==0){ //resample corners every 2 seconds (30fps)
            corners.clear();
            cv::goodFeaturesToTrack(frame_grey, corners, MAX_CORNERS, QUALITY_LEVEL, MIN_DISTANCE,cv::noArray(), BLOCK_SIZE);
            printf("GFT\n");
        }
        if (it>0){
            cv::calcOpticalFlowPyrLK(previousFrame_grey, frame_grey, corners, new_corners, status, err);
            printf("LK\n");
        }
        else{
            status=std::vector<uchar>(corners.size(), 1);
        }

        std::vector <cv::Point2f> good_corners;
        //if (it==1){
        int i=0;
        printf("pre-fill in status\n");
        for (i; i<corners.size();i++){
            if (status[i]==1){
                good_corners.push_back(corners[i]);
            }
            cv::circle(frame, corners[i], RADIUS, cv::Scalar(0,0,0));
        }
        printf("corners %i\n", i);
            
            //cv::imwrite("sample.jpeg", frame);

        //}

	    if (nullptr != window) {
		    window->updateImage(frame);
	    }
        printf("updated %i\n", it);
        previousFrame_grey=frame_grey.clone();
        corners=good_corners;
        it++;
	}
    };

    Libcam2OpenCV camera;
    MyCallback myCallback;
};

#endif // WINDOW_H
