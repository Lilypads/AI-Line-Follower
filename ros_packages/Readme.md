# ROS Packages

This folder contains misc. ROS packages used for the project.

## Contents

* __line_react_control:__ this package is used to provide line following capability to the Gazebo simulation. It takes the line sensor values published by Gazebo as an input and publishes a motor control message

* __enki_line_react_control:__ this package is used to provide line following capability to the Enki simulation. It takes the line sensor values published by Enki as an input and publishes a motor control message

## How to use the packages

1. Copy the package to src directory of a catkin workspace

*Note: Do not copy both packages to the same workspace as it will cause error when building the package.

*Note2: The package does not have to be in the same workspace as your simulation.

__You can do that by system files program or doing the following command:__

1.1 In case creating a new workspace (~/catkin_ws)
```
cd ~
mkdir catkin_ws
cd catkin_ws
mkdir src
```
1.1.1 for Enki
```
cp -r ~/AI-Line-Follower/ros_packages/enki_line_react_control ~/catkin_ws/src
```
1.1.2 for Gazebo
```
cp -r ~/AI-Line-Follower/ros_packages/line_react_control ~/catkin_ws/src
```
1.2 If already have a workspace

1.2.1 for Enki
```
cp -r ~/AI-Line-Follower/ros_packages/enki_line_react_control ~/path_to_src_of_workspace
```
1.2.2 for Gazebo
```
cp -r ~/AI-Line-Follower/ros_packages/line_react_control ~/path_to_src_of_workspace
```

*Note: Assume you clone this repository to ~/ or Home directory as instructed on first README. Otherwise, change ~/AI-Line-Follower/ros_packages/ to where you clone the directory to on your PC.

2. Change directory to the workspace:
```
cd ~/catkin_ws
```
or
```
cd ~/path_to_workspace
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
