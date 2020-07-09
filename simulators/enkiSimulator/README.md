# Enki Line following robot simulation

## About Enki (QT5)

a fast 2D robot simulator using the QT5 build system.

© 1999-2017 Stéphane Magnenat and others ([full list](AUTHORS))

© 2017 Bernd Porr <mail@berndporr.me.uk>


### License

[GPL 2.0](LICENSE).

## Content
* __enki:__ enki library provided by the original project
* __examples:__ projects built on enki library

  * __ros_example:__ line following robot simulator using Enki library.

## How to run simulation

### Prerequisite

Install qt5:
```
sudo apt-get install qt5-default
```

### Build Enki Library

Change directory to the library directory:
```
cd ~/AI-Line-Follower/simulators/enkiSimulator/enki
```
Build the library:
```
qmake
make
```

### Build "ros_example"

Empty the catkin_ws/devel and catkin_ws/build folders due to saved user specific cache stuff which will cause error when building ROS package on a different PC. You can do that by system files program or doing the following command:
```
cd ~/AI-Line-Follower/simulators/enkiSimulator/examples/ros_example/catkin_ws
rm -r devel
rm -r build
mkdir devel
mkdir build
```

Build the ROS package:
```
catkin_make
```

### Run the simulation

Run roscore on one terminal:
```
roscore
```
*Note that you need to keep this running all the time on one terminal when running the simulation. You can exit this later by pressing ctrl+c.*

Change directory to __ros_example__ catkin workspace folder.
```
cd ~/AI-Line-Follower/simulators/enkiSimulator/examples/ros_example/catkin_ws
```
Source the workspace:
```
source devel/setup.bash
```
*Note that you should always source a workspace everytime before running it to avoid ROS being unable to find the package.*

Run the simulation:
```
rosrun enki_ros_pck robot
```

__The robot window should show up, but not yet moving.__

* To make the robot move following the line, go to __ros_packages__ directory and follow the README instructions there.

* To open the custom rqt plugins in rqt gui windows, go to __rqt_plugins__ directory and follow the README instructions there.

### To check which node is currently running

On another terminal:
```
rosnode list
```
### To view communication between nodes using rqt_graph

On another terminal:
```
rosrun rqt_graph rqt_graph
```
