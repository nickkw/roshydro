Required packages

- kinect2_bridge (follow instructions for setting up kinect2 / kinect)
- Modified kinfuLS package (included in repository)

Necessary modifications to PCL version 1.8 to use kinect_pathing package

- Add the following to {pcl_directory}/src/gpu/kinfu_large_scale/src/kinfu.cpp after the function definition for void pcl::gpu::kinfuLS::KinfuTracker::extractAndSaveWorld () {}

	pcl::PointCloud<pcl::PointXYZI>::Ptr
	pcl::gpu::kinfuLS::KinfuTracker::extractWorldPointCloud()
	{
	  //extract current volume to world model
	  PCL_INFO("Extracting point cloud...");
	  cyclical_.checkForShift(tsdf_volume_, getCameraPose (), 0.6 * volume_size_, true, true, true); // this will force the extraction of the whole cube.
	  PCL_INFO("Done\n");
	  
	  finished_ = false; // TODO maybe we could add a bool param to prevent kinfuLS from exiting after we saved the current world model
	  
	  // std::cout << "pcl   " << tsdf_cloud_ptr << std::endl;
	  return cyclical_.getWorldModel ()->getWorld ();
	}

-Add the following to {pcl_directory}/src/gpu/kinfu_large_scale/include/kinfu.h after the function prototype for void extractAndSaveWorld () {}

	  /** \brief Extract the world and return a pointer to a point cloud of the world.
	    */
	  pcl::PointCloud<pcl::PointXYZI>::Ptr
	  extractWorldPointCloud();

- If getting runtime error of insufficient device memory, change the values of line 68 of {pcl_directory}/src/gpu/kinfu_large_scale/include/device.h
	to lower values which are multiples of 32. On an Nvidia GeForce GT 635, a value of 384 would work well if not running any other graphics-intensive
	processes. Changing this value will change the resolution of the tsdf cloud, so more memory on the GPU = better resolution.

Necessary modifications to this kinfuLS ROS package

- 