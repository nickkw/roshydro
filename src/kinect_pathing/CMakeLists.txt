cmake_minimum_required(VERSION 2.8.3)
project(kinect_pathing)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
  PCL
  tf
  pcl_ros
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES kinect_pathing
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
	include
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
)

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

# Declare a cpp executable
add_executable(kinect_clouds_pub src/extract_path.cpp src/kinect_clouds_pub.cpp)
# Specify libraries to link a library or executable target against
target_link_libraries(kinect_clouds_pub
  ${catkin_LIBRARIES}
)

# Declare a cpp executable
add_executable(kinect_waypoint_pub src/kinect_waypoint_pub.cpp)
# Specify libraries to link a library or executable target against
target_link_libraries(kinect_waypoint_pub
  ${catkin_LIBRARIES}
)

# Declare a cpp executable
add_executable(tf_broadcaster src/tf_broadcaster.cpp)
# Specify libraries to link a library or executable target against
target_link_libraries(tf_broadcaster
  ${catkin_LIBRARIES}
)

# Declare a cpp executable
add_executable(move_to_waypoint src/move_to_waypoint.cpp)
# Specify libraries to link a library or executable target against
target_link_libraries(move_to_waypoint
  ${catkin_LIBRARIES}
)