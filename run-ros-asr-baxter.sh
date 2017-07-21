#!/bin/bash

cd src
#gnome-terminal -e rosrun ros_asr kaldi_relay
#gnome-terminal -e 'sh -c (../bin/kaldi_relay)'
roslaunch ros_asr end_effector_control.launch

