#!/usr/bin/python

from operator import pos
import roslib
import rospy
import tf
#from darknet_ros_msgs.msg import BoundingBoxes,ObjectCount
from sensor_msgs.msg import Image,CameraInfo
from distance.msg import BoundingBox, BoundingBoxes

from geometry_msgs.msg import PointStamped
from visualization_msgs.msg import MarkerArray,Marker
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
        self.height=720
        self.width=1280
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
        	
            array = MarkerArray()
            id_m=0
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
                        #rospy.loginfo("Found: "+obj+" Pose: "+str(pose_tf))
                        self.br.sendTransform((pose_tf[0],pose_tf[1],pose_tf[2]), (0.0, 0.0, 0.0, 1.0),rospy.Time.now(),box.Class, self.camera_link)
                        #Marker
                        x=pose_tf[0]
                        y=pose_tf[1]
                        z=pose_tf[2]
                        marker=Marker()
                        marker.header.frame_id = 'camera_link'
                        marker.header.stamp = rospy.Time.now()
                        # set shape, Arrow: 0; Cube: 1 ; Sphere: 2 ; Cylinder: 3
                        marker.type = 2
                        marker.id=id_m
                        id_m=id_m+1
                        marker.text=obj
                        scale=0.06
                        marker.scale.x =scale
                        marker.scale.y =scale
                        marker.scale.z =scale
                        
                        
                        # Set the color
                        marker.color.r = 0.0
                        marker.color.b = 0.0
                        marker.color.g = 1.0
                        marker.color.a = 1.0
                        
                        marker.pose.position.x=x
                        marker.pose.position.y=y
                        marker.pose.position.z=z
                        
                        marker.pose.orientation.x = 0.0
                        marker.pose.orientation.y = 0.0
                        marker.pose.orientation.z = 0.0
                        marker.pose.orientation.w = 1.0
                        array.markers.append(marker)
                        publisher_marker = rospy.Publisher('/marker', Marker, queue_size=15)
                        publisher_marker.publish(marker)        
            
            publisher_obj = rospy.Publisher('/obj_info', MarkerArray, queue_size=10)
            publisher_obj.publish(array)


	                    
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
