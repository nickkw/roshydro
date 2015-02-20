#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf/transform_broadcaster.h>
#define PI 3.1415926536

std::string base_frame_, odom_frame_, map_frame_, left_sensor_frame_, right_sensor_frame_, front_sensor_frame_;

// Implementation variables
std::string velstr, xmsg, ymsg, thmsg;
size_t strpos1, strpos2; // Used to get substrings from velmsg string

double x_odo = 0.0;
double y_odo = 0.0;
double th_odo = 0.0;

double x_pose = 0.0;
double y_pose = 0.0;
double th_pose = 0.0;

// velCallback function takes incoming odometry message from the low level board
void velCallback(const std_msgs::String& str) {
  
  velstr = str.data;

  // // Get current time
  // current_time = ros::Time::now();

  // Retrieve message substrings
  strpos1 = velstr.find(" "); // Find the position of the space which delineates the x_odo and y_odo fields. Strpos returns the first match.
  strpos2 = velstr.find(" ", strpos1+1);  // Find the position of the second space which delineates the y_odo and th_odo fields. Note strpos argument starts the search on the character directly after the first space.

  xmsg = velstr.substr(0, strpos1-1); // Take the substring of velstr from the beginning to the last character before the space as the xmsg
  ymsg = velstr.substr(strpos1+1, strpos2-1);
  thmsg = velstr.substr(strpos2);

  // Convert message substrings to doubles and store values as current
  x_odo = atof(xmsg.c_str());
  y_odo = atof(ymsg.c_str());
  th_odo = atof(thmsg.c_str());
}

// poseCallback function takes 2D pose from rviz topic 'initialpose' and passes it on to the base_frame tf object
void poseCallback(const geometry_msgs::PoseWithCovarianceStamped& msg) {
  x_pose = msg.pose.pose.position.x - x_odo;
  y_pose = msg.pose.pose.position.y - y_odo;

  tf::Pose pose;
  tf::poseMsgToTF(msg.pose.pose, pose);
  th_pose = tf::getYaw(pose.getRotation()) - th_odo;

  // x_pose = msg.pose.pose.position.x;
  // y_pose = msg.pose.pose.position.y;

  // tf::Pose pose;
  // tf::poseMsgToTF(msg.pose.pose, pose);
  // th_pose = tf::getYaw(pose.getRotation());
}


int main(int argc, char** argv){
  // Initialization
  ros::init(argc, argv, "tf_broadcaster");
  ros::NodeHandle nh_;
  ros::NodeHandle pnh_("~");

  ros::Subscriber odom_str_sub_, pose_sub_;

  // Set velocity string message subscriber
  odom_str_sub_ = nh_.subscribe("odo_pose_pge5", 50, &velCallback);

  // Set pose callback subscriber
  pose_sub_ = nh_.subscribe("initialpose", 2, &poseCallback);



  // Parameters
  nh_.param<std::string>("base_frame", base_frame_, "base_frame"); // Names the base tf frame.
  nh_.param<std::string>("odom_frame", odom_frame_, "odom_frame"); // Names the odom tf frame.
  nh_.param<std::string>("map_frame", map_frame_, "map"); // Names the map tf frame.
  nh_.param<std::string>("left_sensor_frame", left_sensor_frame_, "srf08_left"); // Names the frame of the front left sensor.
  nh_.param<std::string>("right_sensor_frame", right_sensor_frame_, "srf08_right"); // Names the frame of the right sensor.
  nh_.param<std::string>("front_sensor_frame", front_sensor_frame_, "srf08_front"); // Names the frame of the front sensor.

  // Loop
  ros::Rate rate(10.0);

  // tf objects
  tf::TransformBroadcaster br;
  tf::Transform transform;
  tf::Quaternion rotation;

  while (nh_.ok()){

    ros::spinOnce(); // Check for incoming messages

    // // Front sensor -> base transform
    // transform.setOrigin( tf::Vector3(0.135, 0.0, 0.0) ); // X - Y - Z (in m)
    // rotation.setRPY(0, 0, 0); // Third argument is theta (yaw) angle
    // transform.setRotation( rotation );
    // br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), base_frame_, front_sensor_frame_));

    // // Left sensor -> base transform
    // transform.setOrigin( tf::Vector3(0.075, 0.18, 0.0) );  // X - Y - Z (in m)
    // rotation.setRPY(0, 0, 70*PI/180); // Third argument is theta (yaw) angle
    // transform.setRotation( rotation );
    // br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), base_frame_, left_sensor_frame_));

    // // Right sensor -> base transform
    // transform.setOrigin( tf::Vector3(0.075, -0.18, 0.0) ); // X - Y - Z (in m)
    // rotation.setRPY(0, 0, -70*PI/180); // Third argument is theta (yaw) angle
    // transform.setRotation( rotation );
    // br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), base_frame_, right_sensor_frame_));

    // // Odom -> Base transform WORKING
    // transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
    // rotation.setRPY(0, 0, 0);
    // transform.setRotation( rotation );
    // br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), odom_frame_, base_frame_));

    // // Map -> Odom transform WORKING
    // transform.setOrigin( tf::Vector3(x_odo + x_pose, y_odo + y_pose, 0.0) );
    // rotation.setRPY(0, 0, th_odo + th_pose);
    // transform.setRotation( rotation );
    // br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), map_frame_, odom_frame_));


    // Map -> Base transform
    transform.setOrigin( tf::Vector3(x_pose + x_odo, y_pose + y_odo, 0.0) );
    rotation.setRPY(0, 0, th_pose + th_odo);
    transform.setRotation( rotation );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), map_frame_, base_frame_));

    // Map -> Odom transform
    transform.setOrigin( tf::Vector3(x_odo, y_odo, 0.0) );
    rotation.setRPY(0, 0, th_odo);
    transform.setRotation( rotation );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), map_frame_, odom_frame_));


    rate.sleep();
  }
  return 0;
};