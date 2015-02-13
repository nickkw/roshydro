#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PointStamped.h"
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <string>
#include <sstream>
#include "math.h"
using namespace std;

ros::Publisher command_pge5_pub_ ;
geometry_msgs::Point way_point;
float angle = 0;
int move_status = 0;

string x;
string y;
string w;

float xf;
float yf;
float wf;

std_msgs::String msg;
std::stringstream ss;
std::ostringstream linear_vel;
std::ostringstream angular_vel;

float range_l, range_r, range_f;  // Range on left, right, and front sensors, respectively

float l_vel_max_, a_vel_max_; // Maximum velocities, passed by ROS parameters

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
void rangeCallback(const sensor_msgs::Range& msg)
{
  if(msg.header.frame_id == left_sensor_frame_) 
    range_l = msg.range;

  else if(msg.header.frame_id == right_sensor_frame_) 
    range_r = msg.range;

  else if(msg.header.frame_id == front_sensor_frame_) 
    range_f = msg.range;

  else
    ROS_ERROR_ONCE("Unknown sensor header frame detected! Ensure only the front and right sensors are being used and launched.")
}

/* Helper functions */


// Goes in a straight line towards the goal. Updates ss string stream with all the information that needs to be passed in main() function
void moveNoObstacles(){

  // Initialization
  float diff;

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
}

void avoidObstacle(){
  // Object close on front + side sensor, turn  until obstacle only on one side - turn at a sharper rate (greater angular gain) the closer it is to the front sensor
  // Object on side sensor only - move forward slowly until 

  float l_vel, a_vel;


  if(range_l > range_r){

  }

  float l_vel = l_vel_max_ * 
}

int main(int argc, char **argv)
{
  // Initialization
  ros::init(argc, argv, "move_to_target_pge5");
  ros::NodeHandle n;
  ros::NodeHandle pnh_("~");

  // Parameters
  pnh_.param<std::string>("left_sensor_frame", left_sensor_frame_, "srf08_left"); // Names the frame of the front left sensor.
  pnh_.param<std::string>("right_sensor_frame", right_sensor_frame_, "srf08_right"); // Names the frame of the right sensor.
  pnh_.param<std::string>("right_sensor_frame", front_sensor_frame_, "srf08_front"); // Names the frame of the front sensor.
  pnh_.param<float>("lin_vel_max", l_vel_max_, 1.0);  // Maximum linear velocity
  pnh_.param<float>("ang_vel_max", a_vel_max_, 1.0);  // Maximum angular velocity

  // Subscribers and Publishers
  command_pge5_pub_ = n.advertise<std_msgs::String>("command_pge5", 1000);

  ros::Subscriber range_sub = n.subscribe<sensor_msgs::Range>("range", 20, wayPointCallback);
  ros::Subscriber point_sub = n.subscribe<geometry_msgs::PointStamped>("/clicked_point", 1, wayPointCallback);
  ros::Subscriber odom_sub = n.subscribe("odo_pose_pge5", 10, &odometryCallback);

  ros::Rate loop_rate(15);

  while (ros::ok())
  {
   // if all sensor ranges > 50 cm, move on
   moveNoObstacles();

   // Else avoid obstacle


  // Publish the velocity commands to ROS
   msg.data = ss.str();
   command_pge5_pub_.publish(msg);

   ros::spinOnce();

   loop_rate.sleep();
 }
 return 0;
}
