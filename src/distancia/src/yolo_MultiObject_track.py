#!/usr/bin/python

from operator import pos
import roslib
import rospy
import tf
#from darknet_ros_msgs.msg import BoundingBoxes,ObjectCount
from sensor_msgs.msg import Image,CameraInfo
from distance.msg import BoundingBox, BoundingBoxes
from geometry_msgs.msg import PointStamped
import numpy as np
import time
import pyrealsense2
from cv_bridge import CvBridge, CvBridgeError


class MultiObject_Tracker:
    def __init__(self,obejcts_to_track):
        self.objects_to_track = obejcts_to_track
        self.camera_link = "camera_link"
        self.camera_info = "/camera/color/camera_info"
        self.depth_img_topic = "/camera/aligned_depth_to_color/image_raw"
        self.height=480
        self.width=640
        self.depth_img = np.zeros((self.height,self.width))
        self.br = tf.TransformBroadcaster()
        self.object_count = 0
        self.is_objects_found = False


    #callback functions:
    
    def bouding_boxes_callback(self,msg):
        self.bounding_boxes = msg.bounding_boxes
        #print(self.bounding_boxes)
        self.is_objects_found = True


    def cam_info_callback(self,msg):
        self.cam_info = msg
        self.height = self.cam_info.height
        self.width = self.cam_info.width
        #print("h i w de la imatge ",self.height, ' ', self.width)    #720 1280
    
    def depth_img_callback(self,msg):
        bridge = CvBridge()
        try:
            im = bridge.imgmsg_to_cv2(msg, "passthrough")
        except CvBridgeError as e:
            print(e)
        self.depth_img = im

    def start_subscribers(self):
        rospy.Subscriber("/yolov5/BoundingBoxes", BoundingBoxes, self.bouding_boxes_callback) 
        #rospy.Subscriber("/darknet_ros/found_object", ObjectCount, self.count_objects_callback) 
        rospy.Subscriber(self.camera_info, CameraInfo, self.cam_info_callback) 
        rospy.Subscriber(self.depth_img_topic, Image, self.depth_img_callback)
    
    def objects_tf_send(self):
        if self.is_objects_found == True:
        	
            self.is_objects_found = False
            for box in self.bounding_boxes:
                for obj in self.objects_to_track:
                    if obj == box.Class:
                        xmin = box.xmin
                        ymin = box.ymin
                        xmax = box.xmax
                        ymax = box.ymax
                        rect = [xmin,ymin,xmax,ymax]
                        x = rect[2] - (rect[2]-rect[0])/2
                        y = rect[3] - (rect[3]-rect[1])/2
                        loc = [int(x),int(y)]
                        #print(loc)
                        d = self.depth_img[loc[1]][loc[0]]
                        pose = self.convert_depth_to_phys_coord_using_realsense(loc[0],loc[1],d,self.cam_info)
                        pose_tf = np.array([pose[2]/1000, -pose[0]/1000, -pose[1]/1000])
                        rospy.loginfo("Found: "+obj+" Pose: "+str(pose_tf))
                        self.br.sendTransform((pose_tf[0],pose_tf[1],pose_tf[2]), (0.0, 0.0, 0.0, 1.0),rospy.Time.now(),box.Class, self.camera_link)
                        #publish pointer
                        self.proj_point(pose_tf)
                        
                                                
                           

    def proj_point(self,pose):
        x=pose[0]
        y=pose[1]
        z=pose[2]
        #time.sleep(1)
        point_msg = PointStamped()
        point_msg.header.stamp = rospy.Time.now()
        point_msg.header.frame_id = 'camera_link'
        point_msg.point.x =x #point[0]
        point_msg.point.y =y # point[1]
        point_msg.point.z =z # point[2]
        print(' x i y i z ',x,' ',y ,' ', z)
        publisher = rospy.Publisher('/cep_found', PointStamped, queue_size=10)
        publisher.publish(point_msg)
    
    def convert_depth_to_phys_coord_using_realsense(self,x, y, depth, cameraInfo):  
        _intrinsics = pyrealsense2.intrinsics()
        _intrinsics.width = cameraInfo.width
        _intrinsics.height = cameraInfo.height
        _intrinsics.ppx = cameraInfo.K[2]
        _intrinsics.ppy = cameraInfo.K[5]
        _intrinsics.fx = cameraInfo.K[0]
        _intrinsics.fy = cameraInfo.K[4]
        #_intrinsics.model = cameraInfo.distortion_model
        _intrinsics.model  = pyrealsense2.distortion.none
        _intrinsics.coeffs = [i for i in cameraInfo.D]  
        result = pyrealsense2.rs2_deproject_pixel_to_point(_intrinsics, [x, y], depth)  
        #result[0]: right, result[1]: down, result[2]: forward
        return result



if __name__=='__main__':
    rospy.init_node('Pointer')
    rate = rospy.Rate(10.0)
    tracker = MultiObject_Tracker(obejcts_to_track = ['Trunks','Canopy','Person'])
    tracker.start_subscribers()
    time.sleep(3)
    while not rospy.is_shutdown():
        tracker.objects_tf_send()
        # rate.sleep()
    rospy.spin()
