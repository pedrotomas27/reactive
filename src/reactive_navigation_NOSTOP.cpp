#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include <random>


double obstacle_distance;
bool robot_stopped;
bool rotate_right;

void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

	if (!robot_stopped){
		ROS_INFO("Received a LaserScan with %i samples", (int) msg->ranges.size());
		
		obstacle_distance=*std::min_element (msg->ranges.begin(), msg->ranges.end());
		ROS_INFO("minimum distance to obstacle: %f", obstacle_distance);
	}
}

int main(int argc,char **argv){
	

	

	ros::init(argc, argv, "reactive_navigation");
	ros::NodeHandle n;
	
	ros::Publisher cmd_vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 100);
	ros::Subscriber laser_sub=n.subscribe("base_scan", 100, laserCallback);
	
	ros::Rate loop_rate(10);
	geometry_msgs::Twist cmd_vel_msg;
	robot_stopped = true;
	obstacle_distance = 1.0;
	rotate_right = true;
	double Var = 0;
	while (ros::ok()){
		if(obstacle_distance>0.3){
		   cmd_vel_msg.linear.x = 1.0;
		   cmd_vel_msg.angular.z= 0.0;
		   
		    if(robot_stopped){
		   	ROS_INFO("Moving Foward");
		   	robot_stopped = false;
		   	}

			
		}else{
			
			double min_value = -1.0;  
    		double max_value = 1.0; 
			std::random_device rd;
    		std::mt19937 gen(rd());
    	    std::uniform_real_distribution<double> dis(min_value, max_value);

			double random_value = dis(gen);
		   	for (int i = 0; i < 20; ++i) {
				
        		cmd_vel_msg.linear.x = 0.0;
        		cmd_vel_msg.angular.z = random_value;
        
        		cmd_vel_pub.publish(cmd_vel_msg);
        		ros::Duration(0.1).sleep(); 
				

				
			}
    		
			Var = Var +1;

			if (Var >5){
				cmd_vel_msg.linear.x = 0.0;
        		cmd_vel_msg.angular.z = M_PI;
        
        		cmd_vel_pub.publish(cmd_vel_msg);
        		ros::Duration(0.1).sleep(); 

			}

    		

		   
		   if(robot_stopped){
		      ROS_INFO("Stopping");
		      robot_stopped = true;
		   }
		   
		}
		
		cmd_vel_pub.publish(cmd_vel_msg);
		
		ros::spinOnce();
		
		loop_rate.sleep();
	}
    return 0;
}
		
