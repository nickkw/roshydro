# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "hector_nav_msgs: 0 messages, 5 services")

set(MSG_I_FLAGS "-Inav_msgs:/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/nav_msgs/cmake/../msg;-Istd_msgs:/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg;-Igeometry_msgs:/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg;-Iactionlib_msgs:/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/actionlib_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(hector_nav_msgs_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages

### Generating Services
_generate_srv_cpp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetRecoveryInfo.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Pose.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/nav_msgs/cmake/../msg/Path.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_cpp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetRobotTrajectory.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Pose.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/nav_msgs/cmake/../msg/Path.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_cpp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetSearchPosition.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Pose.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PoseStamped.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_cpp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetNormal.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PointStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Vector3.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_cpp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetDistanceToObstacle.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PointStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_nav_msgs
)

### Generating Module File
_generate_module_cpp(hector_nav_msgs
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_nav_msgs
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(hector_nav_msgs_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(hector_nav_msgs_generate_messages hector_nav_msgs_generate_messages_cpp)

# target for backward compatibility
add_custom_target(hector_nav_msgs_gencpp)
add_dependencies(hector_nav_msgs_gencpp hector_nav_msgs_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hector_nav_msgs_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages

### Generating Services
_generate_srv_lisp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetRecoveryInfo.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Pose.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/nav_msgs/cmake/../msg/Path.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_lisp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetRobotTrajectory.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Pose.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/nav_msgs/cmake/../msg/Path.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_lisp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetSearchPosition.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Pose.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PoseStamped.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_lisp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetNormal.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PointStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Vector3.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_lisp(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetDistanceToObstacle.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PointStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_nav_msgs
)

### Generating Module File
_generate_module_lisp(hector_nav_msgs
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_nav_msgs
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(hector_nav_msgs_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(hector_nav_msgs_generate_messages hector_nav_msgs_generate_messages_lisp)

# target for backward compatibility
add_custom_target(hector_nav_msgs_genlisp)
add_dependencies(hector_nav_msgs_genlisp hector_nav_msgs_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hector_nav_msgs_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages

### Generating Services
_generate_srv_py(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetRecoveryInfo.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Pose.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/nav_msgs/cmake/../msg/Path.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_py(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetRobotTrajectory.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Pose.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/nav_msgs/cmake/../msg/Path.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_py(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetSearchPosition.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Pose.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PoseStamped.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_py(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetNormal.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PointStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Vector3.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_nav_msgs
)
_generate_srv_py(hector_nav_msgs
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_nav_msgs/srv/GetDistanceToObstacle.srv"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/PointStamped.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/geometry_msgs/cmake/../msg/Point.msg;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_nav_msgs
)

### Generating Module File
_generate_module_py(hector_nav_msgs
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_nav_msgs
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(hector_nav_msgs_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(hector_nav_msgs_generate_messages hector_nav_msgs_generate_messages_py)

# target for backward compatibility
add_custom_target(hector_nav_msgs_genpy)
add_dependencies(hector_nav_msgs_genpy hector_nav_msgs_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hector_nav_msgs_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_nav_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_nav_msgs
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(hector_nav_msgs_generate_messages_cpp nav_msgs_generate_messages_cpp)
add_dependencies(hector_nav_msgs_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_nav_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_nav_msgs
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(hector_nav_msgs_generate_messages_lisp nav_msgs_generate_messages_lisp)
add_dependencies(hector_nav_msgs_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_nav_msgs)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_nav_msgs\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_nav_msgs
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(hector_nav_msgs_generate_messages_py nav_msgs_generate_messages_py)
add_dependencies(hector_nav_msgs_generate_messages_py std_msgs_generate_messages_py)
