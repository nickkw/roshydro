#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#define PI 3.1415926536

std::string base_frame_, left_sensor_frame_, right_sensor_frame_, front_sensor_frame_;

int main(int argc, char** argv){
  // Initialization
  ros::init(argc, argv, "tf_broadcaster");
  ros::NodeHandle node;
  ros::NodeHandle pnh_("~");

  tf::TransformBroadcaster br;
  tf::Transform transform;

  // Parameters
  pnh_.param<std::string>("base_frame", base_frame_, "base_frame"); // Names the base tf frame.
  pnh_.param<std::string>("left_sensor_frame", left_sensor_frame_, "srf08_left"); // Names the frame of the front left sensor.
  pnh_.param<std::string>("right_sensor_frame", right_sensor_frame_, "srf08_right"); // Names the frame of the right sensor.
  pnh_.param<std::string>("front_sensor_frame", front_sensor_frame_, "srf08_front"); // Names the frame of the front sensor.

  // Loop
  ros::Rate rate(10.0);
  while (node.ok()){
    tf::Quaternion rotation;

    // Front sensor transform
    transform.setOrigin( tf::Vector3(0.135, 0.0, 0.0) ); // X - Y - Z (in m)
    rotation.setRPY(0, 0, 0); // Third argument is theta (yaw) angle
    transform.setRotation( rotation );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), base_frame_, front_sensor_frame_));

    // Left sensor transform
    transform.setOrigin( tf::Vector3(0.075, 0.18, 0.0) );  // X - Y - Z (in m)
    rotation.setRPY(0, 0, 70*PI/180); // Third argument is theta (yaw) angle
    transform.setRotation( rotation );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), base_frame_, left_sensor_frame_));

    // Right sensor transform
    transform.setOrigin( tf::Vector3(0.075, -0.18, 0.0) ); // X - Y - Z (in m)
    rotation.setRPY(0, 0, -70*PI/180); // Third argument is theta (yaw) angle
    transform.setRotation( rotation );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), base_frame_, right_sensor_frame_));


    rate.sleep();
  }
  return 0;
};