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
* __examples:__ projects built on enki library

  * __ros_example:__ line following robot simulator using Enki library.

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

### Build Enki Library

Change directory to the library directory:
```
cd .../AI-Line-Follower/enki
```
Build the library:
```
qmake
make
```

### Build "ros_example"

Change directory to the workspace directory:
```
cd .../AI-Line-Follower/examples/ros_example/catkin_ws
```
Build the ROS package:
```
catkin_make
```

## Run the simulation

Change directory to the workspace directory:
```
cd .../AI-Line-Follower/examples/ros_example/catkin_ws
```

Run roscore on one terminal:
```
roscore
```
*Note that you need to keep this running all the time on one terminal when running the simulation. You can exit this later by pressing ctrl+c.*


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
