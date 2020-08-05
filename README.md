![Alt text](https://github.com/Lilypads/AI-Line-Follower/blob/master/line%20follower.png)
![Alt text](https://github.com/Lilypads/AI-Line-Follower/blob/master/braitenberg.png)

# ROS/ENKI Robot Simulation

This repository provides ROS (Robot Operating System) based robot simulation packages using Enki Library. Together provided custom rqt plugins can be used for viewing and testing neural network implementations for the robots.

## Content
* __catkin_ws:__ line following robot simulator using Enki library catkin workspace directory
  * __enki_braitenberg.launch:__ launch file for running many nodes at once, Braitenberg example
  * __enki_line_follow.launch:__ launch file for running many nodes at once, Line Following example
  * __src:__ directory containing different nodes
     * __enki_btb_react_control:__ subscribe to camera view, publish robot velocity, Braitenberg example
     * __enki_line_react_control:__ subscribe to camera view, publish robot velocity, Line Following example
     * __enki_ros_btb:__ enki environment and robot simulation, Braitenberg example
     * __enki_ros_pck:__ enki environment and robot simulation, Line Following example
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

Clone the library:
```
git clone https://github.com/berndporr/enki
```
Change directory to the library directory:
```
cd enki/enki
```
Make and install library:
```
qmake
make
sudo make install
```

## Build the repository workspace

Change directory to the workspace directory:
```
cd ROS-ENKI_robot_simulation/catkin_ws
```
Build the ROS package:
```
catkin_make
```

## Run the simulation

Change directory to the workspace directory:
```
cd ROS-ENKI_robot_simulation/catkin_ws
```

### Method1: Roslaunch

For Line Following example,
```
source devel/setup.bash
roslaunch enki_line_follow.launch
```
For Braitenberg example,
```
source devel/setup.bash
roslaunch enki_braitenberg.launch
```

### Method2: Manually run roscore and rosrun each node on seperate terminals

Run roscore on one terminal:
```
roscore
```

__2.1 Line Following Example__

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

__2.2 Braitenberg Example__

Run the enki simulation environment on the second terminal:
```
source devel/setup.bash
rosrun enki_ros_btb enki_ros_btb_node
```

Run the btb react control node on the third terminal:
```
source devel/setup.bash
rosrun enki_btb_react_control enki_btb_react_control_node
```


Run rqt on the forth terminal:
```
source devel/setup.bash
rqt --force-discover
```

Select plugins: __Plugins > Design Special Topic 5__

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
