# Raspberry PI libcamera to openCV library with callback

This is a wrapper around libcamera which makes it a lot easier to establish
a callback containing an openCV matrix. This can then be processed by opencv
and then displayed with QT.

## Prerequisites

```
apt install libopencv-dev libcamera-dev libqwt-qt5-dev qtdeclarative5-dev
```

## How to compile and run the example

### QT camera viewer with callback

```
cmake .
make
./webcam-viewer
```

### Metadata printer with callback

```
cd metadataprinter
cmake .
make
./printdata
```

## How to use the libbrary

 1. Include `libcam2opencv.h` and add `libcam2opencv.cpp` to your project.

 2. Create your custom callback
```
    struct MyCallback : Libcam2OpenCV::Callback {
        Window* window = nullptr;
        virtual void hasFrame(const cv::Mat &frame, const ControlList &) {
            if (nullptr != window) {
                window->updateImage(frame);
            }
        }
    };
```

 3. Create instances of the the camera and the callback:

```
Libcam2OpenCV camera;
MyCallback myCallback;
```

 4. Register the callback

```
camera.registerCallback(&myCallback);
```

 5. Start the camera delivering frames via the callback

```
camera.start();
```

 6. Stop the camera

```
camera.stop();
```
