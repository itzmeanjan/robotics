#include "ball_chaser/DriveToTarget.h"
#include "ros/ros.h"
#include "sensor_msgs/Image.h"

ros::ServiceClient client;

/*
  Drives robot to specified direction, by sending service request to
  `/ball_chaser/command_robot` service
*/
void driveRobot(float linX, float angZ)
{
  ball_chaser::DriveToTarget driveToTarget;

  driveToTarget.request.linear_x = linX;
  driveToTarget.request.angular_z = angZ;

  if (client.exists())
  {
    client.call(driveToTarget);
  }
}

/*
  Checks captured image for existence of white pixel & if found, drives robot towards it,
  otherwise sets robot velocities to 0 ( for stopping it )
*/
void handleCameraImage(sensor_msgs::Image img)
{
  ROS_INFO("[ %d ] Received new image from camera", img.header.seq);

  uint8_t whiteBall = 255;  // pixel intensity of white colored pixel
  int ballPosition = -1;    // initially assuming ball can't be seen by camera

  for (int i = 0; i < img.height * img.step; i++)
  {
    if (img.data[i] == whiteBall)  // found white pixel
    {
      ballPosition = i;
      break;
    }
  }

  if (ballPosition == -1)  // after traversing whole image, if no sign of white pixel, stopping robot
  {
    return driveRobot(0.0, 0.0);
  }

  ballPosition %= img.step;  // obatined column index, because that can give us some hint about location of white pixel
                             // along width of camera frame
  int segmentWidth = img.step / 3;  // splitting width of image in three ( almost equal ) parts

  if (ballPosition < segmentWidth)  // present in left part, take a left turn
  {
    driveRobot(0.0, -0.3);
  }
  else if (ballPosition < 2 * segmentWidth)  // present in middle portion, so move forward
  {
    driveRobot(0.3, 0.0);
  }
  else  // present on right part, so take a slight right turn
  {
    driveRobot(0.0, 0.3);
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "process_image");

  ros::NodeHandle node;
  client = node.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
  ros::Subscriber subs = node.subscribe<sensor_msgs::Image>("/camera/rgb/image_raw", 10, handleCameraImage);

  ros::spin();  // handle communication & block until termination signal received

  return 0;
}
