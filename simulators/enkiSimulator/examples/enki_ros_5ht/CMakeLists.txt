cmake_minimum_required(VERSION 3.0)

project(enki_ros_5ht)

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

set(CMAKE_CXX_STANDARD 11) # This is equal to QMAKE_CXX_FLAGS += -std=c++0x

# Find includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)

#set(CMAKE_PREFIX_PATH /usr/include/lqt)

#message(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(quicktime)

set(SOURCES merge2quicktime.cpp patience-5ht.cpp robot.cpp world.cpp worldpoint.cpp direction.cpp bandp.cpp limbic-system-model/filter.cpp limbic-system-model/limbic-system-model.cpp)

set(HEADERS bandp.h defs.h direction.h merge2quicktime.h patience-5ht.h robot.h world.h worldpoint.h limbic-system-model/filter.h limbic-system-model/limbic-system-model.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/limbic-system-model)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")
include_directories(-isystem /usr/include/lqt)


add_executable(robot ${HEADERS} ${SOURCES})
target_link_libraries(robot ${catkin_LIBRARIES} Qt5::Widgets quicktime)

