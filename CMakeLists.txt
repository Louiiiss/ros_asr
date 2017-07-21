cmake_minimum_required(VERSION 2.8.3)
project(ros_asr)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  tf
  std_msgs
  geometry_msgs
  visualization_msgs
  interactive_markers
  sensor_msgs
  baxter_core_msgs
  visualization_msgs
  trac_ik_lib
  eigen_conversions
  kdl_conversions
)

catkin_package(
#  INCLUDE_DIRS include
  LIBRARIES
  CATKIN_DEPENDS
    trac_ik_lib
    kdl_conversions
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${eigen_INCLUDE_DIRS}
)

add_executable(kaldi_relay src/kaldi_relay.cpp)
target_link_libraries(kaldi_relay ${catkin_LIBRARIES})



