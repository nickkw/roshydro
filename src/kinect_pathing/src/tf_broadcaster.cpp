#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf/transform_broadcaster.h>

std::string base_frame_, odom_frame_, map_frame_, wp_fixed_frame_;

// Implementation variables
std::string velstr, xmsg, ymsg, thmsg;
size_t strpos1, strpos2; // Used to get substrings from velmsg string

double x_odo = 0.0;
double y_odo = 0.0;
double th_odo = 0.0;

double x_odo_raw = 0.0;
double y_odo_raw = 0.0;
double th_odo_raw = 0.0;

double x_odo_origin = 0.0;
double y_odo_origin = 0.0;
double th_odo_origin = 0.0;

double x_init_pose = 0.0;
double y_init_pose = 0.0;
double th_init_pose = 0.0;

double x_newmap_pose = 0.0;
double y_newmap_pose = 0.0;
double th_newmap_pose = 0.0;


// velCallback function takes incoming odometry message from the low level board
void velCallback(const std_msgs::String& str) {
  
  velstr = str.data;

  // Retrieve message substrings
  strpos1 = velstr.find(" "); // Find the position of the space which delineates the x_odo and y_odo fields. Strpos returns the first match.
  strpos2 = velstr.find(" ", strpos1+1);  // Find the position of the second space which delineates the y_odo and th_odo fields. Note strpos argument starts the search on the character directly after the first space.

  xmsg = velstr.substr(0, strpos1-1); // Take the substring of velstr from the beginning to the last character before the space as the xmsg
  ymsg = velstr.substr(strpos1+1, strpos2-1);
  thmsg = velstr.substr(strpos2);

  // If the odometry is restarted, reset the origin (messy implementation)
  if(atof(xmsg.c_str()) == 0.0 && atof(ymsg.c_str()) == 0.0 && atof(thmsg.c_str()) == 0.00) {
    x_odo_origin = 0;
    y_odo_origin = 0;
    th_odo_origin = 0;
  }

  // Convert message substrings to doubles and store values as current
  x_odo_raw = atof(xmsg.c_str()) - x_odo_origin;
  y_odo_raw = atof(ymsg.c_str()) - y_odo_origin;
  th_odo_raw = atof(thmsg.c_str()) - th_odo_origin;

  // Transform odometry messages into pose frame
  x_odo = x_odo_raw * cos(th_init_pose) - y_odo_raw * sin(th_init_pose); // Find new x' component relative to the new frame (since odometry hardware always reports relative to initial startup coordinates)
  y_odo = x_odo_raw * sin(th_init_pose) + y_odo_raw * cos(th_init_pose); // Find new y' component relative to the new frame (since odometry hardware always reports relative to initial startup coordinates)
  th_odo = th_odo_raw;
}

// poseCallback function takes 2D pose from rviz topic 'initialpose' and passes it on to the base_frame tf object in order to initialize the pose
void poseCallback(const geometry_msgs::PoseWithCovarianceStamped& msg) {

  // Implementation to 'reinitialize' odometry origin upon reception of a new pose
  x_odo_origin = x_odo_raw;
  y_odo_origin = y_odo_raw;
  th_odo_origin = th_odo_raw;

  // Update the pose
  x_init_pose = msg.pose.pose.position.x;
  y_init_pose = msg.pose.pose.position.y;

  tf::Pose pose;
  tf::poseMsgToTF(msg.pose.pose, pose);
  th_init_pose = tf::getYaw(pose.getRotation());
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
  nh_.param<std::string>("waypoint_frame", wp_fixed_frame_, "wp_fixed_frame"); // Names the base tf frame.
  
  // Loop
  ros::Rate rate(10.0);

  // tf objects
  tf::TransformBroadcaster br;
  tf::Transform transform;
  tf::Quaternion rotation;

  while (nh_.ok()){

    ros::spinOnce(); // Check for incoming messages

    // Map -> Base transform
    transform.setOrigin( tf::Vector3(x_init_pose + x_odo, y_init_pose + y_odo, 0.0) );
    rotation.setRPY(0, 0, th_init_pose + th_odo);
    transform.setRotation( rotation );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), map_frame_, base_frame_));

    // Map -> WP fixed frame transform
    // transform.setOrigin( tf::Vector3(x_init_pose + 0.10, y_init_pose + 0.17, 0.0) );
    transform.setOrigin( tf::Vector3(x_init_pose, y_init_pose, 0.0) );
    rotation.setRPY(0, 0, th_init_pose);
    transform.setRotation( rotation );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), map_frame_, wp_fixed_frame_));

    rate.sleep();
  }
  return 0;
};