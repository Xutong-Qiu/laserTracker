# Project Proposal

## 1. Motivation & Objective

The primary goal of this project is to enable intuitive interaction with digital content on projection screens using laser pointers. Traditional input devices such as a mouse or a touchscreen require physical contact or proximity to the controlling interface. Our approach aims to free the user from these constraints, allowing for remote interaction with digital interfaces, such as controlling a cursor on a screen with a laser pointer using a background subtraction algorithm.

## 2. State of the Art & Its Limitations

Currently, interacting with projection screens typically relies on physical touch, such as touchscreens, or conventional pointing devices like mice and trackpads. While these methods are well-established, they often bind the user to a fixed location and surface. Alternative technologies, such as laser pointers equipped with clicking and scrolling capabilities, while freeing the user from these physical constraints, fall short when it comes to providing a full-fledged control system for a computer's cursor. As such, while these devices add convenience in certain contexts, they do not offer a comprehensive solution for interacting with digital content in a manner akin to a mouse.

In addition to these traditional methods, the market has seen the introduction of 'air mice' that employ accelerometers to control the cursor. Despite their innovative approach to cursor control, freeing the user from the need to have a surface to navigate upon, they have not gained widespread popularity. Many users find that these devices are too large for comfortable use, offer limited accuracy, and suffer from reliability issues. This can lead to a frustrating user experience, particularly in precise or high-stakes applications. Furthermore, because they require significant hand movement, they may not be suitable for all users or situations, such as in tight spaces or during presentations where minimal movement is preferred.

The limitations of these existing technologies present a clear opportunity for innovation. By leveraging advanced computer vision techniques and embedded systems, we can create a more intuitive, accurate, and flexible method of interaction. This would not only enhance the user experience but also extend the contexts in which digital projection interactions can be effectively employed.

## 3. Novelty & Rationale

The project introduces a novel approach by integrating two compact yet powerful computing platforms: the Raspberry Pi and the Arduino Nano 33 BLE. Leveraging the Raspberry Pi's capacity for real-time video feed processing, the system employs background subtraction algorithms to precisely pinpoint the location of a laser pointer. This identified position is then translated into cursor coordinates.

What sets this system apart is the seamless communication established between the Raspberry Pi and the Nano 33 BLE. After processing the video feed and locating the laser point, the Raspberry Pi computes the desired cursor location and transmits these coordinates to the Nano 33 BLE. The Nano, a device selected for its low power consumption and Bluetooth connectivity, receives the cursor position and directly interfaces with the computer to move the cursor to the calculated spot on the screen.

The use of the Raspberry Pi for complex computation and the Nano 33 BLE for cursor control not only makes this system highly efficient but also opens the door to greater accessibility and affordability for such technology. The rationale behind this dual-device approach is grounded in maximizing the strengths of both platforms: the Raspberry Pi's processing power for image analysis and the Nano 33 BLE's agility in peripheral control. With these combined capabilities, the project is poised for success, harnessing the recent advancements in computer vision and microcontroller technology to create a responsive and reliable user interface tool.

## 4. Potential Impact

Technically, the project simplifies the interaction with digital content in educational, professional, and creative settings. Broadly, it democratizes technology by making advanced interaction methods more affordable and accessible, potentially leading to innovative uses in presentations, interactive art installations, and educational settings.

## 5. Challenges

The foremost challenge lies in achieving real-time processing speeds to ensure a fluid user experience. The system is required to sustain a smooth video stream, ideally at 30 frames per second, which translates to the necessity for the algorithm to identify the laser point within a stringent timeframe of 30 milliseconds for each frame. Accomplishing this with the limited computational resources of the Raspberry Pi heightens the complexity of designing an efficient algorithm.

Further complicating the processing speed challenge is the recent obsolescence of the older Raspberry Pi camera libraries such as `raspistill` and `raspivid`. Their lack of support necessitates a transition to the newer `libcamera` library, which, despite being more modern and capable, has scarce resources and community knowledge due to its relative novelty in the field. This project requires pioneering work to develop an understanding and application of `libcamera` for real-time video processing.

In addition, the inherent risks of this project are tied to the hardware limitations of the Raspberry Pi and Nano 33 BLE in processing and communication, which might introduce potential inaccuracies in the laser pointer tracking system. These challenges collectively demand a robust solution that can adapt to varying operational conditions and technological constraints.


## 6. Requirements for Success

Key skills include proficiency in C++ for algorithm development, understanding of computer vision, and experience with Raspberry Pi or similar embedded systems. Access to suitable hardware and development tools is also necessary.

## 6. Requirements for Success

The successful execution of this project hinges on a multidisciplinary skill set. Proficiency in C++ is critical, not only for the development of the background subtraction algorithm but also for the implementation of efficient code that can operate within the stringent performance constraints of the Raspberry Pi. A thorough understanding of computer vision principles is equally essential, as is experience in handling real-time video processing and data communication between the Raspberry Pi and Nano 33 BLE.

Given the project's dependence on specific camera libraries, the ability to navigate and adapt to the new `libcamera` library quickly is required. This involves an in-depth comprehension of the library's architecture and its application in the context of real-time image analysis and processing.

Experience with embedded systems is another cornerstone for success, particularly with configuring and optimizing the Raspberry Pi and Nano 33 BLE to communicate effectively while ensuring minimal latency and high reliability in cursor position control.

Access to suitable hardware, including the Raspberry Pi, Nano 33 BLE, and a compatible laser module, is a prerequisite. The project also necessitates development tools and testing environments that can simulate various lighting conditions and projection scenarios to fine-tune the system's accuracy and responsiveness.

In addition to the technical skills and hardware, the project demands a problem-solving mindset to overcome the anticipated challenges, such as achieving real-time processing speed, managing ambient light interference, and ensuring compatibility with the newer `libcamera` library. The ability to innovate and iterate on the system design, based on testing feedback, will be critical for navigating the uncharted territory of `libcamera` implementation and achieving a successful project outcome.


## 8. Execution Plan

The project will follow a structured approach to development, with the following key tasks laid out in the roadmap:

1. **Literature Review and Algorithm Selection:**
   Begin by conducting a thorough investigation into existing research and implementations of laser pointer tracking using computer vision. This will inform the selection of an appropriate tracking algorithm that has been proven effective, which can be further tailored to the specific needs of this project.

2. **Algorithm Adaptation and Development on PC:**
   With a suitable algorithm identified, the next step involves analyzing its components, simplifying the approach where possible, and making necessary adaptations to enhance performance. Initially, this will involve rewriting the algorithm from Python to C++ to take advantage of the performance gains C++ offers, and getting it to work in a PC environment, which provides a stable platform for development before moving to the constrained environment of the Raspberry Pi.

3. **Porting and Optimization on Raspberry Pi:**
   Once the algorithm performs effectively on a PC, the focus will shift to porting the code to the Raspberry Pi OS. The initial port will be followed by rigorous benchmarking and optimization to ensure that the algorithm runs efficiently within the hardware limitations of the Raspberry Pi, achieving the real-time processing speeds required for a smooth user experience.

4. **Understanding BLE Communication:**
   In parallel with the computer vision tasks, undertake a detailed study of BLE (Bluetooth Low Energy) communication protocols, focusing on the central and peripheral model. This research will provide the necessary knowledge to establish reliable communication between the Raspberry Pi and the Nano 33 BLE.

5. **Implementation of BLE Communication:**
   Develop the code necessary for enabling the Raspberry Pi to send cursor location data to the Nano 33 BLE. This involves ensuring that the data packets are sent promptly and received accurately, forming a responsive control system for the cursor.

6. **Integration and System Testing:**
   With both the tracking algorithm and BLE communication established, the final step involves integrating these two components. After integration, comprehensive system testing is crucial to validate the real-time performance of the tracking and to ensure the system's overall reliability and responsiveness.

7. **Iteration and Refinement:**
   Based on feedback from system testing, iterate on the design and implementation to refine the user experience. This may involve further optimization of the tracking algorithm or improvements in the BLE communication protocol.

Each of these tasks represents a milestone in the project’s development, requiring a breadth of skills from algorithm development to embedded system communication. Regular reviews will ensure that the project stays on track and that any unforeseen challenges are addressed promptly.


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

