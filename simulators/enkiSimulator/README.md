# Enki (QT5)

a fast 2D robot simulator using the QT5 build system.

© 1999-2017 Stéphane Magnenat and others ([full list](AUTHORS))

© 2017 Bernd Porr <mail@berndporr.me.uk>


## License

[GPL 2.0](LICENSE).

## Description
This project provides an additional ROS based package in the examples directory. This gives an example for using the enki simulator with the ROS framework used in [AI-Line-Follower](https://github.com/a2198699s/AI-Line-Follower).

## How to run simulation

__First the enki library needs to be built__

1. Change directory to the library directory (assuming you clone this repository to ~/ or Home directory as instructed on first README, else just cd to where the directory is):
```
cd ~/AI-Line-Follower/simulators/enkiSimulator/enki
```
2. Build the library:
```
qmake
make
```

*Note that if you cannot run qmake and make, you probably need to install qt5 first:
```
sudo apt-get install qt5-default
```

__Next try to build an example "ros_example"__

3. Empty the catkin_ws/devel and catkin_ws/build folders due to saved user specific cache stuff which will cause error when building ROS package on a different PC. You can do that by system files program or doing the following command:
```
cd ~/AI-Line-Follower/simulators/enkiSimulator/examples/ros_example/catkin_ws
rm -r devel
rm -r build
mkdir devel
mkdir build
```

4. Build the ROS package:
```
catkin_make
```

__Now Run the simulation__

5. Run roscore on one terminal (if haven't already):
```
roscore
```
*Note that you need to keep this running all the time on one terminal when running the simulation. You can exit this later by pressing ctrl+c.

6. Make sure you are in the enki example workspace on your new terminal or do the following command:
```
cd ~/AI-Line-Follower/simulators/enkiSimulator/examples/ros_example/catkin_ws
```
7. Source the workspace:
```
source devel/setup.bash
```
*Note that you should always source a workspace everytime before running it to avoid ROS being unable to find the package.

8. Run it:
```
rosrun enki_ros_pck robot
```

__The robot window should show up, but not yet moving.__

* To make the robot move following the line, go to __ros_packages__ directory and follow the README instructions there.

* To open the custom rqt plugins in rqt gui windows, go to __rqt_plugins__ directory and follow the README instructions there.

### How to check which node is currently running

On another terminal:
```
rosnode list
```
### How to view communication between nodes using rqt_graph

On another terminal:
```
rosrun rqt_graph rqt_graph
```
