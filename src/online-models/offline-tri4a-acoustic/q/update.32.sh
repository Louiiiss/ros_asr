#!/bin/bash
cd /home/ac1ssf/noisy-wsj-pf
. ./path.sh
( echo '#' Running on `hostname`
  echo '#' Started at `date`
  echo -n '# '; cat <<EOF
gmm-est --power=0.2 --write-occs=/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/33.occs --mix-up=40000 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/32.mdl "gmm-sum-accs - /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/32.*.acc |" /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/33.mdl 
EOF
) >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/update.32.log
time1=`date +"%s"`
 ( gmm-est --power=0.2 --write-occs=/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/33.occs --mix-up=40000 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/32.mdl "gmm-sum-accs - /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/32.*.acc |" /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/33.mdl  ) 2>>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/update.32.log >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/update.32.log
ret=$?
time2=`date +"%s"`
echo '#' Accounting: time=$(($time2-$time1)) threads=1 >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/update.32.log
echo '#' Finished at `date` with status $ret >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/update.32.log
[ $ret -eq 137 ] && exit 100;
touch /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/done.78406
exit $[$ret ? 1 : 0]
## submitted with:
# qsub -S /bin/bash -v PATH -cwd -j y -o /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/update.32.log -l rmem=6G   /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/update.32.sh >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/update.32.log 2>&1