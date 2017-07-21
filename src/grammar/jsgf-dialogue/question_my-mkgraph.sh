#!/bin/bash

i=$1
model_dir=$2
graph=$3
type=$4

../utils/mkgraph.sh $type/tmp_lang_$i $model_dir $graph
