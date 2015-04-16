from __future__ import print_function
import argparse
import os
import stat
import sys

# find the import for catkin's python package - either from source space or from an installed underlay
if os.path.exists(os.path.join('/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/catkin/cmake', 'catkinConfig.cmake.in')):
    sys.path.insert(0, os.path.join('/home/nicholaskwan-wong/ros_catkin_ws/install_isolated/share/catkin/cmake', '..', 'python'))
try:
    from catkin.environment_cache import generate_environment_script
except ImportError:
    # search for catkin package in all workspaces and prepend to path
    for workspace in "/home/nicholaskwan-wong/pcl_workspace/devel_isolated/registration_viewer;/home/nicholaskwan-wong/pcl_workspace/devel_isolated/kinfu;/home/nicholaskwan-wong/pcl_workspace/devel_isolated/kinect2_calibration;/home/nicholaskwan-wong/pcl_workspace/devel_isolated/kinect2_bridge;/home/nicholaskwan-wong/pcl_workspace/devel_isolated/iai_kinect2;/home/nicholaskwan-wong/pcl_workspace/devel_isolated/freenect_stack;/home/nicholaskwan-wong/pcl_workspace/devel_isolated/freenect_launch;/home/nicholaskwan-wong/pcl_workspace/devel_isolated/freenect_camera;/home/nicholaskwan-wong/pcl_workspace/devel_isolated/depth_registration;/home/nicholaskwan-wong/roshydro/devel;/home/nicholaskwan-wong/ros_catkin_ws/install_isolated;/opt/ros/indigo".split(';'):
        python_path = os.path.join(workspace, 'lib/python2.7/dist-packages')
        if os.path.isdir(os.path.join(python_path, 'catkin')):
            sys.path.insert(0, python_path)
            break
    from catkin.environment_cache import generate_environment_script

code = generate_environment_script('/home/nicholaskwan-wong/roshydro/devel/env.sh')

output_filename = '/home/nicholaskwan-wong/roshydro/build/catkin_generated/setup_cached.sh'
with open(output_filename, 'w') as f:
    #print('Generate script for cached setup "%s"' % output_filename)
    f.write('\n'.join(code))

mode = os.stat(output_filename).st_mode
os.chmod(output_filename, mode | stat.S_IXUSR)
