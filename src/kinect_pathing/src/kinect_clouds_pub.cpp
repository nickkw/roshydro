/***************************************

Kinect cloud publisher

TODO:
-Clean up code -- use better way than bool variable to publish cloud if available from callback

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
#include <pcl_ros/transforms.h>
#include <kinect_pathing.h>
#include <std_msgs/Bool.h>

// Global objects
sensor_msgs::PointCloud2 projection_cloud_msg, hull_cloud_msg;
bool newCloud;
tf::TransformListener* tl;
bool acceptNewCloud = false;

// Take new clouds message callback
void acceptNewCloudCallback(const std_msgs::Bool::ConstPtr &msg)
{
  acceptNewCloud = msg->data;
}

// Tsdf point cloud message callback
void tsdfCloudCallback(const sensor_msgs::PointCloud2::ConstPtr &msg)
{
  if (acceptNewCloud) {
    ROS_INFO("Pulling from newest published point cloud!\n");
    if (msg->width <= 0) {
      ROS_WARN("Input point cloud has no points! Check that kinect device is working.");
      return;
    }
    pcl::PointCloud<pcl::PointXYZI>::Ptr tsdf_cloud (new pcl::PointCloud<pcl::PointXYZI>);
    pcl::PCLPointCloud2::Ptr tsdf_cloud2 (new pcl::PCLPointCloud2);
    pcl::PointCloud<pcl::PointXYZ>::Ptr projection_cloud (new pcl::PointCloud<pcl::PointXYZ>), hull_cloud (new pcl::PointCloud<pcl::PointXYZ>);

    // Extract the projection and hull clouds
    pcl::fromROSMsg(*msg, *tsdf_cloud); // Convert from PointCloud2 message to PCL type
    pcl::toPCLPointCloud2 (*tsdf_cloud, *tsdf_cloud2);
    extractPath (tsdf_cloud2, projection_cloud, hull_cloud); // Extract the projection and the path

    // Rotate the point clouds to the proper orientation
    tf::StampedTransform tf;
    tf::Quaternion rotation;
    tf.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
    rotation.setRPY(-1.5708, 0, 0);
    tf.setRotation( rotation );
    pcl_ros::transformPointCloud (*projection_cloud, *projection_cloud, tf);
    pcl_ros::transformPointCloud (*hull_cloud, *hull_cloud, tf);

    // Transform all points in point cloud from wp_fixed_frame to map_frame_
    tl->lookupTransform("map", "wp_fixed_frame", ros::Time(0), tf);
    pcl_ros::transformPointCloud (*projection_cloud, *projection_cloud, tf);
    pcl_ros::transformPointCloud (*hull_cloud, *hull_cloud, tf);

    // Convert the point clouds to ros messages
    pcl::toROSMsg(*projection_cloud, projection_cloud_msg);
    pcl::toROSMsg(*hull_cloud, hull_cloud_msg);
    projection_cloud_msg.header.frame_id = hull_cloud_msg.header.frame_id = "map";
    projection_cloud_msg.header.stamp = hull_cloud_msg.header.stamp = ros::Time::now();
    newCloud = true;
    acceptNewCloud = false;
  }
}


int main(int argc, char **argv)
{
  // Initialization
  ros::init(argc, argv, "kinect_pathing");
  ros::NodeHandle nh;
  ros::NodeHandle pnh_("~");

  // Parameters
  bool request_new_saves_;
  double update_rate_;
  pnh_.param<bool>("request_new_saves", request_new_saves_, true); // Set whether or not the node will ask KinFuLS to save new point clouds
  pnh_.param<double>("update_rate", update_rate_, 1.0); // Set rate of saving in hz
  
  // Objects
  std_msgs::String pubmsg;
  pcl::PCDReader reader;
  pcl::PCLPointCloud2::Ptr cloud_input (new pcl::PCLPointCloud2);
  tf::TransformListener listener; // TF listener to get robot pose
  tl = &listener;

  // Subscribers and Publishers
  ros::Publisher projection_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("projection_cloud", 10);
  ros::Publisher hull_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("hull_cloud", 10);
  ros::Publisher kinfu_save_pub = nh.advertise<std_msgs::String>("/kinfu_save_pointcloud", 1);
  ros::Subscriber kinfu_tsdf_cloud_sub = nh.subscribe<sensor_msgs::PointCloud2>("kinfu_tsdf_cloud", 10, &tsdfCloudCallback);
  ros::Subscriber accept_cloud_sub = nh.subscribe<std_msgs::Bool>("accept_new_cloud", 1, &acceptNewCloudCallback);

  if (request_new_saves_)
    ROS_INFO_STREAM_ONCE("Publishing KinFu clouds at a rate of: " << update_rate_ << " Hz");

  ros::Rate loop_rate(update_rate_);
  while (ros::ok())
  {
    if (request_new_saves_) {
      // MESSY IMPLEMENTATION -- publish empty message to get kinfu node to save
      if (acceptNewCloud) {
        kinfu_save_pub.publish(pubmsg);
      }
    }

    // // Read raw point cloud from file
    // if (reader.read ("/home/nicholaskwan-wong/.ros/world.pcd", *cloud_input) < 0) {
    //   ROS_WARN("~/.ros/world.pcd is an empty file!");
    // }
    // else {
    //   extractPath (cloud_input, projection_cloud, hull_cloud); // Extract the projection and the path
    //   // Convert the point clouds to ros messages
    //   pcl::toROSMsg(*projection_cloud, projection_cloud_msg);
    //   pcl::toROSMsg(*hull_cloud, hull_cloud_msg);
    //   projection_cloud_msg.header.frame_id = hull_cloud_msg.header.frame_id = "cloud_frame";
    //   projection_cloud_msg.header.stamp = hull_cloud_msg.header.stamp = ros::Time::now();
    // }

    if (newCloud) {
      // Publish the messages
      hull_cloud_pub.publish(hull_cloud_msg);
      projection_cloud_pub.publish(projection_cloud_msg);
      newCloud = false;
    }


    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}