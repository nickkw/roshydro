#ifndef KINECT_PATHING_H__
#define KINECT_PATHING_H__

// Extract the path of the input point cloud and pass back to the function as pointers to the resultant clouds
void extractPath (pcl::PCLPointCloud2::Ptr input_cloud, pcl::PointCloud<pcl::PointXYZ>::Ptr projection_cloud, pcl::PointCloud<pcl::PointXYZ>::Ptr hull_cloud);

#endif