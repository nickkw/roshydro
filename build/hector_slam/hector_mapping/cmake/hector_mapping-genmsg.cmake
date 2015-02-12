# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "hector_mapping: 2 messages, 0 services")

set(MSG_I_FLAGS "-Ihector_mapping:/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(hector_mapping_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(hector_mapping
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg/HectorDebugInfo.msg"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg/HectorIterData.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_mapping
)
_generate_msg_cpp(hector_mapping
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg/HectorIterData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_mapping
)

### Generating Services

### Generating Module File
_generate_module_cpp(hector_mapping
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_mapping
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(hector_mapping_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(hector_mapping_generate_messages hector_mapping_generate_messages_cpp)

# target for backward compatibility
add_custom_target(hector_mapping_gencpp)
add_dependencies(hector_mapping_gencpp hector_mapping_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hector_mapping_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(hector_mapping
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg/HectorDebugInfo.msg"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg/HectorIterData.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_mapping
)
_generate_msg_lisp(hector_mapping
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg/HectorIterData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_mapping
)

### Generating Services

### Generating Module File
_generate_module_lisp(hector_mapping
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_mapping
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(hector_mapping_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(hector_mapping_generate_messages hector_mapping_generate_messages_lisp)

# target for backward compatibility
add_custom_target(hector_mapping_genlisp)
add_dependencies(hector_mapping_genlisp hector_mapping_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hector_mapping_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(hector_mapping
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg/HectorDebugInfo.msg"
  "${MSG_I_FLAGS}"
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg/HectorIterData.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_mapping
)
_generate_msg_py(hector_mapping
  "/home/nicholaskwan-wong/roshydro/src/hector_slam/hector_mapping/msg/HectorIterData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_mapping
)

### Generating Services

### Generating Module File
_generate_module_py(hector_mapping
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_mapping
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(hector_mapping_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(hector_mapping_generate_messages hector_mapping_generate_messages_py)

# target for backward compatibility
add_custom_target(hector_mapping_genpy)
add_dependencies(hector_mapping_genpy hector_mapping_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hector_mapping_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_mapping)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hector_mapping
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_mapping)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hector_mapping
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_mapping)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_mapping\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hector_mapping
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
