//codi interessant: https://github.com/IntelRealSense/librealsense/blob/master/wrappers/opencv/dnn/rs-dnn.cpp



// This program publishes randomly-generated velocity
// messages for turtlesim.
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>  // For geometry_msgs::Twist
#include <stdlib.h> // For rand() and RAND_MAX
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <distance/BoundingBox.h>
#include <distance/BoundingBoxes.h>
#include <cv_bridge/cv_bridge.h>
#include <librealsense2/rs.hpp>
//#include "opencv/cv.h"
//#include <opencv/highgui.h>
//#include <bits/stdc++.h>
//#include <list>
#include <typeinfo>
#include <iostream>

sensor_msgs::Image color_image;
sensor_msgs::Image depth_image;
distance::BoundingBoxes boxes;

float fx=636.4285888671875;
float fy=635.8297119140625;
//principal point
float ppx=636.2267456054688;
float ppy=378.491455078125;

float get_distance(float x, float y){
  int a =2.1;
  //float punt[2] -> punt;

  return a;
}

void position_extraction(float xmin, float ymin, float xmax, float ymax){
  
  float x_punt_mig=(xmax-xmin)/2+xmin;
  float y_punt_mig=(ymax-ymin)/2+ymin;

  //ROS_INFO_STREAM(" punt mig "<<x_punt_mig);
  float distance = get_distance(x_punt_mig, y_punt_mig);
  //std::vector<distance::BoundingBox> bboxes = msg->bounding_boxes;
  //list_box=box.bounding_boxes;
  //box = boxes.BoundingBoxes.bounding_boxes; 
  //box = boxes.bounding_boxes[0];
  //for (const auto& bbox : bboxes) {
  //      ROS_INFO_STREAM(bbox.xmin<<bbox.ymin<< bbox.xmax<< bbox.ymax);
  //  }
}



//callback function 
void callback_image(const sensor_msgs::ImageConstPtr& msg){
  cv_bridge::CvImagePtr cv_ptr;
  /*try
  {
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
  cv::Mat src_gray,src_gray1;
    
  //distance = 0.001*cv_ptr->image.at<u_int16_t>(xr, yr);    */
  
  //IplImage* imgMsgToCv(sensor_msgs::Image::ConstPtr image_message,string cv_encoding="passthrough")
  //color_image=msg;  //sensor_msgs::Image type
  ROS_INFO_STREAM("linia 52 "<< typeid(color_image).name());
  //Convert a sensor_msgs::Image message to an OpenCV IplImage.

  
  
  //sensor_msgs::CvBridge bridge;
  //img=msg;
}
//callback function 
void callback_depth_image(const sensor_msgs::Image& msg){
  //ROS_INFO_STREAM("arriba la depth imatge ");
  depth_image=msg;
  //sensor_msgs::CvBridge bridge;
  //img=msg;
  
}

void callback_BoundingBoxes(const distance::BoundingBoxes::ConstPtr &msg){
  
  std::vector<distance::BoundingBox> bboxes = msg->bounding_boxes;

  ROS_INFO_STREAM("dsfgbdfgdf  "<<bboxes.size());
  for  (int i = 0; i< bboxes.size(); i++){
    int xmin=bboxes[i].xmin;
    int xmax=bboxes[i].xmax;
    int ymin=bboxes[i].ymin;
    int ymax=bboxes[i].ymax;
    std::string classe = bboxes[i].Class;
    ROS_INFO_STREAM(" i "<<i<<" class "<< classe << "x's "<< xmin <<" - "<<xmax);

    position_extraction(xmin, ymin, xmax, ymax);
  }  

}

int main(int argc, char **argv) {
  // Initialize the ROS system and become a node.
  ros::init(argc, argv, "distancies_node");
  ros::NodeHandle nh;

  // Create a publisher object.
  ros::Publisher pub = nh.advertise<sensor_msgs::Image>("soc_igual", 100);
  //ROS_INFO_STREAM("pub created");
  // Create a subscriber object.
  ros::Subscriber im_sub = nh.subscribe("/camera/color/image_raw", 100,&callback_image);

  ros::Subscriber d_im_sub = nh.subscribe("/camera/depth/image_rect_raw", 100,&callback_depth_image);
  
  ros::Subscriber boxes_sub = nh.subscribe("/yolov5/BoundingBoxes", 100,&callback_BoundingBoxes);

  

  // Seed the random number generator.
  //srand(time(0));

  // Loop at 2Hz until the node is shut down.
  ros::Rate rate(2);
  while(ros::ok()) {
    // Create and fill in the message.  The other four
    // fields, which are ignored by turtlesim, default to 0.
    sensor_msgs::Image img;
    
    pub.publish(color_image);
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
