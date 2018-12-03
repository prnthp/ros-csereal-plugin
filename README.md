# ROS CSereal
<img alt="ROS-CSereal logo" src="https://prnthp.github.io/images/roscsereal.png" width="500" />
ROS Csereal Native Plugin for ROSSerial ↔ Unity
Built on top of rosserial_windows. Uses TCP for communication
<img alt="csreal screenshot" src="https://prnthp.github.io/images/csereal-screenshot.PNG" width="280" />
Features a simple drag-n-drop interface, making it super easy to implement ROS in your Unity3D project.

## Compilation
Precompiled x64 binary is provided in the Unity directory.

## Dependencies
- [rosserial](http://wiki.ros.org/rosserial)
- [osiris](https://github.com/prnthp/fillet): This is a helper package to maintain rosserial nodes, since as of April 2018, cannot re-establish connections with rosserial_windows. ROS Csereal can call the reaper service in the osiris package to kill rosserial nodes and revive them.

## Installation
- **rosserial.unitypackage** - Double click or drag into Unity Editor
- **ros-csereal-unity.dll** - Copy to your Unity project's folder (*NOT ./Assets*). That folder should *contain* the folder Assets.
