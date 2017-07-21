#!/bin/bash

. path_config.sh || exit 1;

name=commands
model_dir=../online-models/offline-tri4a-acoustic

#model_dir=../online-models/offline_wsj_pfstar_tri4b
#model_dir=../online-models/tri4a-nnet-acoustic
#graph=../online-models/graph-$name-nnet
graph=../online-models/graph-$name-gmm


#name=pioneer
cd grammar

./jsgf_to_fsm.sh $name
./fsm_to_fst.sh $name
./make_grammar_lang.sh $name
./my-mkgraph.sh tmp_lang $model_dir $graph



