#include <iostream>
#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/project_inliers.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/surface/concave_hull.h>
#include <pcl/common/transforms.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/radius_outlier_removal.h>

// Extract the path of the input point cloud and pass back to the function as pointers to the resultant clouds
void extractPath (pcl::PCLPointCloud2::Ptr input_cloud, pcl::PointCloud<pcl::PointXYZ>::Ptr projection_cloud, pcl::PointCloud<pcl::PointXYZ>::Ptr hull_cloud)
{
  pcl::PCLPointCloud2::Ptr cloud_original (new pcl::PCLPointCloud2), cloud_filtered1 (new pcl::PCLPointCloud2),  cloud_filtered2 (new pcl::PCLPointCloud2);
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered3 (new pcl::PointCloud<pcl::PointXYZ>), cloud_filtered4 (new pcl::PointCloud<pcl::PointXYZ>);

  // std::cerr << "PointCloud before filtering: " << cloud_original->width * cloud_original->height << " data points." << std::endl;

  // Create the intensity passthrough filtering object
  pcl::PassThrough<pcl::PCLPointCloud2> pass;
  pass.setInputCloud (input_cloud);
  pass.setFilterFieldName ("intensity");
  pass.setFilterLimits (-0.1,0.1);
  pass.filter (*cloud_filtered1);

  // // Create the z-axis passthrough filtering object
  // pass.setInputCloud (cloud_filtered1);
  // pass.setFilterFieldName ("y"); // Note that it is the 'y' axis in this case
  // pass.setFilterLimits (0,230); // Set z-range here
  // //pass.setFilterLimitsNegative (true);
  // pass.filter (*cloud_filtered1);

  // Create the downsample filter
  pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
  sor.setInputCloud (cloud_filtered1);
  sor.setLeafSize (1.5f, 1.5f, 1.5f);
  sor.filter (*cloud_filtered2);

  // Convert to the templated PointCloud
  pcl::fromPCLPointCloud2 (*cloud_filtered2, *cloud_filtered3);

  // Create the segmentation filter which will remove the ground plane
  pcl::ModelCoefficients::Ptr coefficients1 (new pcl::ModelCoefficients ());
  pcl::PointIndices::Ptr inliers (new pcl::PointIndices ());
  // Create the segmentation object
  pcl::SACSegmentation<pcl::PointXYZ> seg;
  seg.setOptimizeCoefficients (true);
  seg.setModelType (pcl::SACMODEL_PLANE);
  seg.setMethodType (pcl::SAC_RANSAC);
  seg.setMaxIterations (1000);
  seg.setDistanceThreshold (3.0);
  // Create the filtering object
  pcl::ExtractIndices<pcl::PointXYZ> extract;
  // Segment the largest planar component from the remaining cloud
  seg.setInputCloud (cloud_filtered3);
  seg.segment (*inliers, *coefficients1);
  // Extract the inliers
  extract.setInputCloud (cloud_filtered3);
  extract.setIndices (inliers);
  extract.setNegative (true);
  extract.filter (*cloud_filtered4);

  // Convert to the templated PointCloud
  pcl::toPCLPointCloud2 (*cloud_filtered4, *cloud_filtered1);

  // Create the projection filtering object
  // Create a set of planar coefficients to project on the xy plane
  pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients ());
  coefficients->values.resize (4);
  coefficients->values[0] = 0;
  coefficients->values[1] = 1.0;
  coefficients->values[2] = 0;
  coefficients->values[3] = 0;

  pcl::ProjectInliers<pcl::PCLPointCloud2> proj;
  proj.setModelType (pcl::SACMODEL_PLANE);
  proj.setInputCloud (cloud_filtered1);
  proj.setModelCoefficients (coefficients);
  proj.filter (*cloud_filtered2);

  // Create the radius outliers filtering object
  pcl::RadiusOutlierRemoval<pcl::PCLPointCloud2> outrem;
  outrem.setInputCloud(cloud_filtered2);
  outrem.setRadiusSearch(3.0f);
  outrem.setMinNeighborsInRadius (15);
  outrem.filter (*cloud_filtered1);
  // Filter second time
  outrem.setInputCloud(cloud_filtered1);
  outrem.setRadiusSearch(3.0f);
  outrem.setMinNeighborsInRadius (15);
  outrem.filter (*cloud_filtered2);
  // Filter third time
  outrem.setInputCloud(cloud_filtered2);
  outrem.setRadiusSearch(3.0f);
  outrem.setMinNeighborsInRadius (10);
  outrem.filter (*cloud_filtered2);
  // Convert to the templated PointCloud to template of PointXYZ type
  pcl::fromPCLPointCloud2 (*cloud_filtered2, *cloud_filtered3);

  // Find centroid and set as temporary origin (in preparation of scaling)
  Eigen::Affine3f transform1 = Eigen::Affine3f::Identity();
  Eigen::Matrix<float,4,1> centroid;
  pcl::compute3DCentroid(*cloud_filtered3, centroid); // Get centroid
  transform1.translation() << -centroid(0), -centroid(1), -centroid(2); // Shift origin to centroid
  // std::cerr << "Centroid: " << "x: " << centroid(0) << " y: " << centroid(1) << " z: " << centroid(2) << std::endl;
  pcl::transformPointCloud (*cloud_filtered3, *projection_cloud, transform1);

  // Scale the point cloud
  Eigen::Affine3f transform2 = Eigen::Affine3f::Identity();
  float scale = 2.0;
  transform2.scale(scale);
  pcl::transformPointCloud (*projection_cloud, *cloud_filtered4, transform2);

  // Create a Convex Hull representation of the projected inliers
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_hull (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::ConvexHull<pcl::PointXYZ> chull;
  chull.setInputCloud (cloud_filtered4);
  chull.reconstruct (*hull_cloud);

  // Downsample projection cloud further since we are using it for visualization purposes at this point
  pcl::VoxelGrid<pcl::PointXYZ> sor2;
  sor2.setInputCloud (projection_cloud);
  sor2.setLeafSize (5.0f, 5.0f, 5.0f);
  sor2.filter (*projection_cloud);

  // Set the origin back to it's original position
  transform1.translation() << centroid(0), centroid(1), centroid(2); // Shift origin back to original location
  pcl::transformPointCloud (*projection_cloud, *projection_cloud, transform1);
  pcl::transformPointCloud (*hull_cloud, *hull_cloud, transform1);

  // TODO -- less magic number -- figure out actual scale needed
  transform2.scale(0.0033);
  pcl::transformPointCloud (*projection_cloud, *projection_cloud, transform2);
  pcl::transformPointCloud (*hull_cloud, *hull_cloud, transform2);

  // pcl::PCDWriter writer;
  // writer.write ("table_scene_lms400_hull.pcd", *hull_cloud, false);
  // writer.write ("table_scene_lms400_projection.pcd", *projection_cloud, false);
}