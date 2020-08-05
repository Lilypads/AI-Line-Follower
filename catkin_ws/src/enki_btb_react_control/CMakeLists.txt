cmake_minimum_required(VERSION 3.0)

project(enki_btb_react_control)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  sensor_msgs
  std_msgs
)

catkin_package(
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

#set(enki_LIBRARY /usr/local/lib/libenki.a)

#find_package(Qt5 COMPONENTS Widgets REQUIRED)
#find_package(Qt5 COMPONENTS OpenGL REQUIRED)

set(SOURCES line_react_control.cpp)

#include_directories(/usr/local/include/enki)

add_executable(enki_btb_react_control_node ${SOURCES})
target_link_libraries(enki_btb_react_control_node ${catkin_LIBRARIES})
