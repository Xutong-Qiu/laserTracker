# Project Proposal

## 1. Motivation & Objective

The primary goal of this project is to enable intuitive interaction with digital content on projection screens using laser pointers. Traditional input devices such as a mouse or a touchscreen require physical contact or proximity to the controlling interface. Our approach aims to free the user from these constraints, allowing for remote interaction with digital interfaces, such as controlling a cursor on a screen with a laser pointer using a background subtraction algorithm.

## 2. State of the Art & Its Limitations

Current methods for interacting with projection screens often involve physical touch (touchscreens) or traditional pointing devices (mice, trackpads). Infrared and camera-based tracking systems exist but can be costly and complex. These systems often struggle with ambient light interference and require direct line-of-sight, limiting their practicality in everyday use.

## 3. Novelty & Rationale

The novelty of our project lies in utilizing an embedded system, specifically a Raspberry Pi, to process real-time video feeds for background subtraction to identify a laser point's location. The use of an accessible and cost-effective platform like Raspberry Pi makes this technology more accessible. Our confidence in the approach's success stems from advances in computer vision and embedded systems that make real-time processing feasible on such compact hardware.

## 4. Potential Impact

Technically, the project simplifies the interaction with digital content in educational, professional, and creative settings. Broadly, it democratizes technology by making advanced interaction methods more affordable and accessible, potentially leading to innovative uses in presentations, interactive art installations, and educational settings.

## 5. Challenges

Challenges include ensuring real-time processing speeds, managing ambient light interference, and calibrating the system for different projection sizes and geometries. Risks involve hardware limitations and potential inaccuracies in pointer tracking.

## 6. Requirements for Success

Key skills include proficiency in C++ for algorithm development, understanding of computer vision, and experience with Raspberry Pi or similar embedded systems. Access to suitable hardware and development tools is also necessary.

## 7. Metrics of Success

Success will be measured by the system's responsiveness, accuracy of pointer tracking, and the ability to operate under various lighting conditions and distances.

## 8. Execution Plan

The project will be divided into stages: algorithm development, system integration, testing and calibration, and user testing. For a team project, tasks will be divided into algorithm development, hardware setup, integration, and testing phases.

## 9. Related Work

### 9.a. Papers

- 

### 9.b. Datasets

- Background subtraction dataset from XYZ lab for algorithm training and testing.

### 9.c. Software

- libcamera for Pi camera bottom-level API.
- libcamera-app for Pi camera stream processing.
- OpenCV for computer vision processing.
- Raspbian OS for the Raspberry Pi
- Arduino IDE for Arduino Nano 33 BLE programming

## 10. References

- \[URL\]
- \[URL\]
- \[URL\]
- "OpenCV Library." OpenCV. \[URL\]
- "Raspbian OS." Raspberry Pi Foundation. \[URL\]

