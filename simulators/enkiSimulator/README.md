# Enki (QT5)

a fast 2D robot simulator using the QT5 build system.

© 1999-2017 Stéphane Magnenat and others ([full list](AUTHORS))

© 2017 Bernd Porr <mail@berndporr.me.uk>


## License

[GPL 2.0](LICENSE).

# Description
This project provides an additional ROS based package in the examples directory. This gives an example for using the enki simulator with the ROS framework used in [AI-Line-Follower](https://github.com/a2198699s/AI-Line-Follower).

# Use with ROS

First the enki library needs to be built:
```
cd enkiSimulator
cd enki
qmake
make
```

*Note that if you cannot run make, you probably need to install qt5 first:
```
sudo apt-get install qt5-default
```

Next try to build and run an example __ros_example__:

Empty the catkin_ws/devel and catkin_ws/build folders due to saved user specific cache stuff which will cause error when building ROS package on a different PC
```
cd ..
cd examples/ros_example/catkin_ws
cd devel
rm *
cd ..
cd build
rm *
```

Build the ROS package:
```
cd ..
catkin_make
```

Run roscore on one terminal (if haven't already):
```
roscore
```
*Note that you need to keep this running all the time on one terminal when running the simulation. You can exit this later by pressing ctrl+x.

To run the simulation:

Make sure you are in the enki example workspace (.../enkiSimulator/examples/ros_example/catkin_ws)

source the workspace
```
source devel/setup.bash
```
and run it on another terminal
```
rosrun enki_ros_pck robot
```
The robot window should show up, but not yet moving.

To make the robot move following the line, go to __ros_packages__ directory and follow the README instructions there.

To open the custom rqt plugins in rqt gui windows, go to __rqt_plugins__ directory and follow the README instructions there.
