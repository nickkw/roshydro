#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"
#include "geometry_msgs/PointStamped.h"
#include <tf/transform_listener.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <sstream>
#include <algorithm>
#include "math.h"

#define LEFT 0
#define RIGHT 1
#define FLIP 2

using namespace std;

/* Global variables */
ros::Publisher command_pge5_pub_ ;
geometry_msgs::PointStamped waypoint;

tf::TransformListener* listener_wp = NULL; // Initialize the object as a pointer

string left_sensor_frame_, right_sensor_frame_, front_left_sensor_frame_, front_right_sensor_frame_, front_sensor_frame_, base_frame_, map_frame_;
double l_vel_max_, a_vel_max_; // Maximum velocities, passed by ROS parameters
double range_thresh_;

bool movingToObjective = false;

bool avoiding = false;
bool turning = false;

float range_l, range_r, range_f_l, range_f_r, range_f;  // Range on left, right, and front sensors, respectively

int turnedDir;

string xstr, ystr, wstr;
double x, y, th;  // Represent the robot's pose in the map

std_msgs::String msg, pubmsg;
// std::stringstream ss;
// std::ostringstream linear_vel;
// std::ostringstream angular_vel;


/* Callback functions */

void wayPointCallback(const geometry_msgs::PointStamped::ConstPtr& msg)
{
  waypoint.point.x = msg->point.x;
  waypoint.point.y = msg->point.y;

  ROS_INFO("Point published! Moving to map coordinates:   x: %3.3f y: %3.3f", waypoint.point.x, waypoint.point.y);
}

// Updates reported sensor ranges
void rangeCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  if(msg->header.frame_id == left_sensor_frame_) 
    range_l = msg->range;

  else if(msg->header.frame_id == right_sensor_frame_) 
    range_r = msg->range;

  else if(msg->header.frame_id == front_left_sensor_frame_) 
    range_f_l = msg->range;

  else if(msg->header.frame_id == front_right_sensor_frame_) 
    range_f_r = msg->range;

  else if(msg->header.frame_id == front_sensor_frame_) 
    range_f = msg->range;

  else
    ROS_ERROR_ONCE("Unknown sensor header frame detected! Ensure only the front and right sensors are being used and launched.");
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

  diff = fmod(th, M_PI);  // Normalize theta
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

// Avoids obstacle within range_thresh_ based on sensor weightings
void avoidObstacle(){

  // Initialization
  std::stringstream ss;
  std::ostringstream linear_vel;
  std::ostringstream angular_vel;

  float l_vel, a_vel;

  // // Front sensors right in front of obstacle... stop and turn!
  if(range_f < 0.40 || range_f_l < 0.40 || range_f_r < 0.40) {
    l_vel = 0; // Don't bump into the object!
    // if(range_l < range_r || range_f_l < range_f_r) // Predict obstacle on left -- turn clockwise
    if(range_l < range_r) {  // Predict obstacle on left -- turn clockwise
      a_vel = -0.7;
      turnedDir = RIGHT;
    }
    // else if (range_r < range_l || range_f_r < range_f_l) // Predict obstacle on right -- turn counter-clockwise
    else if(range_r < range_l) { // Predict obstacle on right -- turn counter-clockwise
      a_vel = 0.7;
      turnedDir = LEFT;
    }
  }
  // Front sensor closest to obstacle case -- turn to avoid obstacle
  // else if( (range_f < range_l) && (range_f < range_r) ){
  else if( (fmin(fmin(range_f, range_f_l),range_f_r) < range_l) && (fmin(fmin(range_f, range_f_l),range_f_r) < range_r) ){
    l_vel = fmin(fmin(range_f, range_f_l),range_f_r); // The closer the sensor is to the obstacle, the slower it goes
    if (range_l < range_r) // Predict obstacle on left -- turn clockwise w/ gain set by how close the object is on the front sensor
      a_vel = -1*(0.6 + range_thresh_ - fmin(fmin(range_f, range_f_l), range_f_r));
    if (range_r < range_l) // Predict obstacle on right -- turn counter-clockwise w/ gain set by how close the object is on the front sensor
      a_vel = 0.6 + range_thresh_ - fmin(fmin(range_f, range_f_l), range_f_r);
  }

  // Side sensors too close to obstacle case -- turn slightly to avoid
  else if (range_l < 0.35 || range_r < 0.35) {
    l_vel = 0.4; // Give a slight bit of forward velocity
    if (range_l < range_r) // Turn clockwise
      a_vel = -0.6;
    else if (range_r < range_l) // Turn counter clockwise
      a_vel = 0.6;
  }

  // Side sensors closer than front sensor obstacle case -- drive forward
  else {
    l_vel = fmin((float)0.5, fmin(range_l, range_r)); // Linear velocity is the lesser of 0.3 or the normalized distance ranged 
    a_vel = 0; // Don't turn at all
  }


  // Scale velocities to max parameters, min prevents any errors in normalizing in the above code
  l_vel = std::min(l_vel * l_vel_max_, l_vel_max_);
  a_vel = std::min(a_vel * a_vel_max_, a_vel_max_);

  // Format velocities into string
  linear_vel << l_vel;
  angular_vel << a_vel;

  ss << linear_vel.str();
  ss << " ";
  ss << angular_vel.str(); 

  pubmsg.data = ss.str();
}


int guessDirection(){
  int direction;
  double angle;

  angle = atan2((waypoint.point.y - y), (waypoint.point.x - x));
  if(angle < M_PI/2 && angle > -M_PI/2){
    direction = RIGHT;
  }
  else {
    direction = LEFT;
  }
  return direction;
}

// Rotate the robot by angle amount.
void turn(double angle){

  std::stringstream ss;
  std::ostringstream linear_vel;
  std::ostringstream angular_vel;
  float l_vel, a_vel;
  static double init_angle;

  // If it's the first time entering the turning state, set the initial angle
  if(!turning)
    init_angle = th;

  static double goal_angle = angle;

  l_vel = 0;
  a_vel = 0;

  if(abs(th - init_angle) > abs(angle)){ // Turn is complete!
    turning = false;
  }

  else { // Otherwise, keep turning.
    turning = true;

    if(angle > 0) {  // Left turn
      a_vel = a_vel_max_ * 0.5;
    }
    else if (angle < 0) { // Right turn
      a_vel = -1 * a_vel_max_ * 0.5;
    }
  }

  // Publish
  linear_vel << l_vel;
  angular_vel << a_vel;

  ss << linear_vel.str();
  ss << " ";
  ss << angular_vel.str(); 

  pubmsg.data = ss.str();
}

// Avoids obstacle within range_thresh_ based on sensor weightings
void avoidObstacle2(){

  // Initialization
  std::stringstream ss;
  std::ostringstream linear_vel;
  std::ostringstream angular_vel;
  float l_vel, a_vel;
  float angle;

  // If it's the first time entering the avoidance state, guess a direction to go
  if(!avoiding){
    turnedDir = guessDirection();
    turning = false;

    if(turnedDir == LEFT){
      angle = M_PI/2;
    }
    else if(turnedDir == RIGHT){
      angle = -M_PI/2;
    }
    turn(angle);
    avoiding = true;
  }

  // Complete a turn if turning
  if(turning){
    if(turnedDir == LEFT){
      angle = M_PI/2;
    }
    else if(turnedDir == RIGHT){
      angle = -M_PI/2;
    }
    else if(turnedDir == FLIP){
      angle = M_PI;
    }
    turn(angle);
  }

  // Otherwise drive forward and avoid obstacles on either side.
  else {
    l_vel = fmin(fmin(range_f, range_f_l),range_f_r); // Drive forward

    if (range_l < 0.40 || range_r < 0.40) { // If the side sensors is a little close to the obstacle, turn.
      // l_vel = 0.4; // Give a slight bit of forward velocity
      if (range_l < range_r) // Turn right
        a_vel = -0.3;
      else if (range_r < range_l) // Turn left
        a_vel = 0.3;
    }
    else {
      a_vel = 0;
    }

    // Scale velocities to max parameters, min prevents any errors in normalizing in the above code
    l_vel = fmin(l_vel * l_vel_max_, l_vel_max_);
    a_vel = fmin(a_vel * a_vel_max_, a_vel_max_);

    // Format velocities into string
    linear_vel << l_vel;
    angular_vel << a_vel;

    ss << linear_vel.str();
    ss << " ";
    ss << angular_vel.str(); 

    pubmsg.data = ss.str();

    // An obstacle has appeared in front! Alert the system for the next iteration.
    // if (range_f < 0.30 || range_f_l < 0.30 || range_f_r < 0.30) {
    if (range_f < 0.30 && turning == false) {
      turning = false;
      angle = M_PI;
      turnedDir = FLIP;
      turn(angle);
    }
  }
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

  pnh_.param<std::string>("left_sensor_frame", left_sensor_frame_, "srf08_left"); // Names the frame of the front left sensor.
  pnh_.param<std::string>("right_sensor_frame", right_sensor_frame_, "srf08_right"); // Names the frame of the right sensor.
  pnh_.param<std::string>("front_left_sensor_frame", front_left_sensor_frame_, "srf08_front_left"); // Names the frame of the front left sensor.
  pnh_.param<std::string>("front_right_sensor_frame", front_right_sensor_frame_, "srf08_front_right"); // Names the frame of the right sensor.
  pnh_.param<std::string>("front_sensor_frame", front_sensor_frame_, "srf08_front"); // Names the frame of the front sensor.

  pnh_.param<double>("lin_vel_max", l_vel_max_, 1.0);  // Maximum linear velocity
  pnh_.param<double>("ang_vel_max", a_vel_max_, 1.0);  // Maximum angular velocity

  pnh_.param<double>("obstacle_range_threshold", range_thresh_, 1.0);  // [in metres] Minimum (ceiling) proximity for an object to be considered an obstacle

  // Subscribers and Publishers
  command_pge5_pub_ = n.advertise<std_msgs::String>("command_pge5", 1000);

  ros::Subscriber range_sub = n.subscribe<sensor_msgs::Range>("range", 20, &rangeCallback);
  ros::Subscriber point_sub = n.subscribe<geometry_msgs::PointStamped>("/clicked_point", 1, &wayPointCallback);

  // TF objects to get pose
  tf::TransformListener listener; // TF listener to get robot pose
  tf::StampedTransform transform;

  listener_wp = new(tf::TransformListener); // TF listener to get waypoint. Needed so we can use the transformListener inside the waypoint callback function.

  ros::Rate loop_rate(15);

  while (ros::ok())
  {
    // Malfunctioning range sensor case
    if(range_l == 0 || range_r == 0 || range_f == 0 || range_f_r == 0 || range_f_l == 0) {
      std::stringstream tt;
      // ROS_ERROR("A sensor is malfunctioning! (A sensor returned range 0)");
      // Set velocity to 0
      tt << "0";
      tt << " ";
      tt << "0";
      pubmsg.data = tt.str();
      }

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
    if( (abs(x - waypoint.point.x) < 0.15) && (abs(y - waypoint.point.y) < 0.15)) {
      if(movingToObjective) {
        ROS_INFO("Reached objective!");
        movingToObjective = false;
      }
      avoiding = false;
      std::stringstream tt;
      tt << "0.0";
      tt << " ";
      tt << "0.0";
      pubmsg.data = tt.str();
    }

    // Check avoid obstacle case first
    else if((range_f < range_thresh_) || (range_f_l < range_thresh_) || (range_f_r < range_thresh_) || (range_l < range_thresh_) || (range_r < range_thresh_)) {
      
      avoidObstacle2();

    }

    // if all sensor ranges > range_thresh_ cm, move on
    else if((range_f > range_thresh_) && (range_f_l > range_thresh_) && (range_f_r > range_thresh_) && (range_l > range_thresh_) && (range_r > range_thresh_)) {

      movingToObjective = true;
      avoiding = false;
      moveNoObstacles();

      // // Temporary until waypoint is implemented
      // std::stringstream tt;
      // tt << "0.222";
      // tt << " ";
      // tt << "0";
      // pubmsg.data = tt.str();
    }

    // If not sure what to do, simply stop
    else {
      std::stringstream tt;
      tt << "0.0";
      tt << " ";
      tt << "0.0";
      pubmsg.data = tt.str();
    }

    // Publish the velocity commands to ROS

    command_pge5_pub_.publish(pubmsg);

    ros::spinOnce();

    loop_rate.sleep();
 }
  
 return 0;
}
