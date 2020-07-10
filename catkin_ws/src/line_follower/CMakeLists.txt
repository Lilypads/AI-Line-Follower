cmake_minimum_required(VERSION 3.0)

project(enki_ros_pck)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package(
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

message(${CMAKE_CURRENT_SOURCE_DIR})

#add_library(enki_lib)
#target_link_libraries(enki_lib ${CMAKE_CURRENT_SOURCE_DIR}/../../../enki/libenki.a)

set(enki_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/../enki/libenki.a)

#find_package(enki REQUIRED)
#include_directories(${enki_INCLUDE_DIR})
#include_directories(${enki_INCLUDE_DIR}/enki)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)

set(SOURCES run.cpp Racer.cpp bandpass.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../enki)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(robot ${SOURCES})
target_link_libraries(robot ${catkin_LIBRARIES} ${enki_LIBRARY} Qt5::OpenGL Qt5::Widgets GL)
