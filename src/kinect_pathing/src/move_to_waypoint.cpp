/***************************************

Moves in a straight line towards waypoint

***************************************/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PointStamped.h"
#include <tf/transform_listener.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <sstream>
#include <algorithm>
#include "math.h"

using namespace std;

/* Global variables */
ros::Publisher command_pge5_pub_ ;
geometry_msgs::PointStamped waypoint;
tf::TransformListener* listener_wp = NULL; // Initialize the object as a pointer
string base_frame_, map_frame_;
double l_vel_max_, a_vel_max_; // Maximum velocities, passed by ROS parameters
string xstr, ystr, wstr;
double x, y, th; // Represent the robot's pose in the map
std_msgs::String msg, pubmsg;

/* Callback functions */
// Updates waypoint published in rviz
void wayPointCallback(const geometry_msgs::PointStamped::ConstPtr& msg)
{
  waypoint.point.x = msg->point.x;
  waypoint.point.y = msg->point.y;
  ROS_INFO("Point published! Moving to map coordinates: x: %3.3f y: %3.3f", waypoint.point.x, waypoint.point.y);
}

/* Helper functions */
// Goes in a straight line towards the goal. Updates ss string stream with all the information that needs to be passed in main() function
void moveNoObstacles(){
  // Initialization
    double diff;
    double angle;
    double l_vel, a_vel;
    std::stringstream ss;
    std::ostringstream linear_vel;
    std::ostringstream angular_vel;
  // Linear velocity processing
    l_vel = l_vel_max_ * sqrt( pow((waypoint.point.y - y), 2) + pow((waypoint.point.x - x), 2) );
    angle = atan2((waypoint.point.y - y), (waypoint.point.x - x));
  diff = fmod(th, M_PI); // Normalize theta
  if (angle > th)
  {
    diff = angle - th;
    if (diff > M_PI)
      diff = (-1) * ((2*M_PI - angle) + th);
  }
  else
  {
    diff = th - angle;
    if (diff > M_PI)
      diff = (2*M_PI - th) + angle;
    else
      diff *= -1;
  }
  diff = fmod(diff, M_PI); // Normalize diff to between -PI and PI using float modulus
  diff = diff / M_PI; // Normalize diff to one
  a_vel = 2 * a_vel_max_ * diff;
  // Velocity limiter
  if (l_vel > l_vel_max_)
    l_vel = l_vel_max_;
  if (abs(a_vel) > a_vel_max_) {
    if(a_vel > 0)
      a_vel = 2 * a_vel_max_;
    else if (a_vel < 0)
      a_vel = 2 * -a_vel_max_;
  }
  // Process to string message
  linear_vel << l_vel;
  angular_vel << a_vel;
  if (l_vel > 0.01)
  {
    ss << linear_vel.str();
    ss << " ";
    ss << angular_vel.str();
  }
  else
  {
    ss << "0";
    ss << " ";
    ss << "0";
  }
  pubmsg.data = ss.str();
}

int main(int argc, char **argv)
{
  // Initialization
  ros::init(argc, argv, "simple_obstacle_pathing");
  ros::NodeHandle n;
  ros::NodeHandle pnh_("~");

  // Parameters
  n.param<std::string>("base_frame", base_frame_, "base_frame"); // Names of the base_frame tf frame to subscribe to.
  n.param<std::string>("map_frame", map_frame_, "map"); // Names of the map tf frame to subscribe to.

  pnh_.param<double>("lin_vel_max", l_vel_max_, 1.0); // Maximum linear velocity
  pnh_.param<double>("ang_vel_max", a_vel_max_, 1.0); // Maximum angular velocity

  // Subscribers and Publishers
  command_pge5_pub_ = n.advertise<std_msgs::String>("command_pge5", 1000);
  ros::Subscriber point_sub = n.subscribe<geometry_msgs::PointStamped>("/waypoint", 1, &wayPointCallback);

  // TF objects to get pose
  tf::TransformListener listener; // TF listener to get robot pose
  tf::StampedTransform transform;
  listener_wp = new(tf::TransformListener); // TF listener to get waypoint. Needed so we can use the transformListener inside the waypoint callback function.
  ros::Rate loop_rate(15);
  while (ros::ok())
  {
    // Update pose of robot
    try {
      listener.waitForTransform(map_frame_, base_frame_, ros::Time(0), ros::Duration(10.0) );
      listener.lookupTransform(map_frame_, base_frame_, ros::Time(0), transform);
    }
    catch (tf::TransformException ex) {
      ROS_ERROR("%s",ex.what());
    }
    x = transform.getOrigin().x();
    y = transform.getOrigin().y();
    th = tf::getYaw(transform.getRotation());

    // If the robot is within a 20cm radius of the target waypoint or if a waypoint has not been yet specified, don't move.
    if( (abs(x - waypoint.point.x) < 0.10) && (abs(y - waypoint.point.y) < 0.10)) {
      std::stringstream tt;
      tt << "0.0";
      tt << " ";
      tt << "0.0";
      pubmsg.data = tt.str();
    }
    else {
      moveNoObstacles();
    }

    // Publish the velocity commands to ROS
    command_pge5_pub_.publish(pubmsg);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}