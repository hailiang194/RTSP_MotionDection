# RTSP_MotionDection

Via RTPS, detect and record the motion 

## Dependencies

* Visual Studio 2022 - MICROSOFT VISUAL C++ 2015 - 2022 RUNTIME License
* GStreamer - version 1.24.12 - LGPL License
* OpenCV - version 4.10.0 - Apache License


## Features

* Via RTPS, detect and record the motion

## Configuration

Build the project with configuration mode is Release with x64

## Execution
```
RTSP_MotionDection.exe <GStreamer pipelines before default appsink plugin>
```

**Note**: The actual GStreamer pipeline is going to be ```<GStreamer pipelines before default appsink plugin>  ! appsink name=mysink max-buffers=2 sync=1 caps=video/x-raw,format=BGR```. All the motion detection videos are stored in Output\ directory

## Getting help

If you have questions, concerns, bug reports, etc, please file an issue in this repository's Issue Tracker.
