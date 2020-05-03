#include "ball_chaser/DriveToTarget.h"
#include "geometry_msgs/Twist.h"
#include "ros/ros.h"

ros::Publisher publisher;

/*
  Responds to service requests, for driving robot to certain direction.

  Can only set linear velocity along X-axis & angular velocity along
  Z-axis ( for rotation purpose )
*/
bool drivingHandler(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
  ROS_INFO("DriveToTarget received :: linearX: %f, angularZ: %f ", (float)req.linear_x, req.angular_z);

  geometry_msgs::Twist msg;
  msg.linear.x = (float)req.linear_x;
  msg.angular.z = (float)req.angular_z;

  publisher.publish(msg);

  res.msg_feedback = "Wheel velocity set to :: linearX: " + std::to_string(req.linear_x) +
                     ", angularZ: " + std::to_string(req.angular_z) + " ";
  ROS_INFO_STREAM(res.msg_feedback);

  return true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "drive_bot");

  ros::NodeHandle node;
  publisher = node.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
  ros::ServiceServer service = node.advertiseService("/ball_chaser/command_robot", drivingHandler);

  ros::spin();  // handle communication & block until termination signal received

  return 0;
}
