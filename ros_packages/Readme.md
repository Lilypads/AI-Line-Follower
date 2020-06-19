# ROS Packages

This folder contains misc. ROS packages used for the project.

## Contents

* __line_react_control:__ this package is used to provide line following capability to the Gazebo simulation. It takes the line sensor values published by Gazebo as an input and publishes a motor control message

* __enki_line_react_control:__ this package is used to provide line following capability to the Enki simulation. It takes the line sensor values published by Enki as an input and publishes a motor control message

## How to use

Copy the package you want to use to src folder in a catkin workspace (.../catkin_ws/src)

cd to the workspace(.../catkin_ws)

Then build the package:
```
catkin_make
```

Always source the workspace before runnning to avoid ROS being unable to find the package
```
source devel/setup.bash
```

To run, ensure roscore and the Enki/Gazebo simulation is running. (See __simulators__ directory)
```
rosrun <package_name> <exe_name>
```
for example
```
rosrun enki_line_react_control line_react_control_node
```
or
```
rosrun line_react_control line_react_control_node
```
