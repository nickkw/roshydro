# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "viso2_ros: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iviso2_ros:/home/nicholaskwan-wong/roshydro/src/viso2/viso2_ros/msg;-Istd_msgs:/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(viso2_ros_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(viso2_ros
  "/home/nicholaskwan-wong/roshydro/src/viso2/viso2_ros/msg/VisoInfo.msg"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/viso2_ros
)

### Generating Services

### Generating Module File
_generate_module_cpp(viso2_ros
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/viso2_ros
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(viso2_ros_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(viso2_ros_generate_messages viso2_ros_generate_messages_cpp)

# target for backward compatibility
add_custom_target(viso2_ros_gencpp)
add_dependencies(viso2_ros_gencpp viso2_ros_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS viso2_ros_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(viso2_ros
  "/home/nicholaskwan-wong/roshydro/src/viso2/viso2_ros/msg/VisoInfo.msg"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/viso2_ros
)

### Generating Services

### Generating Module File
_generate_module_lisp(viso2_ros
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/viso2_ros
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(viso2_ros_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(viso2_ros_generate_messages viso2_ros_generate_messages_lisp)

# target for backward compatibility
add_custom_target(viso2_ros_genlisp)
add_dependencies(viso2_ros_genlisp viso2_ros_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS viso2_ros_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(viso2_ros
  "/home/nicholaskwan-wong/roshydro/src/viso2/viso2_ros/msg/VisoInfo.msg"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/viso2_ros
)

### Generating Services

### Generating Module File
_generate_module_py(viso2_ros
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/viso2_ros
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(viso2_ros_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(viso2_ros_generate_messages viso2_ros_generate_messages_py)

# target for backward compatibility
add_custom_target(viso2_ros_genpy)
add_dependencies(viso2_ros_genpy viso2_ros_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS viso2_ros_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/viso2_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/viso2_ros
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(viso2_ros_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/viso2_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/viso2_ros
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(viso2_ros_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/viso2_ros)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/viso2_ros\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/viso2_ros
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(viso2_ros_generate_messages_py std_msgs_generate_messages_py)
