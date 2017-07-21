#!/bin/bash
cd /home/ac1ssf/noisy-wsj-pf
. ./path.sh
( echo '#' Running on `hostname`
  echo '#' Started at `date`
  echo -n '# '; cat <<EOF
gmm-align-compiled --transition-scale=1.0 --acoustic-scale=0.1 --self-loop-scale=0.1 --beam=10 --retry-beam=40 --careful=false "gmm-boost-silence --boost=1.0 1 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/20.mdl - |" "ark:gunzip -c /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/fsts.$SGE_TASK_ID.gz|" "ark,s,cs:apply-cmvn  --utt2spk=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/utt2spk scp:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/cmvn.scp scp:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/feats.scp ark:- | splice-feats --left-context=3 --right-context=3 ark:- ark:- | transform-feats /data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/final.mat ark:- ark:- | transform-feats --utt2spk=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/utt2spk ark:/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/trans.$SGE_TASK_ID ark:- ark:- |" "ark:|gzip -c >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/ali.$SGE_TASK_ID.gz" 
EOF
) >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/align.20.$SGE_TASK_ID.log
time1=`date +"%s"`
 ( gmm-align-compiled --transition-scale=1.0 --acoustic-scale=0.1 --self-loop-scale=0.1 --beam=10 --retry-beam=40 --careful=false "gmm-boost-silence --boost=1.0 1 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/20.mdl - |" "ark:gunzip -c /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/fsts.$SGE_TASK_ID.gz|" "ark,s,cs:apply-cmvn  --utt2spk=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/utt2spk scp:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/cmvn.scp scp:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/feats.scp ark:- | splice-feats --left-context=3 --right-context=3 ark:- ark:- | transform-feats /data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/final.mat ark:- ark:- | transform-feats --utt2spk=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/utt2spk ark:/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/trans.$SGE_TASK_ID ark:- ark:- |" "ark:|gzip -c >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/ali.$SGE_TASK_ID.gz"  ) 2>>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/align.20.$SGE_TASK_ID.log >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/align.20.$SGE_TASK_ID.log
ret=$?
time2=`date +"%s"`
echo '#' Accounting: time=$(($time2-$time1)) threads=1 >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/align.20.$SGE_TASK_ID.log
echo '#' Finished at `date` with status $ret >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/align.20.$SGE_TASK_ID.log
[ $ret -eq 137 ] && exit 100;
touch /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/done.118841.$SGE_TASK_ID
exit $[$ret ? 1 : 0]
## submitted with:
# qsub -S /bin/bash -v PATH -cwd -j y -o /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/align.20.log -l rmem=6G  -t 1:32 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/align.20.sh >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/align.20.log 2>&1
