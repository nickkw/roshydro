*********************
BUG PATHING ALGORITHM
*********************

0. Quickstart
----------------------
This assumes that you have successfully performed an ssh into the wandboard.

On the wandboard:
-Download the range_broadcaster and bug_pathing packages into your workspace, catkin_make.
-source devel/setup.bash
-Run "sudo chmod a+rw /dev/i2c-*" to give access permissions to the i2c bus.
-Run "sudo ifconfig eth0 192.168.1.10 netmask 255.255.255.0 up" to change the IP to allow the low level board to get commands from the wandboard.
-Run "ssh root@192.168.1.105" to connect to the low level board. (Password is ftbl07)

On the low level board:
-Set the date using "date -s "<date>""
-Run ./player to start running the low level software which will control the motors.

On the wandboard:
-Run roslaunch bug_pathing wandboard_bug_pathing_client.launch on the wandboard.

On the master computer (desktop):
-Launch roslaunch wandboard_bug_pathing_server.launch (you may have to change the path to the map file labmap_feb2_15h51.yaml)
-Set the initial location of the robot on the map using the "Initial Pose" button in rviz.
	-The 4 pieces of electrical tape on the floor to the right of the workstation represent the initial pose of the robot in rviz, rotated clockwise by 90 degrees. (see the initialpose.pdf file)
-Send a waypoint on the map in rviz using "Publish Point" and the robot should begin to move towards it, avoiding any obstacles in its way. It may get stuck and be unable to enter narrower obstacles, which is a software bug.

To stop the motor process on the low level board, run on the WANDBOARD:
-rostopic pub /command std_msgs/String "101.0 0.0"
	-This will stop the ./player process on the low level board. You will have to run ./player on the low level again to rerun the process.


1. Installation, launch files, and configuration
----------------------

1.1 Installation

	Copy the bug_pathing package directory to the /src folder of your catkin workspace and run catkin_make. Ensure that the range_broadcaster package is also installed,
	as it contains the drivers for the ultrasound sensors.

1.2 Launch files

	Launch files are included in the /launch directory. The client launch file should be run on the wandboard. An example of a server launch file is also provided.

	The client launch file will launch the bug_pathing control node, all the ultrasound sensor nodes, the respective static tf broadcasters, as well as the command and odometry nodes required
	by the robot.

1.3 Configuring ultrasound sensors
	
	The parameters are well documented on the provided client launch file. See the README for the range_broadcaster package for more details on how the ultrasound
	sensors are implemented. The main thing to look out for is to make sure the sensor_frame parameter matches the respective sensor_frame parameters of the bug_pathing node.

	The main parameters that will needed to be set are the sensor_frame, i2c_bus, and i2c_address parameters. The sensor frame sets the name of the sensor to be used by
	the bug_pathing node. i2c_bus defines which hardware i2c bus the sensor is attached to. The i2c_address parameter defines the i2c address of the sensor. 

	To determine the i2c address of a sensor, attach only one sensor to the wandboard and run i2cdetect -y [bus] where bus is the hardware bus which the sensor is attached to.
	The address of the sensor will be returned.

1.3 Configuring sensor tfs

	Change the arguments of the static tf broadcasters to reflect the physical location of the sensors. The argument format is [x y z yaw roll pitch sensor_frame rate_in_hz].

1.4 Configuring bug_pathing node

	The remap tag is required since the command node of the robot will listen to only the /command topic.
	Change the *_sensor_frame parameters to reflect the given sensor_frame names from the range_broadcaster nodes.
	The lin_vel_max and ang_vel_max parameters control the maximum speed of the robot. 0.5 and 2.0, respectively, work reasonably well.
	obstacle_range_threshold defines the minimum distance for which an object will be considered an obstacle. It is recommended to leave this at 0.55 for now as it is tested to work reasonably well.


2. About
----------------------
This package is a path planning algorithm based off of the 'bug2' algorithm as described by the book Probabilistic Robotics by Sebastian Thrun.
Using ultrasound sensors, the robot should avoid obsacles while navigating toward a waypoint provided by rviz.

When the robot approaches an obstacle, it will decide which direction to turn based on its angle of approach to the obstacle. For instance, if the robot approaches a flat wall angled to the left,
it will turn to the left. Once obstacles are no longer detected on any of its sensors, the robot will attempt to head straight towards the waypoint. It will continue to avoid obstacles every time
a new obstacle is encountered. Because of the simplicity of this algorithm, the robot may circle around several times before successfully avoiding an obstacle. It may also be unable to navigate through
narrow obstacles such as a door.


3. Troubleshooting
----------------------

-Launching the client launch file yields "ERROR: Did not open I2C bus. Bus failed."
	- Change the i2c bus permissions on the wandboard using "sudo chmod a+rw /dev/i2c-*" and relaunch.

-The robot does not move
	-Ensure ./player is running on the low level board and that the eth0 ip address of the wandboard is set to 192.168.1.10

Please contact kwanwong[at]ualberta.ca if you have any questions.
