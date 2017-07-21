#!/bin/bash
cd /home/ac1ssf/noisy-wsj-pf
. ./path.sh
( echo '#' Running on `hostname`
  echo '#' Started at `date`
  echo -n '# '; cat <<EOF
gmm-est --power=0.2 --remove-low-count-gaussians=false /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/35.mdl "gmm-sum-accs - /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/35.*.acc|" /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/35.alimdl 
EOF
) >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/est_alimdl.log
time1=`date +"%s"`
 ( gmm-est --power=0.2 --remove-low-count-gaussians=false /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/35.mdl "gmm-sum-accs - /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/35.*.acc|" /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/35.alimdl  ) 2>>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/est_alimdl.log >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/est_alimdl.log
ret=$?
time2=`date +"%s"`
echo '#' Accounting: time=$(($time2-$time1)) threads=1 >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/est_alimdl.log
echo '#' Finished at `date` with status $ret >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/est_alimdl.log
[ $ret -eq 137 ] && exit 100;
touch /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/done.87064
exit $[$ret ? 1 : 0]
## submitted with:
# qsub -S /bin/bash -v PATH -cwd -j y -o /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/est_alimdl.log -l rmem=6G   /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/est_alimdl.sh >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/est_alimdl.log 2>&1
