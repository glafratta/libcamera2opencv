# Raspberry PI libcamera to openCV library with callback

This is a wrapper around libcamera which makes it a lot easier to establish
a callback containing an openCV matrix. This can then be processed by opencv
and then displayed with QT.

## Prerequisites

```
apt install libopencv-dev libcamera-dev libqwt-qt5-dev qtdeclarative5-dev
```

## How to compile and run

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
