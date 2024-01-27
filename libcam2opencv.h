#ifndef __LIBCAM2OPENCV
#define __LIBCAM2OPENCV

/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2020, Ideas on Board Oy.
 * Copyright (C) 2024, Bernd Porr
 * Copyright (C) 2021, kbarni https://github.com/kbarni/
 */

#include <iomanip>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <memory>
#include <sys/mman.h>
#include <opencv2/opencv.hpp>

// need to undefine QT defines here as libcamera uses the same expressions (!).
#undef signals
#undef slots
#undef emit
#undef foreach

#include <libcamera/libcamera.h>

using namespace libcamera;

/**
 * Settings
 **/
struct Libcam2OpenCVSettings {
    /**
     * Width of the video capture. A zero lets libcamera decide the width.
     **/
    unsigned int width = 0;
    
    /**
     * Height of the video capture. A zero lets libcamera decide the height.
     **/
    unsigned int height = 0;

    /**
     * Framerate. A zero lets libcamera decide the framerate.
     **/
    unsigned int framerate = 0;
};

class Libcam2OpenCV {
public:
    struct Callback {
	virtual void hasFrame(const cv::Mat &frame, const ControlList &metadata) = 0;
	virtual ~Callback() {}
    };

    /**
     * Register the callback for the frame data
     **/
    void registerCallback(Callback* cb) {
	callback = cb;
    }

    /**
     * Starts the camera and the callback at default resolution and framerate
     **/
    void start(Libcam2OpenCVSettings settings = Libcam2OpenCVSettings() );

    /**
     * Stops the camera and the callback
     **/
    void stop();
    
private:
    std::shared_ptr<Camera> camera;
    std::map<FrameBuffer *, std::vector<libcamera::Span<uint8_t>>> mapped_buffers;
    std::unique_ptr<CameraConfiguration> config;
    cv::Mat frame;
    Callback* callback = nullptr;
    FrameBufferAllocator* allocator = nullptr;
    Stream *stream = nullptr;
    std::unique_ptr<CameraManager> cm;
    std::vector<std::unique_ptr<Request>> requests;
    ControlList controls;

    std::vector<libcamera::Span<uint8_t>> Mmap(FrameBuffer *buffer) const
    {
	auto item = mapped_buffers.find(buffer);
	if (item == mapped_buffers.end())
	    return {};
	return item->second;
    }

    /*
     * --------------------------------------------------------------------
     * Handle RequestComplete
     *
     * For each Camera::requestCompleted Signal emitted from the Camera the
     * connected Slot is invoked.
     */
    void requestComplete(Request *request);

    std::string cameraName(Camera *camera)
    {
	return camera->id();
    }
};

#endif
