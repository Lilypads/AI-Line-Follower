cmake_minimum_required(VERSION 3.0)

project(enki_ros_btb)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  roslib
)

catkin_package(
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)


set(enki_LIBRARY /usr/local/lib/libenki.a)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)

set(SOURCES run.cpp Racer.cpp bandp.cpp)

include_directories(/usr/local/include/enki)

add_executable(enki_ros_btb_node ${SOURCES})
target_link_libraries(enki_ros_btb_node ${catkin_LIBRARIES} ${enki_LIBRARY} Qt5::OpenGL Qt5::Widgets GL)
