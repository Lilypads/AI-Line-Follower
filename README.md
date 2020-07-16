# Line following robot with ROS/ENKI

This repository provides a ROS (Robot Operating System) based framework for simulating line following robot using Enki Library. Together provided custom rqt plugins can be used for viewing and testing neural network implementations for the line following robot.

## About Enki (QT5)

a fast 2D robot simulator using the QT5 build system.

© 1999-2017 Stéphane Magnenat and others ([full list](AUTHORS))

© 2017 Bernd Porr <mail@berndporr.me.uk>


### License

[GPL 2.0](LICENSE).

## Content
* __enki:__ enki library provided by the original project
* __catkin_ws:__ line following robot simulator using Enki library.
  * __src:__ directory containing different nodes
     * __enki_line_react_control:__ subscribe to camera view, publish robot velocity
     * __line_follower:__ enki environment and robot simulation
     * __rqt_line_sensor_control:__ custom rqt from __Design Special Topic 5__
     * __rqt_line_sensor_view:__ custom rqt from __Design Special Topic 5__
     * __rqt_neural_net_control:__ custom rqt from __Design Special Topic 5__
     * __rqt_neural_network_diagnostics:__ custom rqt from __Design Special Topic 5__

## Prerequisites

### Install ROS Melodic

 * Note that each platform might have a slightly different installation method.
 See http://wiki.ros.org/melodic/Installation for supported platforms and their installation method.

The following method is specific for Ubuntu18.04(Bionic) platform.

1. Add GPG key of ROS to apt

        sudo wget -O - https://raw.githubusercontent.com/ros/rosdistro/master/ros.key | sudo apt-key add -

2. Add apt repository for ROS

        sudo apt-add-repository "deb http://packages.ros.org/ros/ubuntu bionic main"

3. Update repositories

        sudo apt-get update

4. Install the ROS Desktop

        sudo apt install ros-melodic-desktop-full

5. Set your environment variables

        echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc
        source ~/.bashrc

6. Initialize ROS

        sudo apt install python-rosdep
        sudo rosdep init
        rosdep update
        
### Install qt5
```
sudo apt-get install qt5-default
```

### Install Enki Library

Clone enki library from https://github.com/berndporr/enki

Change directory to the library directory:
```
cd enki-master/enki
```
Make and install library:
```
qmake
sudo make install
```

## Build the repository workspace

Change directory to the workspace directory:
```
cd AI-Line-Follower/catkin_ws
```
Build the ROS package:
```
catkin_make
```

## Run the simulation

Change directory to the workspace directory:
```
cd AI-Line-Follower/catkin_ws
```

### Method1: Roslaunch

```
source devel/setup.bash
roslaunch enki_line_follow.launch
```
*Note: do expect one node to fail due to texture file(cc.png) not found when using roslaunch. To run the node again using rosrun on another terminal:
```
source devel setup.bash
rosrun enki_ros_pck robot
```

### Method2: Manually run roscore and rosrun each node on seperate terminals

Run roscore on one terminal:
```
roscore
```

Run the enki simulation environment on the second terminal:
```
source devel/setup.bash
rosrun enki_ros_pck robot
```

Run the line react control node on the third terminal:
```
source devel/setup.bash
rosrun enki_line_react_control line_react_control_node
```

Run rqt on the forth terminal:
```
source devel/setup.bash
rqt --force-discover
```

Select plugins:
__Plugins > Design Special Topic 5__

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
