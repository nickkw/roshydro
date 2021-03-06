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

config_description = {'upper': 'DEFAULT', 'lower': 'groups', 'srcline': 233, 'name': 'Default', 'parent': 0, 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'cstate': 'true', 'parentname': 'Default', 'class': 'DEFAULT', 'field': 'default', 'state': True, 'parentclass': '', 'groups': [], 'parameters': [{'srcline': 262, 'description': 'Set the spatial locator', 'max': 2, 'cconsttype': 'const int', 'ctype': 'int', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'spatial_locator', 'edit_method': "{'enum_description': 'Set the spatial locator', 'enum': [{'srcline': 9, 'description': 'ANN', 'srcfile': '/home/nicholaskwan-wong/roshydro/src/perception_pcl/pcl_ros/cfg/MLS.cfg', 'cconsttype': 'const int', 'value': 0, 'ctype': 'int', 'type': 'int', 'name': 'ANN'}, {'srcline': 9, 'description': 'FLANN', 'srcfile': '/home/nicholaskwan-wong/roshydro/src/perception_pcl/pcl_ros/cfg/MLS.cfg', 'cconsttype': 'const int', 'value': 1, 'ctype': 'int', 'type': 'int', 'name': 'FLANN'}, {'srcline': 9, 'description': 'Dense/organized data locator', 'srcfile': '/home/nicholaskwan-wong/roshydro/src/perception_pcl/pcl_ros/cfg/MLS.cfg', 'cconsttype': 'const int', 'value': 2, 'ctype': 'int', 'type': 'int', 'name': 'organized'}]}", 'default': 0, 'level': 0, 'min': 0, 'type': 'int'}, {'srcline': 262, 'description': 'Sphere radius for nearest neighbor search', 'max': 0.5, 'cconsttype': 'const double', 'ctype': 'double', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'search_radius', 'edit_method': '', 'default': 0.02, 'level': 0, 'min': 0.0, 'type': 'double'}, {'srcline': 262, 'description': 'Whether to use polynomial fit', 'max': True, 'cconsttype': 'const bool', 'ctype': 'bool', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'use_polynomial_fit', 'edit_method': '', 'default': True, 'level': 0, 'min': False, 'type': 'bool'}, {'srcline': 262, 'description': 'Set the spatial locator', 'max': 5, 'cconsttype': 'const int', 'ctype': 'int', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'polynomial_order', 'edit_method': '', 'default': 2, 'level': 0, 'min': 0, 'type': 'int'}, {'srcline': 262, 'description': "How 'flat' should the neighbor weighting gaussian be (the smaller, the more local the fit)", 'max': 0.5, 'cconsttype': 'const double', 'ctype': 'double', 'srcfile': '/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py', 'name': 'gaussian_parameter', 'edit_method': '', 'default': 0.02, 'level': 0, 'min': 0.0, 'type': 'double'}], 'type': '', 'id': 0}

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

MLS_ANN = 0
MLS_FLANN = 1
MLS_organized = 2
