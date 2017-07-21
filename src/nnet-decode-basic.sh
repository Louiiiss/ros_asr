#!/bin/bash

# Copyright 2014  Johns Hopkins University (Author: Daniel Povey)
# Apache 2.0

# Begin configuration section.  
stage=0
nj=4
cmd=run.pl
max_active=7000
threaded=false
modify_ivector_config=false #  only relevant to threaded decoder.
beam=15.0
lattice_beam=6.0
acwt=0.1   # note: only really affects adaptation and pruning (scoring is on
           # lattices).
per_utt=false
online=true  # only relevant to non-threaded decoder.
do_endpointing=true
do_speex_compressing=false
scoring_opts=
skip_scoring=false
silence_weight=1.0
# set this to a value less than 1 (e.g. 0) to enable silence weighting.
max_state_duration=40 # This only has an effect if you are doing silence
  # weighting.  This default is probably reasonable.  transition-ids repeated
  # more than this many times in an alignment are treated as silence.
iter=final
# End configuration section.

#echo "$0 $@"  # Print the command line for logging

#[ -f ./path.sh ] && . ./path.sh; # source the path.
. utils/parse_options.sh || exit 1;

#echo $#

if [ $# != 8 ]; then
   echo "Usage: $0 [options] <graph-dir> <data-dir> <decode-dir>"
   echo "... where <decode-dir> is assumed to be a sub-directory of the directory"
   echo " where the models are, as prepared by steps/online/nnet2/prepare_online_decoding.sh"
   echo "e.g.: $0 exp/tri3b/graph data/test exp/tri3b_online/decode/"
   echo ""
   echo ""
   echo "main options (for others, see top of script file)"
   echo "  --config <config-file>                           # config containing options"
   echo "  --nj <nj>                                        # number of parallel jobs"
   echo "  --cmd (utils/run.pl|utils/queue.pl <queue opts>) # how to run jobs."
   echo "  --acwt <float>                                   # acoustic scale used for lattice generation "
   echo "  --per-utt <true|false>                           # If true, decode per utterance without"
   echo "                                                   # carrying forward adaptation info from previous"
   echo "                                                   # utterances of each speaker.  Default: false"
   echo "  --online <true|false>                            # Set this to false if you don't really care about"
   echo "                                                   # simulating online decoding and just want the best"
   echo "                                                   # results.  This will use all the data within each"
   echo "                                                   # utterance (plus any previous utterance, if not in"
   echo "                                                   # per-utterance mode) to estimate the iVectors."
   echo "  --scoring-opts <string>                          # options to local/score.sh"
   echo "  --iter <iter>                                    # Iteration of model to decode; default is final."
   exit 1;
fi


#graphdir=$1
#data=$2
#dir=$3
#srcdir=`dirname $dir`; # The model directory is one level up from decoding directory.
#sdata=$data/split$nj;

graphdir=$1
dir=$2
source=$3
output_path=$4
keyphrase=$5
record_wav=$6
read_pipe=$7
should_listen=$8
decoder=nnet
srcdir=$dir

for f in $srcdir/conf/online_nnet2_decoding.conf $srcdir/${iter}.mdl; do
  if [ ! -f $f ]; then
    echo "$0: no such file $f"
    echo "Make sure that the online_dir is suitable for nnet decoding"
    exit 1;
  fi
done

if [ "$silence_weight" != "1.0" ]; then
  silphones=$(cat $graphdir/phones/silence.csl) || exit 1
  silence_weighting_opts="--ivector-silence-weighting.max-state-duration=$max_state_duration --ivector-silence-weighting.silence_phones=$silphones --ivector-silence-weighting.silence-weight=$silence_weight"
else
  silence_weighting_opts=
fi

#spk2utt_rspecifier="ark:$sdata/JOB/spk2utt"
wav_rspecifier="scp:wav.scp"
lat_wspecifier="ark:|gzip -c > $dir/lat.1.gz" 
utt=testing

#decoder=online2-wav-nnet2-latgen-faster
parallel_opts=
opts="--online=$online"

args=($utt $source $output_path $keyphrase $record_wav $read_pipe $should_listen $decoder);

#echo "Sending ${args[@]}"



#new-src/new-sf-controller $opts $silence_weighting_opts --do-endpointing=$do_endpointing \
#     --config=$srcdir/conf/online_nnet2_decoding.conf \
#     --max-active=$max_active --beam=$beam --lattice-beam=$lattice_beam \
#     --acoustic-scale=$acwt --word-symbol-table=$graphdir/words.txt \
#     $srcdir/${iter}.mdl $graphdir/HCLG.fst "$wav_rspecifier" \
#    "$lat_wspecifier" ${args[@]} || exit 1;

#basic_src/new-sf-controller $opts $silence_weighting_opts --do-endpointing=$do_endpointing \
#     --config=$srcdir/conf/online_nnet2_decoding.conf \
#     --max-active=$max_active --beam=$beam --lattice-beam=$lattice_beam \
#     --acoustic-scale=$acwt --word-symbol-table=$graphdir/words.txt \
#     $srcdir/${iter}.mdl $graphdir/HCLG.fst "$wav_rspecifier" \
#    "$lat_wspecifier" ${args[@]} || exit 1;
    
basic_src/new-sf-controller  $silence_weighting_opts --do-endpointing=$do_endpointing \
     --config=$srcdir/conf/online_nnet2_decoding.conf \
     --max-active=$max_active --beam=$beam --lattice-beam=$lattice_beam \
     --acoustic-scale=$acwt --word-symbol-table=$graphdir/words.txt \
     $srcdir/${iter}.mdl $graphdir/HCLG.fst "$wav_rspecifier" \
    "$lat_wspecifier" ${args[@]} || exit 1;
