#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <sstream>
#include "pstream.h"
#include <string>

using namespace std;

int main(int argc, char **argv)
{

  // Setup
  ros::init(argc, argv, "kaldi_relay");
  ros::NodeHandle n;
  ros::Publisher command_pub = n.advertise<std_msgs::String>("kaldi_relay", 1000);
  ros::Rate loop_rate(10);

  cout << "Running Kaldi Relay" << endl;
  // Bool to take the correct output line
  bool cmdOpen = false;
  // Mark the preceeding line
  string marker = "SingleUtterance finished";


  while (ros::ok())
  {


    // Run shell script and capture output (so run Kaldi, return its console output)
    // Navigate to root then through to the asr shell script
    redi::ipstream proc("cd /;.//home/dcs/catkin_ws/src/ros_asr/src/asr.sh 0", redi::pstreams::pstdout | redi::pstreams::pstderr);
    string line;

    // Take each line and pass it through to the listener
    while (std::getline(proc.out(), line)){
      // Only take the relevant line
      if (cmdOpen==true){
        cmdOpen = false;
        std_msgs::String msg;

        msg.data = line;

        ROS_INFO("%s", msg.data.c_str());

        // publish command
        command_pub.publish(msg);

      }

      // Open bool
      if (line.find(marker) != std::string::npos) {
        cmdOpen = true;
      }

      ros::spinOnce();
    }
  }
  return 0;
}
