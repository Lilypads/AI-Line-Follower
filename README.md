# AI bot with ROS

This repository provides a ROS (Robot Operating System) based framework for development and testing of neural network implementations with a line following robot. The line following robot can be realised as either a simulation or as a physical robot.

## Prerequisites

Install ROS:

1. Add GPG key of ROS to apt

        sudo wget -O - https://raw.githubusercontent.com/ros/rosdistro/master/ros.key | sudo apt-key add -

2. Add apt repository for ROS

        sudo apt-add-repository "deb http://packages.ros.org/ros/ubuntu bionic main"

3. Update repositories

        sudo apt-get update

4. Install the Ros Desktop

	sudo apt install ros-melodic-desktop

5. Initialize ROS

        sudo rosdep init
        rosdep update



## Contents

* __rqt_plugins:__ this directory contains the custom rqt GUI plugins used for the project

* __ros_packages:__ this directory contains some misc. ROS packages useful for the project

* __physical_robot:__ this directory contains work related with the development of the physical robot for the project

* __simulators:__ this directory contains simulator packages used in the framework

* __neural_networks:__ this directory contains the neural network implementations used in the project

* __interface_specifications:__ this directory contains specifications for module interfaces for the framework



## How to use

Instructions for using each section of the framework can be found in the respective folder.

__This repository is also dependent on submodules so when cloning use:__
```
git clone https://github.com/a2198699s/AI-Line-Follower --recurse-submodules
```
