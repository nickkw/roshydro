## *********************************************************
## 
## File autogenerated for the pcl_ros package 
## by the dynamic_reconfigure package.
## Please do not edit.
## 
## ********************************************************/

##**********************************************************
## Software License Agreement (BSD License)
##
##  Copyright (c) 2008, Willow Garage, Inc.
##  All rights reserved.
##
##  Redistribution and use in source and binary forms, with or without
##  modification, are permitted provided that the following conditions
##  are met:
##
##   * Redistributions of source code must retain the above copyright
##     notice, this list of conditions and the following disclaimer.
##   * Redistributions in binary form must reproduce the above
##     copyright notice, this list of conditions and the following
##     disclaimer in the documentation and/or other materials provided
##     with the distribution.
##   * Neither the name of the Willow Garage nor the names of its
##     contributors may be used to endorse or promote products derived
##     from this software without specific prior written permission.
##
##  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
##  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
##  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
##  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
##  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
##  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
##  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
##  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
##  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
##  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
##  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
##  POSSIBILITY OF SUCH DAMAGE.
##**********************************************************/

from dynamic_reconfigure.encoding import extract_params

inf = float('inf')

config_description = {'upper': 'DEFAULT', 'lower': 'groups', 'srcline': 233, 'name': 'Default', 'parent': 0, 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'cstate': 'true', 'parentname': 'Default', 'class': 'DEFAULT', 'field': 'default', 'state': True, 'parentclass': '', 'groups': [], 'parameters': [{'srcline': 262, 'description': 'X coordinate of the minimum point of the box.', 'max': 5.0, 'cconsttype': 'const double', 'ctype': 'double', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'min_x', 'edit_method': '', 'default': -1.0, 'level': 0, 'min': -5.0, 'type': 'double'}, {'srcline': 262, 'description': 'X coordinate of the maximum point of the box.', 'max': 5.0, 'cconsttype': 'const double', 'ctype': 'double', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'max_x', 'edit_method': '', 'default': 1.0, 'level': 0, 'min': -5.0, 'type': 'double'}, {'srcline': 262, 'description': 'Y coordinate of the minimum point of the box.', 'max': 5.0, 'cconsttype': 'const double', 'ctype': 'double', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'min_y', 'edit_method': '', 'default': -1.0, 'level': 0, 'min': -5.0, 'type': 'double'}, {'srcline': 262, 'description': 'Y coordinate of the maximum point of the box.', 'max': 5.0, 'cconsttype': 'const double', 'ctype': 'double', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'max_y', 'edit_method': '', 'default': 1.0, 'level': 0, 'min': -5.0, 'type': 'double'}, {'srcline': 262, 'description': 'Z coordinate of the minimum point of the box.', 'max': 5.0, 'cconsttype': 'const double', 'ctype': 'double', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'min_z', 'edit_method': '', 'default': -1.0, 'level': 0, 'min': -5.0, 'type': 'double'}, {'srcline': 262, 'description': 'Z coordinate of the maximum point of the box.', 'max': 5.0, 'cconsttype': 'const double', 'ctype': 'double', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'max_z', 'edit_method': '', 'default': 1.0, 'level': 0, 'min': -5.0, 'type': 'double'}, {'srcline': 262, 'description': 'Set whether the filtered points should be kept and set to NaN, or removed from the PointCloud, thus potentially breaking its organized structure.', 'max': True, 'cconsttype': 'const bool', 'ctype': 'bool', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'keep_organized', 'edit_method': '', 'default': False, 'level': 0, 'min': False, 'type': 'bool'}, {'srcline': 262, 'description': 'If True the box will be empty Else the remaining points will be the ones in the box', 'max': True, 'cconsttype': 'const bool', 'ctype': 'bool', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'negative', 'edit_method': '', 'default': False, 'level': 0, 'min': False, 'type': 'bool'}, {'srcline': 262, 'description': 'The input TF frame the data should be transformed into before processing, if input.header.frame_id is different.', 'max': '', 'cconsttype': 'const char * const', 'ctype': 'std::string', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'input_frame', 'edit_method': '', 'default': '', 'level': 0, 'min': '', 'type': 'str'}, {'srcline': 262, 'description': 'The output TF frame the data should be transformed into after processing, if input.header.frame_id is different.', 'max': '', 'cconsttype': 'const char * const', 'ctype': 'std::string', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'output_frame', 'edit_method': '', 'default': '', 'level': 0, 'min': '', 'type': 'str'}], 'type': '', 'id': 0}

min = {}
max = {}
defaults = {}
level = {}
type = {}
all_level = 0

#def extract_params(config):
#    params = []
#    params.extend(config['parameters'])    
#    for group in config['groups']:
#        params.extend(extract_params(group))
#    return params

for param in extract_params(config_description):
    min[param['name']] = param['min']
    max[param['name']] = param['max']
    defaults[param['name']] = param['default']
    level[param['name']] = param['level']
    type[param['name']] = param['type']
    all_level = all_level | param['level']

