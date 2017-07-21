#!/bin/bash


lang=$1
model_dir=$2
graph=$3


utils/mkgraph.sh $lang $model_dir $graph
