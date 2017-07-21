#!/bin/bash
cd /home/ac1ssf/noisy-wsj-pf
. ./path.sh
( echo '#' Running on `hostname`
  echo '#' Started at `date`
  echo -n '# '; cat <<EOF
convert-ali /data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/final.mdl /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/1.mdl /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/tree "ark:gunzip -c /data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/ali.$SGE_TASK_ID.gz|" "ark:|gzip -c >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/ali.$SGE_TASK_ID.gz" 
EOF
) >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/convert.$SGE_TASK_ID.log
time1=`date +"%s"`
 ( convert-ali /data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/final.mdl /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/1.mdl /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/tree "ark:gunzip -c /data/ac1ssf/noisy-wsj-pf/exp-noise/tri3b_ali/ali.$SGE_TASK_ID.gz|" "ark:|gzip -c >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/ali.$SGE_TASK_ID.gz"  ) 2>>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/convert.$SGE_TASK_ID.log >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/convert.$SGE_TASK_ID.log
ret=$?
time2=`date +"%s"`
echo '#' Accounting: time=$(($time2-$time1)) threads=1 >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/convert.$SGE_TASK_ID.log
echo '#' Finished at `date` with status $ret >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/convert.$SGE_TASK_ID.log
[ $ret -eq 137 ] && exit 100;
touch /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/done.83185.$SGE_TASK_ID
exit $[$ret ? 1 : 0]
## submitted with:
# qsub -S /bin/bash -v PATH -cwd -j y -o /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/convert.log -l rmem=6G  -t 1:32 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/convert.sh >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/convert.log 2>&1
