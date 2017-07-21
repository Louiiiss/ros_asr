#!/bin/bash

#. config.sh

#interactive=false
read_pipe=$1
#echo "read_pipe $read_pipe"

#graph=models/healthy_graph
#graph=models/fisher_graph
#graph=models/spodiro_graph

#graph=models/tri4a-graph-wsj-pf-nnet
#graph=models/questions-worksheet-tri4a-nnet
graph=online-models/graph-commands-gmm
#graph=models/graph-spodiro_complete-gmm
#graph=models/graph-healthy_living_complete-gmm
#graph=models/graph-number_game-gmm

#graph=models/graph-wsj-pfstar-with-museum-gmm
#graph=models/graph_wsj_pf_museum

#graph=jsgf-dialogue/dialogue/graph-0
#online_dir=online-models/nnet_noisy_online
online_dir=online-models/nnet_noisy_online
#online_dir=models/nnet_a_online
#online_dir=online-models/fisher_wsj_nnet_online
#

#graph=/home/samf/sf-kaldi-asr/online-models/graph-miro-gmm
#graph=/home/samf/spodiro-asr/models/wsj_pepper_graph

#online_dir=/home/samf/spodiro-asr/online-models/pepper_nnet_online




#CHANGE THIS DIRECTORY TO THE LOCATION OF THIS FILE
cd /home/dcs/catkin_ws/src/ros_asr/src





start_listen=true
output_path=speech-out
record_wav=ON

keyphrase=hello_i_am_ready

source=pa
#source=wav
decoder=nnet


args=( $graph $online_dir $source $output_path $keyphrase $record_wav $read_pipe $start_listen);

date=`date +"%d_%m_%Y_%H_%M_%S"`
day=`date +"%d_%m_%Y"`

mkdir -p speech-out/$day/stdouts
mkdir -p speech-out/$day/speech
mkdir -p speech-out/$day/text
mkdir -p speech-out/$day/controls
mkdir -p speech-out/$day/chunks

full_path=speech-out/$day/stdouts/$date.txt

if [ "$decoder" = "nnet" ]; then
	echo "Doing nnet";
	./nnet-decode-basic.sh ${args[@]} 2>&1 | tee $full_path
elif [ "$decoder" = "gmm" ]; then
	echo "Doing gmm";
	./gmm-decode.sh ${args[@]}
else
	echo "Invalid decoder, must be gmm or nnet";
fi


