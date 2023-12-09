# Table of Contents
* Abstract
* [Introduction](#1-introduction)
* [Related Work](#2-related-work)
* [Technical Approach](#3-technical-approach)
* [Evaluation and Results](#4-evaluation-and-results)
* [Discussion and Conclusions](#5-discussion-and-conclusions)
* [References](#6-references)

# Abstract

This project aims to develop a laser pointer tracking system for mouse cursor control on projection screens using a Raspberry Pi and an ESP32. It adapts real-time computer vision algorithms for efficient spot detection and cursor positioning on embedded systems to make a low-cost, intuitive interface that enhances interaction with digital content.

# 1. Introduction

## 1. Motivation & Objective

The primary goal of this project is to enable intuitive interaction with digital content on projection screens using laser pointers. Traditional input devices such as a mouse or a touchscreen require physical contact or proximity to the controlling interface. My approach aims to free the user from these constraints, allowing for remote interaction with digital interfaces, such as controlling a cursor on a screen with a laser pointer.

## 2. State of the Art & Its Limitations

Currently, interacting with projection screens typically relies on physical touch, such as touchscreens, or conventional pointing devices like mice and trackpads. While these methods are well-established, they often bind the user to a fixed location and surface. Alternative technologies, such as laser pointers equipped with clicking and scrolling capabilities, while freeing the user from these physical constraints, fall short when it comes to providing a control for a computer's cursor. As such, while these devices add convenience in certain contexts, they do not offer a comprehensive solution for interacting with digital content in a manner akin to a physical mouse.

In addition to these traditional methods, the market has seen the introduction of 'air mice' that employ an inertial measurement unit(IMU) to control the cursor. Despite their innovative approach to cursor control, freeing the user from the need to have a surface to navigate upon, they have not gained widespread popularity. Many users find that these devices are too large for comfortable use, offer limited accuracy, and suffer from reliability issues. This can lead to a frustrating user experience. Furthermore, because they require significant hand movement, they may not be suitable for all users or situations, such as during presentations where minimal movement is preferred.

The limitations of these existing technologies present a clear opportunity for innovation. By leveraging advanced computer vision techniques and embedded systems, we can create a more intuitive, accurate, and flexible method of interaction. This would not only enhance the user experience but also extend the contexts in which digital projection interactions can be effectively employed.

## 3. Novelty & Rationale

The project introduces a novel approach by integrating two compact computing platforms: the Raspberry Pi and the ESP32. Leveraging the Raspberry Pi's capacity for real-time video feed processing, the system employs background subtraction algorithms to precisely pinpoint the location of a laser pointer. This identified position is then translated into cursor coordinates.

What sets this system apart is the seamless communication established between the Raspberry Pi and the ESP32. After processing the video feed and locating the laser point, the Raspberry Pi computes the desired cursor location and transmits these coordinates to the ESP32. The ESP32, a device selected for its low power consumption and Bluetooth connectivity, receives the cursor position and directly interfaces with the computer to move the cursor to the calculated spot on the screen.

The use of the Raspberry Pi for complex computation and the ESP32 for cursor control not only makes this system highly efficient but also opens the door to greater accessibility and affordability for such technology. The rationale behind this dual-device approach is grounded in maximizing the strengths of both platforms: the Raspberry Pi's processing power for image analysis and the  ESP32's agility in peripheral control. With these combined capabilities, the project is poised for success, harnessing the recent advancements in computer vision and microcontroller technology to create a responsive and reliable user interface tool.

## 4. Potential Impact

The project simplifies the interaction between the presenter and digital content in educational, professional, and creative settings. By making intuitive interaction methods more affordable and accessible, it allows innovative uses in conference and educational settings.

## 5. Challenges

The foremost challenge lies in achieving real-time processing speeds to ensure a fluid user experience. The system is required to sustain a smooth video stream, ideally at 30 frames per second, which translates to the necessity for the algorithm to identify the laser point within a stringent timeframe of 30 milliseconds for each frame. Accomplishing this with the limited computational resources of the Raspberry Pi heightens the complexity of designing an efficient algorithm.

Further complicating the processing speed challenge is the recent obsolescence of the older Raspberry Pi camera libraries such as `raspistill` and `raspivid`. Their lack of support necessitates a transition to the newer `libcamera` library, which, despite being more modern and capable, has online limited resources due to its relative novelty in the field. This project requires pioneering work to develop an understanding and application of `libcamera` for real-time video processing.

In addition, another challenge of this project are tied to the hardware limitations of the Raspberry Pi and ESP32 in processing and communication, which might introduce potential inaccuracies in the laser pointer tracking system. These challenges collectively demand a robust solution that can adapt to varying operational conditions and technological constraints.

## 6. Requirements for Success

The successful execution of this project hinges on a multidisciplinary skill set. Proficiency in C++ is critical, not only for the development of the background subtraction algorithm but also for the implementation of efficient code that can operate within the stringent performance constraints of the Raspberry Pi. A thorough understanding of computer vision principles is equally essential, as is experience in handling real-time video processing and data communication between the Raspberry Pi and ESP32.

Given the project's dependence on specific camera libraries, the ability to navigate and adapt to the new `libcamera` library quickly is required. This involves an in-depth comprehension of the library's architecture and its application in the context of real-time image analysis and processing.

Experience with embedded systems is another cornerstone for success, particularly with configuring and optimizing the Raspberry Pi and ESP32 to communicate effectively while ensuring minimal latency and high reliability in cursor position control.

Access to suitable hardware, including the Raspberry Pi, ESP32, and a compatible camera module, is a prerequisite. The project also requires development tools and testing environments that can simulate projection scenarios to fine-tune the system's accuracy and responsiveness.

In addition to the technical skills and hardware, the project demands a problem-solving mindset to overcome the anticipated challenges, such as achieving real-time processing speed, managing ambient light interference, and ensuring compatibility with the newer `libcamera` library. The ability to innovate and iterate on the system design, based on testing feedback, will be critical for navigating the uncharted territory of `libcamera` implementation and achieving a successful project outcome.

## 7. Metrics of Success

The project's success will be gauged by several key performance indicators:

- **Responsiveness:** The system must maintain a minimum frame rate of 30 frames per second to ensure smooth cursor movement.
- **Accuracy:** Tracking accuracy is critical, with the system reliably pinpointing the laser's position to control the cursor effectively.
- **Reliability:** A stable BLE connection between the Raspberry Pi and the ESP32 is essential for consistent performance.

# 2. Related Work

In the previous of laser pointer tracking, the "Laser Pointer Tracker" [GitHub repository](https://github.com/sanette/laser) provides a Python implementation of a sophsiticated background substraction algorithm. This work is inspired by the scholarly work by Zakaria et al. (2014), titled "A Review on Laser Spot Detection System Based on Image Processing Techniques," which delves into the architecture and methodologies employed in laser spot detection systems, emphasizing the importance of low-level processing methods such as thresholding, background subtraction, region of interest extraction, and coordinate mapping. Based on the proposed algorithm, the "Laser Pointer Tracker" repository made several improvements on the original algorithm. It is able to detect lighting condition, drawing tail of pointer movement, and predict the pointer's position. While robust and versitile, the hardware requirement of the program is high, making it impossible to adapt the libary for our project. Desipte this,this repository's algorithms have informed the development of our tracking system, offering insights into efficient image processing and tracking techniques.

Another pivotal resource lays under the mouse controller is the "ESP32 BLE Absolute Mouse" [GitHub repository](https://github.com/sobrinho/ESP32-BLE-Abs-Mouse), which showcases the implementation of a BLE mouse using the ESP32's HID trackpad interface. This codebase was instrumental in understanding how to emulate a mouse device using ESP32 and provided a starting point for developing the BLE communication protocols necessary for our system.

These works collectively form the bedrock of our project's approach, providing both practical code examples and theoretical underpinnings that guide our development and innovation in laser tracking for mouse cursor control.

# 3. Technical Approach

The heart of the laser tracking mechanism is a highly efficient background subtraction algorithm running on the Raspberry Pi. This algorithm is responsible for distinguishing the laser pointer from the static background in real-time. By analyzing each video frame captured by the Pi's camera module, the algorithm efficiently isolates the bright spot of the laser, enabling precise detection and tracking. The computationally optimized process ensures rapid execution, crucial for maintaining the system's real-time performance. The algorithm's output is a set of coordinates representing the laser's position, which are then queued for transmission to ESP32.

To effectively manage the flow of location data to ESP32, a thread queue was implemented, enabling concurrent sending while avoid data competing. Before executing, threads are monitored for data expiry — if the sending attempt falls outside an acceptable time window (enqueue time versus sending time), the data is considered outdated and discarded to preserve the integrity of real-time interaction.

Building upon this tracking system, ESP32 is programmed using Arduino IDE. The major libary used is a customized mouse library using absolute positioning via the HID touchpad interface, which empowers the device to control the client's mouse cursor. Originally, this library was designed to handle one-way communication between ESP32 and client PC, allowing the ESP32 to function solely as a Bluetooth mouse. To align with the project's requirements for dual connectivity, the library's source code was modified to enable the ESP32 to establish and manage connections with both the Raspberry Pi for input data and the client's PC for cursor control. This adaptation was achieved by introducing additional services and characteristics to the BLE server, expanding its capabilities.

The Raspberry Pi's LibCamera library is used to handle raw camera feed processing. It stores images in the YUV420 format in the frame buffer, with the Y plane containing luminance data and the UV planes containing chrominance information. The system enhances performance by directly accessing the Y plane for the grayscale data necessary for laser tracking, thereby reducing computational overhead and achieving lower latency.

# 4. Evaluation and Results

The outcomes of this project are highlighted in a demo video. Key performance metrics of the current system include:

- **Responsiveness:** The optimized background subtraction algorithm on the Raspberry Pi generally maintains a stable 30 FPS. Based on our observation, occationally background changes, which result in background model rebuilding, can sometime bring down the frame rate down to 27 to 28 FPS, with a lowest of 25 FPS observed. However, the frame drops are bearly noticable given their rare occuration and short duration.
- **Accuracy:** Tracking precision is partially influenced by setup. Ideal scenarios involve full-screen camera coverage. However, even with partial coverage (600x500 pixels), the system accurately navigates the cursor, effectively clicking small interface elements like close and minimize buttons.
- **Reliability:** The system maintains a consistent BLE connection without disconnections during tests.
- **Convenience:** The client PC requires no setup; users connect to the device similar to pairing a Bluetooth mouse.


# 5. Discussion and Conclusions

The system has been successfully implemented and is fully operational, proving to be highly effective for presentation purposes. The setup, consisting of a Raspberry Pi camera module, a Raspberry Pi board, and an ESP32, showcases the feasibility of creating a real-time laser tracking system at a low cost. The outcome of this project indicates a potential for making the system even smaller and developing dedicated hardware that could be easier for users to handle and carry around.

A notable limitation, however, is the BLE-induced latency, emerging as the primary bottleneck. The time it takes for the `sendLocation` function to execute can take as long as 130ms, typically averaging around 100ms. This means that although the Raspberry Pi is capable of tracking the laser pointer's location with less than 30ms delay, the actual update rate of the cursor's position on the client's PC is limited to approximately 10 times per second. This limitation is due to BLE-induced latency. And because of the project's real-time nature, it rules out the possibility of batching data to reduce transmission frequency. Looking ahead, optimizing the system may involve creating a custom BLE library specifically for this application. This could potentially decrease latency and increase how often the cursor's position is updated. 

Another point to consider is the limitations imposed by the HID touchpad interface used by the mouse control library, which does not support certain functionalities that a mouse does, such as "hovering". This restricts the system from performing certain actions that are exclusive to mouse devices. An area for future enhancement could be to integrate the HID mouse and touchpad interfaces, providing a more comprehensive user experience.

Additionally, the user setup experience could be enhanced by attaching an LED display to the Raspberry Pi, which would provide visual feedback about the system's status and operations. Further improvements like these would enhance the system's promptness, providing a better user experience.

# 6. References

- Zakaria, N. F., Zulkifley, M. A., Mustafa, M. M., & Abdul Karim, R. (2014). A Review on Laser Spot Detection System Based on Image Processing Techniques. Journal of Theoretical and Applied Information Technology, 70(2), 333-342. Retrieved from [https://www.jatit.org/volumes/Vol70No2/18Vol70No2.pdf](https://www.jatit.org/volumes/Vol70No2/18Vol70No2.pdf)
- "Laser Pointer Tracker." GitHub repository. [https://github.com/sanette/laser](https://github.com/sanette/laser)
- "Libcamera Library." Libcamera Official Documentation. [https://libcamera.org](https://libcamera.org)
- "OpenCV Library." Open Source Computer Vision Library. [https://github.com/opencv/opencv](https://github.com/opencv/opencv)
- "Raspbian OS." Raspberry Pi Foundation. [https://www.raspberrypi.org/software/operating-systems/](https://www.raspberrypi.org/software/operating-systems/)
- "Arduino USBHID Library." Arduino USB HID Reference. [https://www.arduino.cc/reference/en/libraries/usbhid/](https://www.arduino.cc/reference/en/libraries/usbhid/)
- "SimpleBLE." Raspberry Pi BLE library. [https://github.com/OpenBluetoothToolbox/SimpleBLE](https://github.com/OpenBluetoothToolbox/SimpleBLE)
- "ArduinoBLE Library." Arduino BLE Reference. [https://www.arduino.cc/reference/en/libraries/arduinoble/](https://www.arduino.cc/reference/en/libraries/arduinoble/)
- "ESP32 BLE Absolute Mouse." GitHub repository. [https://github.com/sobrinho/ESP32-BLE-Abs-Mouse](https://github.com/sobrinho/ESP32-BLE-Abs-Mouse)
