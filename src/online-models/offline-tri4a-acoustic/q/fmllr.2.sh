#!/bin/bash
cd /home/ac1ssf/noisy-wsj-pf
. ./path.sh
( echo '#' Running on `hostname`
  echo '#' Started at `date`
  echo -n '# '; cat <<EOF
ali-to-post "ark:gunzip -c /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/ali.$SGE_TASK_ID.gz|" ark:- | weight-silence-post 0.0 1:2:3:4:5:6:7:8:9:10 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/2.mdl ark:- ark:- | gmm-est-fmllr --fmllr-update-type=full --spk2utt=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/spk2utt /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/2.mdl "ark,s,cs:apply-cmvn  --utt2spk=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/utt2spk scp:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/cmvn.scp scp:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/feats.scp ark:- | splice-feats --left-context=3 --right-context=3 ark:- ark:- | transform-feats /data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/final.mat ark:- ark:- | transform-feats --utt2spk=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/utt2spk ark,s,cs:/data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/trans.$SGE_TASK_ID ark:- ark:- |" ark:- ark:/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/tmp_trans.$SGE_TASK_ID 
EOF
) >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/fmllr.2.$SGE_TASK_ID.log
time1=`date +"%s"`
 ( ali-to-post "ark:gunzip -c /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/ali.$SGE_TASK_ID.gz|" ark:- | weight-silence-post 0.0 1:2:3:4:5:6:7:8:9:10 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/2.mdl ark:- ark:- | gmm-est-fmllr --fmllr-update-type=full --spk2utt=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/spk2utt /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/2.mdl "ark,s,cs:apply-cmvn  --utt2spk=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/utt2spk scp:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/cmvn.scp scp:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/feats.scp ark:- | splice-feats --left-context=3 --right-context=3 ark:- ark:- | transform-feats /data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/final.mat ark:- ark:- | transform-feats --utt2spk=ark:/data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/utt2spk ark,s,cs:/data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/trans.$SGE_TASK_ID ark:- ark:- |" ark:- ark:/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/tmp_trans.$SGE_TASK_ID  ) 2>>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/fmllr.2.$SGE_TASK_ID.log >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/fmllr.2.$SGE_TASK_ID.log
ret=$?
time2=`date +"%s"`
echo '#' Accounting: time=$(($time2-$time1)) threads=1 >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/fmllr.2.$SGE_TASK_ID.log
echo '#' Finished at `date` with status $ret >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/fmllr.2.$SGE_TASK_ID.log
[ $ret -eq 137 ] && exit 100;
touch /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/done.87904.$SGE_TASK_ID
exit $[$ret ? 1 : 0]
## submitted with:
# qsub -S /bin/bash -v PATH -cwd -j y -o /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/fmllr.2.log -l rmem=6G  -t 1:32 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/fmllr.2.sh >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/fmllr.2.log 2>&1
