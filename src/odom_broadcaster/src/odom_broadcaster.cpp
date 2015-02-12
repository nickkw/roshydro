#include <ros/ros.h>
#include <std_msgs/String.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

// Publisher and subscriber objects
ros::Publisher odom_pub_;
ros::Subscriber odom_str_sub_;

// Implementation variables
std::string velstr, xmsg, ymsg, thmsg;
size_t strpos1, strpos2; // Used to get substrings from velmsg string

double x = 0.0;
double xlast = 0.0;
double y = 0.0;
double ylast = 0.0;
double th = 0.0;
double thlast = 0.0;

double vx;
double vy;
double vth;

// velCallback function takes incoming odometry message from the low level board
void velCallback(const std_msgs::String& str) {
  
  velstr = str.data;

  // // Get current time
  // current_time = ros::Time::now();

  // Retrieve message substrings
  strpos1 = velstr.find(" "); // Find the position of the space which delineates the x and y fields. Strpos returns the first match.
  strpos2 = velstr.find(" ", strpos1+1);  // Find the position of the second space which delineates the y and th fields. Note strpos argument starts the search on the character directly after the first space.

  xmsg = velstr.substr(0, strpos1-1); // Take the substring of velstr from the beginning to the last character before the space as the xmsg
  ymsg = velstr.substr(strpos1+1, strpos2-1);
  thmsg = velstr.substr(strpos2);

  // Convert message substrings to doubles and store values as current
  x = atof(xmsg.c_str());
  y = atof(ymsg.c_str());
  th = atof(thmsg.c_str());
}

int main(int argc, char** argv){
  ros::init(argc, argv, "odom_broadcaster");

  ros::NodeHandle n;

  // Parameters
  std::string listen;
  std::string broadcast;
  std::string base_frame;
  n.param<std::string>("listen_topic", listen, "odo_pose_pge5");
  n.param<std::string>("broadcast_topic", broadcast, "odom_frame");
  n.param<std::string>("base_frame", base_frame,"base_frame");

  // Set publisher and tf objects
  odom_pub_= n.advertise<nav_msgs::Odometry>(broadcast, 50);
  tf::TransformBroadcaster odom_broadcaster;


  // Set velocity string message subscriber
  odom_str_sub_ = n.subscribe(listen, 50, &velCallback);
  
  // Initialize ros::Time objects
  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(50.0);
  while(n.ok()){

    ros::spinOnce(); // check for incoming messages
    current_time = ros::Time::now();

    // Determine velocity based on current and last position. Use dv = dr/dt
    double dt = (current_time - last_time).toSec();

    vx = (x - xlast) / dt;
    vy = (y - ylast) / dt;
    vth = (th - thlast) / dt;

    // Update the new values as the previous values

    xlast = x;
    ylast = y;
    thlast = th;

    // Since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    // First, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = broadcast;
    odom_trans.child_frame_id = base_frame;

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    // Send the transform
    odom_broadcaster.sendTransform(odom_trans);

    // Next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = broadcast;

    // Set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    // Set the velocity
    odom.child_frame_id = base_frame;
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //publish the message
    odom_pub_.publish(odom);

    last_time = current_time;
    r.sleep();
  }
}
