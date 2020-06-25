# AI bot with ROS

This repository provides a ROS (Robot Operating System) based framework for development and testing of neural network implementations with a line following robot. The line following robot can be realised as either a simulation or as a physical robot.

## Prerequisites

Install ROS Melodic:

 *Note that each platform might have a slightly different installation method.
 See http://wiki.ros.org/melodic/Installation for supported platforms and their installation method.

*The following method is specific for Ubuntu18.04(Bionic) platform.

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

## To duplicate this repository to your PC
First, install git
```
sudo apt install git
```
To check that git was installed and its version
```
git --version
```
__This repository is dependent on submodules so when cloning use:__
```
git clone https://github.com/a2198699s/AI-Line-Follower --recurse-submodules
```
You will now get this repository in your ~/ or Home directory.
To check that it is there:
```
cd ~
ls
```

## How to start

Instructions for using each section of the framework can be found in the respective directory.

To start running simulation, see README in __simulators__ directory.


## Contents

* __rqt_plugins:__ this directory contains the custom rqt GUI plugins used for the project

* __ros_packages:__ this directory contains some misc. ROS packages useful for the project

* __physical_robot:__ this directory contains work related with the development of the physical robot for the project

* __simulators:__ this directory contains simulator packages used in the framework

* __neural_networks:__ this directory contains the neural network implementations used in the project

* __interface_specifications:__ this directory contains specifications for module interfaces for the framework

