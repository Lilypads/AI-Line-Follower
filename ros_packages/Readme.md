# ROS Packages

This folder contains misc. ROS packages used for the project.

## Contents

* __line_react_control:__ this package is used to provide line following capability to the Gazebo simulation. It takes the line sensor values published by Gazebo as an input and publishes a motor control message

* __enki_line_react_control:__ this package is used to provide line following capability to the Enki simulation. It takes the line sensor values published by Enki as an input and publishes a motor control message

## How to use the packages

1. Copy the packages to src directory of a catkin workspace. Assuming creating a new one in this case (~/catkin_ws):
```
cd ~
mkdir catkin_ws
cd catkin_ws
mkdir src
cp -r ~/AI-Line-Follower/ros_packages/* ~/catkin_ws/src
```
*Note that if you already have a workspace, just do the cp line and change the ~/catkin_ws/src to your workspace

**Assume you clone this repository to ~/ or Home directory as instructed on first README, else just change ~/AI-Line-Follower/ros_packages/ to where the directory is and add * at the end.

2. cd to the workspace:
```
cd ~/catkin_ws
```
3. Build the package:
```
catkin_make
```
4. Source the workspace:
```
source devel/setup.bash
```

*Note: ensure roscore and the Enki/Gazebo simulation is running on seperate terminals. (See __simulators__ directory)

5. Run the package:
```
rosrun <package_name> <exe_name>
```
5.1 for Enki
```
rosrun enki_line_react_control line_react_control_node
```
5.2 for Gazebo
```
rosrun line_react_control line_react_control_node
```
