#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"
#include "geometry_msgs/PointStamped.h"
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
// #include <string>
#include <sstream>
#include <algorithm>
#include "math.h"
using namespace std;

ros::Publisher command_pge5_pub_ ;
geometry_msgs::Point way_point;
float angle = 0;
int move_status = 0;

string x;
string y;
string w;

string left_sensor_frame_, right_sensor_frame_, front_sensor_frame_;
double l_vel_max_, a_vel_max_; // Maximum velocities, passed by ROS parameters
double range_thresh_;

float xf;
float yf;
float wf;

std_msgs::String msg, pubmsg;
// std::stringstream ss;
// std::ostringstream linear_vel;
// std::ostringstream angular_vel;

float range_l, range_r, range_f;  // Range on left, right, and front sensors, respectively

// Callback functions

void odometryCallback(const std_msgs::String::ConstPtr& msg)
{
  string data = msg->data.c_str();
  int pos = data.find(" ");
  x = data.substr(0, pos);
  string d = data.substr(pos+1);
  pos = d.find(" ");
  y = d.substr(0, pos);
  w = d.substr(pos+1);
  
  xf = atof(x.c_str());
  yf = atof(y.c_str());
  wf = atof(w.c_str()) * 57.324840764;
  if (wf < 0)
    wf += 360;
  //cout<< "x = " << x <<  endl;
  //cout<< "y = " << y <<  endl;  
  //cout<< "w = " << w <<  endl;
}

void wayPointCallback(const geometry_msgs::PointStamped::ConstPtr& msg)
{
 way_point.x = msg->point.x;
 way_point.y = msg->point.y;
 move_status = 1;
}

// Updates reported sensor ranges
void rangeCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  if(msg->header.frame_id == left_sensor_frame_) 
    range_l = msg->range;

  else if(msg->header.frame_id == right_sensor_frame_) 
    range_r = msg->range;

  else if(msg->header.frame_id == front_sensor_frame_) 
    range_f = msg->range;

  else
    ROS_ERROR_ONCE("Unknown sensor header frame detected! Ensure only the front and right sensors are being used and launched.");
}

/* Helper functions */

// Goes in a straight line towards the goal. Updates ss string stream with all the information that needs to be passed in main() function
void moveNoObstacles(){

  // Initialization
  float diff;
  float l_vel, a_vel;
  std::stringstream ss;
  std::ostringstream linear_vel;
  std::ostringstream angular_vel;

  if (move_status == 1)
  {
    // Angular velocity processing
    angle = atan( (way_point.y - yf) / (way_point.x - xf) ) * 57.324840764;
    if ( way_point.y > yf && way_point.x < xf)
      angle += 180;
    else if ( way_point.y < yf && way_point.x < xf )
      angle += 180;
    else if ( way_point.y < yf && way_point.x > xf )
      angle += 360;
    if (angle > wf)
    {
      diff = angle - wf;
      if (diff > 180)
        diff = (-1) * ((360 - angle) + wf);
    }
    else
    {
      diff = wf - angle;
      if (diff > 180)
        diff = (360 - wf) + angle;
      else
        diff *= -1;
    }

    // Linear velocity
    float l_vel = sqrt( pow((way_point.y - yf), 2) + pow((way_point.x - xf), 2) ) ;
    //float l_vel = abs((way_point.x - xf) / 2);
    if (l_vel > l_vel_max_)
      l_vel = l_vel_max_;

    // Process to string message
    linear_vel << l_vel;
    // cout << "l_vel = " << l_vel << endl;
    angular_vel << (diff/60);

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

  // Normalized range values
  // float range_f_norm = range_f / range_thresh_;
  // float range_l_norm = range_l / range_thresh_;
  // float range_r_norm = range_r / range_thresh_;

  // // Front sensor right in front of obstacle... stop and turn!
  // if(range_f < 0.30) {
  //   l_vel = 0; // Don't bump into the object!
  //   if(range_l < range_r) // Predict obstacle on left -- turn clockwise
  //     a_vel = -0.6;
  //   else if (range_r < range_l) // Predict obstacle on right -- turn counter-clockwise
  //     a_vel = 0.6;
  // }
  // // Front sensor closest to obstacle case -- turn to avoid obstacle
  // else if( (range_f < range_l) && (range_f < range_r) ){
  //   l_vel = range_f * 0.3; // The closer the sensor is to the obstacle, the slower it goes
  //   if (range_l < range_r) // Predict obstacle on left -- turn clockwise w/ gain set by how close the object is on the front sensor
  //     a_vel = -1*(0.5 + range_thresh_ - range_f);
  //   if (range_r < range_l) // Predict obstacle on right -- turn counter-clockwise w/ gain set by how close the object is on the front sensor
  //     a_vel = 0.5 + range_thresh_ - range_f;
  // }

  // // Side sensors too close to obstacle case -- turn slightly to avoid
  // else if (range_l < 0.25 || range_r < 0.25) {
  //   l_vel = 0.1; // Give a slight bit of forward velocity
  //   if (range_l < range_r) // Turn clockwise
  //     a_vel = -0.6;
  //   else if (range_r < range_l) // Turn counter clockwise
  //     a_vel = 0.6;
  // }

  // // Side sensors closer than front sensor obstacle case -- drive forward
  // else {
  //   l_vel = std::min((float)0.3, std::min(range_l, range_r)); // Linear velocity is the lesser of 0.3 or the normalized distance ranged 
  //   a_vel = 0; // Don't turn at all
  // }

  if(range_f < 0.30) {
    l_vel = 0;
    a_vel = 0.61;
  }
  else if ((range_l < 0.2) || (range_r < 0.2)) {
    if (range_l < range_r) { // Predict obstacle on left -- turn clockwise w/ gain set by how close the object is on the front sensor
      l_vel = 0.41;
      a_vel = -0.62;
    } 

    if (range_r < range_l) {
      // Predict obstacle on right -- turn counter-clockwise w/ gain set by how close the object is on the front sensor
      l_vel = 0.42;
      a_vel = 0.63;
    } 
  }
  else {
    l_vel = 0.31;
    a_vel = 0.0;
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

int main(int argc, char **argv)
{
  // Initialization
  ros::init(argc, argv, "simple_obstacle_pathing");
  ros::NodeHandle n;
  ros::NodeHandle pnh_("~");

  // Parameters
  pnh_.param<std::string>("left_sensor_frame", left_sensor_frame_, "srf08_left"); // Names the frame of the front left sensor.
  pnh_.param<std::string>("right_sensor_frame", right_sensor_frame_, "srf08_right"); // Names the frame of the right sensor.
  pnh_.param<std::string>("front_sensor_frame", front_sensor_frame_, "srf08_front"); // Names the frame of the front sensor.
  pnh_.param<double>("lin_vel_max", l_vel_max_, 1.0);  // Maximum linear velocity
  pnh_.param<double>("ang_vel_max", a_vel_max_, 1.0);  // Maximum angular velocity

  pnh_.param<double>("obstacle_range_threshold", range_thresh_, 1.0);  // [in metres] Minimum (ceiling) proximity for an object to be considered an obstacle

  // Subscribers and Publishers
  command_pge5_pub_ = n.advertise<std_msgs::String>("command_pge5", 1000);

  ros::Subscriber range_sub = n.subscribe<sensor_msgs::Range>("range", 20, &rangeCallback);
  ros::Subscriber point_sub = n.subscribe<geometry_msgs::PointStamped>("/clicked_point", 1, &wayPointCallback);
  ros::Subscriber odom_sub = n.subscribe("odo_pose_pge5", 10, &odometryCallback);

  ros::Rate loop_rate(15);



  while (ros::ok())
  {
    if((range_l == 0 || range_r == 0 || range_f == 0)) {
      std::stringstream tt;
      // ROS_ERROR("A sensor is malfunctioning! (A sensor returned range 0)");
      // Set velocity to 0
      tt << "0";
      tt << " ";
      tt << "0";
      pubmsg.data = tt.str();
    }

    // if all sensor ranges > range_thresh_ cm, move on
    else if((range_f > range_thresh_) && (range_l > range_thresh_) && (range_r > range_thresh_)) {
      // moveNoObstacles();
      // Temporary until waypoint is implemented
      std::stringstream tt;
      tt << "0.222";
      tt << " ";
      tt << "0";
      pubmsg.data = tt.str();
    }

    // Else avoid obstacle
    else {
      avoidObstacle();
    }

    // Publish the velocity commands to ROS

    command_pge5_pub_.publish(pubmsg);

    ros::spinOnce();

    loop_rate.sleep();
 }
  
 return 0;
}
