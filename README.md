# ros-csereal-plugin
ROS Csereal Native Plugin for ROSSerial &lt;-> Unity
Built on top of rosserial_windows. Uses TCP for communication

## Compilation
Precompiled x64 binary is provided in the Unity directory.

## Dependencies
- [http://wiki.ros.org/rosserial](rosserial)
- [https://github.com/prnthp/fillet](osiris): This is a helper package to maintain rosserial nodes, since as of April 2018, cannot re-establish connections with rosserial_windows. ROS Csereal can call the reaper service in the osiris package to kill rosserial nodes and revive them.

## Installation
- **rosserial.unitypackage** - Double click or drag into Unity Editor
- **ros-csereal-unity.dll** - Copy to your Unity project's folder (*NOT ./Assets*). That folder should *contain* the folder Assets.
