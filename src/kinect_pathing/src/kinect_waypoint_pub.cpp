/***************************************

Kinect waypoint selector / publisher

***************************************/

#include "ros/ros.h"
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/conversions.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl_conversions.h>
#include <algorithm>
#include <vector>
#include <tf/transform_listener.h>
#include <std_msgs/Bool.h>

#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PointStamped.h>
#include <pcl/common/transforms.h>
#include <pcl_ros/transforms.h>

using namespace std;

pcl::PointCloud<pcl::PointXYZRGB>::Ptr wp_cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
bool newCloud = false;
string base_frame_, map_frame_;
int num_waypoints;


// Waypoint point cloud message callback
void wpCloudCallback(const sensor_msgs::PointCloud2::ConstPtr &msg)
{
  pcl::fromROSMsg(*msg, *wp_cloud); // Convert from PointCloud2 message to PCL type

  num_waypoints = wp_cloud->points.size();
  if (num_waypoints == 0) {
    ROS_WARN("There are no waypoints in the received cloud!");
    newCloud = false;
    return;
  }
  ROS_INFO_STREAM("New cloud received! It contains " << num_waypoints << " waypoints.\n");
  newCloud = true;
  for(int i = 0; i < wp_cloud->points.size(); i++) {
    wp_cloud->points.at(i).r = 120; wp_cloud->points.at(i).g = 120; wp_cloud->points.at(i).b = 120; // 'Inactive' colour - grey
  }
}

int main(int argc, char **argv)
{
  // Initialization
  ros::init(argc, argv, "kinect_pathing");
  ros::NodeHandle nh;
  ros::NodeHandle pnh_("~");

  // Parameters
  nh.param<std::string>("base_frame", base_frame_, "base_frame"); // Names of the base_frame tf frame to subscribe to.
  nh.param<std::string>("map_frame", map_frame_, "map"); // Names of the map tf frame to subscribe to.
  
  // Objects
  tf::TransformListener listener; // TF listener to get robot pose
  tf::StampedTransform transform;
  geometry_msgs::PointStamped waypoint;
  double x,y;
  int current_wp_index = 0;
  int first_wp_index = 0; // First wp that was travelled in the cloud -- to make sure we don't travel around the circle more than once.
  bool new_wp = false;
  bool last_point = false; // True if the robot is headed towards the last waypoint in the cloud after doing a full circle
  bool done;
  int current_wp = 0;
  sensor_msgs::PointCloud2 hull_cloud_colour_msg;


  // Subscribers and Publishers
  ros::Publisher waypoint_pub = nh.advertise<geometry_msgs::PointStamped>("waypoint", 1);
  ros::Publisher hull_cloud_colour_pub = nh.advertise<sensor_msgs::PointCloud2>("waypoint_cloud", 10);
  ros::Subscriber wp_cloud_sub = nh.subscribe<sensor_msgs::PointCloud2>("hull_cloud", 20, &wpCloudCallback);
  
  ros::Rate loop_rate(15);
  while (ros::ok())
  {
    // Get current robot position
    try {
      listener.waitForTransform(map_frame_, base_frame_, ros::Time(0), ros::Duration(10.0) );
      listener.lookupTransform(map_frame_, base_frame_, ros::Time(0), transform);
    }
    catch (tf::TransformException ex) {
      ROS_ERROR("%s",ex.what());
    }
    x = transform.getOrigin().x();
    y = transform.getOrigin().y();

    // If we receive a new cloud, find the newest closest point to the current robot position
    if (newCloud) {
      done = false;
      current_wp_index = 0;
      current_wp = 1;


      // Find the closest waypoint to the current robot position
      float distance;
      float least_distance = sqrt( pow((wp_cloud->points.at(0).x - x),2) + pow((wp_cloud->points.at(0).y - y),2) ); // Initialize least distance as the first element
      for (int i = 0; i < num_waypoints; i++) {
        // Find the distance between each waypoint and the current robot position
        distance = sqrt( pow((wp_cloud->points.at(i).x - x),2) + pow((wp_cloud->points.at(i).y - y),2) );
        if (distance < least_distance) {
          current_wp_index = i; // Update the new closest waypoint
          least_distance = distance;
        }
      }
      first_wp_index = current_wp_index; // Set the first waypoint to be traversed
      // Update waypoint values
      newCloud = false;
      new_wp = true;
      last_point = false;
      waypoint.point.x = wp_cloud->points.at(current_wp_index).x;
      waypoint.point.y = wp_cloud->points.at(current_wp_index).y;
      wp_cloud->points.at(current_wp_index).r = 0; wp_cloud->points.at(current_wp_index).g = 0; wp_cloud->points.at(current_wp_index).b = 255; // 'First' colour - blue
    }

    // Otherwise, see if arrived close to current waypoint
    else if ( (abs(waypoint.point.x - x) < 0.20) && (abs(waypoint.point.y - y) < 0.20) && wp_cloud->points.size()){ // Make sure the cloud is non-empty to avoid out_of_range error
      // Check if arrived to last point
      if (last_point) {
        if(!done) {
          ROS_INFO_STREAM("PATH COMPLETE!");
          done = true;   
        }
      }

      // Otherwise pick the next point
      else {
        ROS_INFO("Waypoint %d of %d reached! (x: %3.2f y: %3.2f)", current_wp, num_waypoints, wp_cloud->points.at(current_wp_index).x, wp_cloud->points.at(current_wp_index).y);
        if (current_wp_index != first_wp_index) // Leave the first waypoint coloured blue
          wp_cloud->points.at(current_wp_index).r = 0; wp_cloud->points.at(current_wp_index).g = 255; wp_cloud->points.at(current_wp_index).b = 0; // 'Done' colour - green
        // Update new waypoint, being careful not to overrun the point vector
        if (current_wp_index+1 >= wp_cloud->points.size())
          current_wp_index = 0;
        else
          current_wp_index++;

        // If heading back towards the first waypoint (i.e. full circle), do not send a new wp once current wp is reached.
        if (current_wp_index == first_wp_index){
          ROS_INFO("HEADING TOWARDS THE STARTING WAYPOINT (x: %3.2f y: %3.2f)", wp_cloud->points.at(current_wp_index).x, wp_cloud->points.at(current_wp_index).y);
          last_point = true;
        }

        // Update waypoint values
        new_wp = true;
        current_wp++;
        waypoint.point.x = wp_cloud->points.at(current_wp_index).x;
        waypoint.point.y = wp_cloud->points.at(current_wp_index).y;
        wp_cloud->points.at(current_wp_index).r = 255; wp_cloud->points.at(current_wp_index).g = 255; wp_cloud->points.at(current_wp_index).b = 0; // 'Active' colour - yellow
        wp_cloud->points.at(first_wp_index).r = 0; wp_cloud->points.at(first_wp_index).g = 0; wp_cloud->points.at(first_wp_index).b = 255; // 'First' colour - blue
      }
    }

    // Publish the new waypoint if there is one
    if (new_wp) {
      new_wp = false;
      waypoint.header.frame_id = map_frame_;
      waypoint.header.stamp = ros::Time::now();
      if(!last_point)
        ROS_INFO("Heading towards waypoint %d of %d (x: %3.2f y: %3.2f)", current_wp, num_waypoints, wp_cloud->points.at(current_wp_index).x, wp_cloud->points.at(current_wp_index).y);
    }

    waypoint_pub.publish(waypoint);

    // Publish the coloured hull cloud
    pcl::toROSMsg(*wp_cloud, hull_cloud_colour_msg);
    hull_cloud_colour_pub.publish(hull_cloud_colour_msg);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}


