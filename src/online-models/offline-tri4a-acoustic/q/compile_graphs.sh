#!/bin/bash
cd /home/ac1ssf/noisy-wsj-pf
. ./path.sh
( echo '#' Running on `hostname`
  echo '#' Started at `date`
  echo -n '# '; cat <<EOF
compile-train-graphs /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/tree /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/1.mdl /data/ac1ssf/langs/wsj-pfstar-with-eval/L.fst "ark:utils/sym2int.pl --map-oov 2 -f 2- /data/ac1ssf/langs/wsj-pfstar-with-eval/words.txt < /data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/text |" "ark:|gzip -c >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/fsts.$SGE_TASK_ID.gz" 
EOF
) >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/compile_graphs.$SGE_TASK_ID.log
time1=`date +"%s"`
 ( compile-train-graphs /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/tree /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/1.mdl /data/ac1ssf/langs/wsj-pfstar-with-eval/L.fst "ark:utils/sym2int.pl --map-oov 2 -f 2- /data/ac1ssf/langs/wsj-pfstar-with-eval/words.txt < /data/ac1ssf/noisy-wsj-pf/data/train-wsj-pf-inc-noise/split32/$SGE_TASK_ID/text |" "ark:|gzip -c >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/fsts.$SGE_TASK_ID.gz"  ) 2>>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/compile_graphs.$SGE_TASK_ID.log >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/compile_graphs.$SGE_TASK_ID.log
ret=$?
time2=`date +"%s"`
echo '#' Accounting: time=$(($time2-$time1)) threads=1 >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/compile_graphs.$SGE_TASK_ID.log
echo '#' Finished at `date` with status $ret >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/compile_graphs.$SGE_TASK_ID.log
[ $ret -eq 137 ] && exit 100;
touch /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/done.83413.$SGE_TASK_ID
exit $[$ret ? 1 : 0]
## submitted with:
# qsub -S /bin/bash -v PATH -cwd -j y -o /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/compile_graphs.log -l rmem=6G  -t 1:32 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/compile_graphs.sh >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/compile_graphs.log 2>&1
