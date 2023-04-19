# What version of CMake is needed?
cmake_minimum_required(VERSION 2.8.3)

# The name of this package.
project(distance)

# Find the catkin build system, and any other packages on which we depend.

find_package(catkin REQUIRED COMPONENTS 
	roscpp 
	rospy
	geometry_msgs 
	turtlesim 
	sensor_msgs
	std_msgs 
	message_generation
	cv_bridge
	nav_msgs
	
	
)



add_message_files(
        DIRECTORY msg
        FILES
        BoundingBox.msg
        BoundingBoxes.msg
)

generate_messages(
        DEPENDENCIES
        std_msgs
)

# Declare our catkin package.
catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS 
  roscpp 
  message_runtime 
  sensor_msgs
  opencv2
  cv_bridge
  
)


# Specify locations of header files.
include_directories(include ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} )
include_directories(/usr/include/opencv4/)

# Declare the executable, along with its source files.
add_executable(depth src/depth.cpp)

# Specify libraries against which to link.
target_link_libraries(depth ${catkin_LIBRARIES})

