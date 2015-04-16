#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include <string>
#include <sstream>
#include <sensor_msgs/Joy.h>
using namespace std;

ros::Publisher vel_pub_;
ros::Subscriber joy_sub_;

int linear_, angular_, turbo_, kill_pge5_process_;
double turbo_factor_;
double l_scale_, a_scale_;

float l_vel;
float a_vel;


void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  if(joy->buttons[kill_pge5_process_] == 1) {
    l_vel = 101;
  }
 else if(joy->buttons[turbo_] == 1) {
    l_vel = turbo_factor_ * l_scale_ * joy->axes[linear_];
  }
  else {
    l_vel = l_scale_ * joy->axes[linear_];
  }

  a_vel = a_scale_ * joy->axes[angular_];

}

void move(){
  std_msgs::String msg;
  std::stringstream ss;
  std::ostringstream linear_vel;
  std::ostringstream angular_vel;

  // Format velocity message as a standard string message
  linear_vel << l_vel;
  cout << "l_vel = " << l_vel << endl;

  angular_vel << a_vel;
  cout << "a_vel = " << a_vel << endl;

  ss << linear_vel.str();
  ss << " ";
  ss << angular_vel.str();

  // if (l_vel > 0.01 | l_vel < -0.01) {
  //   ss << linear_vel.str();
  //   ss << " ";
  //   ss << angular_vel.str();
  // }
  // else {
  //   ss << "0";
  //   ss << " ";
  //   ss << "0";
  // }

  msg.data = ss.str();
  vel_pub_.publish(msg);

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop");
  ros::NodeHandle nh_;

  nh_.param("axis_linear", linear_, 1); // Defines which axis of the joystick is used for linear velocity. Default 1.
  nh_.param("axis_angular", angular_, 0); // Defines which axis of the joystick is used for angular velocity. Default 0.
  nh_.param("button_turbo", turbo_, 0); // Defines which button of the joystick is used for the turbo speed. Default 0.
  nh_.param("button_kill_pge5_process", kill_pge5_process_, 4); // Defines which button of the joystick is used to kill the low level process. Default 0.

  nh_.param("scale_angular", a_scale_, 1.0); // Defines the scale value of the angular velocity. Default 1.0
  nh_.param("scale_linear", l_scale_, 1.0); // Defines the scale value of the angular velocity. Default 1.0

  nh_.param("turbo_factor", turbo_factor_, 1.25); // Defines the scale value of the angular velocity. Default 1.0


  vel_pub_ = nh_.advertise<std_msgs::String>("command", 10);

  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &joyCallback);

  ros::Rate loop_rate(15);

  while(ros::ok()){
    move();
    ros::spinOnce();
    loop_rate.sleep();
  }
}