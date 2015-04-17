/***************************************

Kinect cloud publisher

TODO:
-Have it maybe so that instead of constantly updating the map, updates are done via callback?
-Figure out if there is a better way to retrieve TSDF point clodus from kinfuls node

***************************************/

#include "ros/ros.h"
#include <std_msgs/String.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/conversions.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl/PCLPointCloud2.h>
#include <pcl_conversions.h>

#include <kinect_pathing.h>


int main(int argc, char **argv)
{
  // Initialization
  ros::init(argc, argv, "kinect_pathing");
  ros::NodeHandle nh;
  ros::NodeHandle pnh_("~");

  // Parameters
  bool request_new_saves_;
  pnh_.param<bool>("request_new_saves", request_new_saves_, false); // Set whether or not the node will ask KinFuLS to save new point clouds
  
  // Objects
  sensor_msgs::PointCloud2 projection_cloud_msg, hull_cloud_msg;
  std_msgs::String pubmsg;
  pcl::PCLPointCloud2::Ptr cloud_input (new pcl::PCLPointCloud2);
  pcl::PointCloud<pcl::PointXYZ>::Ptr projection_cloud (new pcl::PointCloud<pcl::PointXYZ>), hull_cloud (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PCDReader reader;

  // Subscribers and Publishers
  ros::Publisher projection_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("projection_cloud", 10);
  ros::Publisher hull_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("hull_cloud", 10);
  ros::Publisher kinfu_save_pub = nh.advertise<std_msgs::String>("/kinfu_save_pointcloud", 1);

  
  ros::Rate loop_rate(0.25);
  while (ros::ok())
  {
    if (request_new_saves_) {
      // MESSY IMPLEMENTATION -- publish empty message to get kinfu node to save
      ROS_INFO("Sending message to KinFu to save cloud.");
      kinfu_save_pub.publish(pubmsg);
    }

    // Read raw point cloud from file
    if (reader.read ("/home/nicholaskwan-wong/.ros/world.pcd", *cloud_input) < 0) {
      ROS_WARN("~/.ros/world.pcd is an empty file!");
    }
    else {
      extractPath (cloud_input, projection_cloud, hull_cloud); // Extract the projection and the path
      // Convert the point clouds to ros messages
      pcl::toROSMsg(*projection_cloud, projection_cloud_msg);
      pcl::toROSMsg(*hull_cloud, hull_cloud_msg);
      projection_cloud_msg.header.frame_id = hull_cloud_msg.header.frame_id = "cloud_frame";
      projection_cloud_msg.header.stamp = hull_cloud_msg.header.stamp = ros::Time::now();
    }

    // Publish the messages
    hull_cloud_pub.publish(hull_cloud_msg);
    projection_cloud_pub.publish(projection_cloud_msg);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}