//codi interessant: https://github.com/IntelRealSense/librealsense/blob/master/wrappers/opencv/dnn/rs-dnn.cpp
#include <typeinfo>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>  // For geometry_msgs::Twist
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h> 
#include <tf/transform_listener.h>
#include <stdlib.h> // For rand() and RAND_MAX
#include <typeinfo>
#include <iostream>
#include <thread>
#include <chrono>
//image:
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <tf/transform_listener.h>
#include <nav_msgs/OccupancyGrid.h>



using namespace cv;

//
int w = 1.5;
int h = 1.5;
int pixels_r = 10;
int pixels_c = 5;
int start_r = 0;
int start_c = 2;
float resolution = 0.1;
int num_times_pos=4;

//lined drawing  from pt1 to pt2 color white(255,255,255)  1-pixel thickness, 8-connected line, and 0-shift
//line(grHistrogram,pt1,pt2,Scalar(255,255,255),1,8,0);  
// colors cv::Mat map(pixels,pixels, CV_8UC3, Scalar(0,0,0));// mapa(pixels,pixels);
cv::Mat map(pixels_r,pixels_c, CV_8UC1, Scalar(0));// mapa(pixels,pixels);

//callback function 
void callback_array(const visualization_msgs::MarkerArray& array){
  int num_markers=array.markers.size();
  map.at<uchar>(start_r,start_c)=100;
  for (int i=0; i<num_markers; i++){
    visualization_msgs::Marker marker = array.markers[i];
    float x = marker.pose.position.x;
    float y = marker.pose.position.y;
    int cell_c=int(marker.pose.position.y/resolution +start_c);
    int cell_r=int(marker.pose.position.x/resolution +start_r);
    float height =marker.pose.position.z;
    std::string classe=marker.text;
    //sleep x veure què passa
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    ROS_INFO_STREAM(classe <<"  "<< x <<"  " << y << "  "<<cell_c << "   " <<cell_r);
    
    //Evaluation of point: cell grows 0.1 everytime is found until 0.4, that goes to 1
    if(cell_r < pixels_r & cell_c< pixels_c & classe == "Canopy"){
      if (int val=map.at<uchar>(cell_r,cell_c) < 4){
      map.at<uchar>(cell_r,cell_c) = map.at<uchar>(cell_r,cell_c) + 1;
      
    }
      else{
      map.at<uchar>(cell_r,cell_c)=200;
    }
    }
    
    


    // Set Value
    int color=200;
    
    //map(0,0) = map.setTo((200));
    //map(1,1)=200;
    //Get Value
    

    
  }
  //Convert map to image to publish
  cv_bridge::CvImage out_msg;
  //out_msg.header   = map->header; // Same timestamp and tf frame as input image
  //out_msg.encoding = sensor_msgs::image_encodings::TYPE_32FC1; // Or whatever
  //out_msg.image    = map; // Your cv::Mat
  //map.at<uchar>(3,2) = 200;
  int val = map.at<uchar>(0,0);
  
  ROS_INFO_STREAM(map);
  
  
  //IplImage* imgMsgToCv(sensor_msgs::Image::ConstPtr image_message,string cv_encoding="passthrough")
  //color_image=msg;  //sensor_msgs::Image type
  //ROS_INFO_STREAM("linia 52 "<< point);
  //Convert a sensor_msgs::Image message to an OpenCV IplImage.
  
  //sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", map).toImageMsg();
  //pub.publish(msg);
  //sensor_msgs::CvBridge bridge;
  
}

void callback_map(const nav_msgs::OccupancyGrid& data){
  geometry_msgs::Pose origin=data.info.origin;
  ROS_INFO_STREAM(data.header);
}


int main(int argc, char **argv) {
  // Initialize the ROS system and become a node.
  ros::init(argc, argv, "mapa_reduit_node");
  ros::NodeHandle nh;

  
  // Create a publisher object.
  //ros::Publisher pub = nh.advertise<sensor_msgs::Image>("soc_igual", 100);
  //ROS_INFO_STREAM("pub created");
  // Create a subscriber object.
  ros::Subscriber pointer = nh.subscribe("/obj_info", 100,&callback_array);
  ros::Subscriber map = nh.subscribe("/rtabmap/grid_map", 100,&callback_map);
  //image_transport::Publisher pub = it_.advertise("camera/image", 1);
  
  //Creació del mapa per rviz
  nav_msgs::OccupancyGrid mapa;


  


  
  //cv::Mat m;//(pixels, pixels);

  //ros::Subscriber d_im_sub = nh.subscribe("/camera/depth/image_rect_raw", 100,&callback_depth_image);
  
  //ros::Subscriber boxes_sub = nh.subscribe("/yolov5/BoundingBoxes", 100,&callback_BoundingBoxes);

  

  // Seed the random number generator.
  //srand(time(0));

  // Loop at 2Hz until the node is shut down.
  ros::Rate rate(2);
  while(ros::ok()) {
    // Create and fill in the message.  The other four
    // fields, which are ignored by turtlesim, default to 0.
    
    //msg.linear.x = double(rand())/double(RAND_MAX);
    //msg.angular.z = 2*double(rand())/double(RAND_MAX) - 1;

    // Publish the message.
    //pub.publish(img);

    // Send a message to rosout with the details.
    //ROS_INFO_STREAM("Sending random velocity command:"<< " linear=" << msg.linear.x     << " angular=" << msg.angular.z);

    // Wait until it's time for another iteration.
    rate.sleep();
    // Let ROS take over.
    ros::spin();

  }
}
