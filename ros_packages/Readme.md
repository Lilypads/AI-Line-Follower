# ROS line following Packages

## Contents

* __line_react_control:__ this package is used to provide line following capability to the Gazebo simulation. It takes the line sensor values published by Gazebo as an input and publishes a motor control message

* __enki_line_react_control:__ this package is used to provide line following capability to the Enki simulation. It takes the line sensor values published by Enki as an input and publishes a motor control message

## How to use the packages

1. Copy the package to src directory of a catkin workspace:

*Note: Do not copy both packages to the same workspace as it will cause error when building the package.

*Note2: The package does not have to be in the same workspace as your simulation.

*Note3: Assume you clone this repository to ~/ or Home directory as instructed on first README. Otherwise, change ~/AI-Line-Follower/ros_packages/ to where you clone the directory to on your PC.

You can do that by system files program or doing the following command:

* In case creating a new workspace (~/catkin_ws)
```
cd ~
mkdir catkin_ws
cd catkin_ws
mkdir src
```
 for Enki
```
cp -r ~/AI-Line-Follower/ros_packages/enki_line_react_control ~/catkin_ws/src
```
 for Gazebo
```
cp -r ~/AI-Line-Follower/ros_packages/line_react_control ~/catkin_ws/src
```
* If already have a workspace

 for Enki
```
cp -r ~/AI-Line-Follower/ros_packages/enki_line_react_control ~/path_to_src_of_workspace
```
 for Gazebo
```
cp -r ~/AI-Line-Follower/ros_packages/line_react_control ~/path_to_src_of_workspace
```

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

*Note2: You should always source a workspace everytime before running it to avoid ROS being unable to find the package. However, building it again is unnecessary.

5. Run the package:
```
rosrun <package_name> <exe_name>
```
for Enki
```
rosrun enki_line_react_control line_react_control_node
```
for Gazebo
```
rosrun line_react_control line_react_control_node
```
