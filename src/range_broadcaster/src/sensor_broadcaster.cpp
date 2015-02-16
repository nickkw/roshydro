/* This node will control one SRF08 sensor and publish sensor_msgs/Range.msg messages. To use a system with multiple sensors, use a launch configuration */

#include "ros/ros.h"
#include "sensor_msgs/Range.h"
#include "srf08.h"
#include <string>
#include <iomanip>

int i2c_bus_, i2c_address_, max_range_, max_gain_, ping_rate_;
std::string sensor_frame_, i2c_address_str_, max_range_str_, max_gain_str_;

int error_count = 0; // Used to count timeout

ros::Publisher range_pub_;
sensor_msgs::Range msg;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "range_node");
  ros::NodeHandle nh_;
  ros::NodeHandle pnh_("~");
  ros::Time current_time;

  // Parameters

  pnh_.param<std::string>("sensor_frame", sensor_frame_, "srf08"); // Names the frame of the sensor.
  pnh_.param<int>("i2c_bus", i2c_bus_, 1); // Defines which i2c bus on the wandboard to use.
  pnh_.param<std::string>("i2c_address", i2c_address_str_, "0x70"); // Defines the address of the SRF08 sensor to be used.
  pnh_.param<std::string>("max_range", max_range_str_, "0x18"); // Defines max range of the sensor to be used - default 1m
  pnh_.param<std::string>("max_gain", max_gain_str_, "0x1F"); // Defines max gain of sensor. See SRF08 datasheet for exact details. Default is maximum value at 0x1F. Scaling is non-linear.

  pnh_.param<int>("ping_rate", ping_rate_, 10); // Defines the rate of pings of the sensor in Hz. The range of this value should not exceed 10.

  // Perform parameter string to integer conversion
  i2c_address_ = std::strtoul(i2c_address_str_.c_str(), NULL, 0);
  max_range_ = std::strtoul(max_range_str_.c_str(), NULL, 0);
  max_gain_ = std::strtoul(max_gain_str_.c_str(), NULL, 0);

  // Initialization

  SRF08 sensor = SRF08(i2c_bus_, i2c_address_); // Create the SRF08 object.

  sensor.setRangeRegister(max_range_); // Change max range.
  sensor.setMaxGainRegister(max_gain_); // Change gain

  range_pub_ = nh_.advertise<sensor_msgs::Range>("range", 10);

  ROS_INFO("SRF08 sensor created on Bus: %d   Address: 0x%02x    with Max range: %d cm    Gain register: 0x%02x", i2c_bus_, i2c_address_, (max_range_*43+43)/10, max_gain_);

  // These range message values will remain constant and are set at initialization.
  msg.header.frame_id = sensor_frame_;
  msg.radiation_type = 0;
  msg.field_of_view = 0.959931089; // 55 degrees = 0.959931089 radians
  msg.min_range = 0.03; // Minimum range in metres
  msg.max_range = (max_range_*43+43)/1000.0; // Maximum range in metres since max_range is given in mm

  // Loop
  ros::Rate loop_rate(ping_rate_); // Loop at ping_rate_ parameter Hz - A value of 10 gives about 100ms per sensor ping which is adequate given the sensor needs 65ms for a ping (built into the function)

  while(ros::ok()){
    // Populate the message. getRange is defined in srf08.cpp and does most of the work.
    msg.range = (float)sensor.getRange()/100.0; // Since sensor outputs ranges in cm, we divide by 100 to get range data in m as per the sensor_msgs/Range.msg standard.
    msg.header.stamp = ros::Time::now(); // Add a timestamp to a message
    // tf implementation can be added here in the future to add even more position data

    // Check in case of sensor disconnect or error. Will print after ~3 seconds of no data
    if(msg.range == 0) {
      error_count++;
      if(error_count == 30) {
        ROS_WARN("Connection with SRF08 sensor on Bus %d with Address 0x%02x was lost or the sensor at the address was not found! Check the connection and the I2C address.", i2c_bus_, i2c_address_);
      }
    }

    // Upon reconnection, print a message
    if(error_count > 30 && msg.range != 0){
      error_count = 0;
      ROS_INFO("Connection re-established with SRF08 sensor on Bus %d with Address 0x%02x.", i2c_bus_, i2c_address_);
    }

    // ROS_INFO("Range: %2.2f m\n", msg.range);

    // Publish the message
    range_pub_.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
  }
}