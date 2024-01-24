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

#undef signals
#undef slots
#undef emit
#undef foreach

#include <libcamera/libcamera.h>

#define TIMEOUT_SEC 3

using namespace libcamera;

class Libcam2OpenCV {
public:
    struct Callback {
	virtual void hasFrame(const cv::Mat &frame) = 0;
	virtual ~Callback() {};
    };
    
    std::shared_ptr<Camera> camera;
    std::map<FrameBuffer *, std::vector<libcamera::Span<uint8_t>>> mapped_buffers;
    std::unique_ptr<CameraConfiguration> config;
    cv::Mat frame;
    Callback* callback = nullptr;
    FrameBufferAllocator* allocator = nullptr;
    Stream *stream = nullptr;
    std::unique_ptr<CameraManager> cm;
    std::vector<std::unique_ptr<Request>> requests;

    void registerCallback(Callback* cb) {
	callback = cb;
    }

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
     *
     * The Slot is invoked in the CameraManager's thread, hence one should avoid
     * any heavy processing here. The processing of the request shall be re-directed
     * to the application's thread instead, so as not to block the CameraManager's
     * thread for large amount of time.
     *
     * The Slot receives this and the Request as a parameter.
     */
    void requestComplete(Request *request);


    /*
     * ----------------------------------------------------------------------------
     * Camera Naming.
     *
     * Applications are responsible for deciding how to name cameras, and present
     * that information to the users. Every camera has a unique identifier, though
     * this string is not designed to be friendly for a human reader.
     *
     * To support human consumable names, libcamera provides camera properties
     * that allow an application to determine a naming scheme based on its needs.
     *
     * In this example, we focus on the location property, but also detail the
     * model string for external cameras, as this is more likely to be visible
     * information to the user of an externally connected device.
     *
     * The unique camera ID is appended for informative purposes.
     */
    std::string cameraName(Camera *camera)
    {
	const ControlList &props = camera->properties();
	std::string name;
	
	const auto &location = props.get(properties::Location);
	if (location) {
	    switch (*location) {
	    case properties::CameraLocationFront:
		name = "Internal front camera";
		break;
	    case properties::CameraLocationBack:
		name = "Internal back camera";
		break;
	    case properties::CameraLocationExternal:
		name = "External camera";
		const auto &model = props.get(properties::Model);
		if (model)
		    name = " '" + *model + "'";
		break;
	    }
	}
	
	name += " (" + camera->id() + ")";
	
	return name;
    }

    void start();
    void stop();
};

#endif
