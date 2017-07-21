#!/bin/bash
cd /home/ac1ssf/noisy-wsj-pf
. ./path.sh
( echo '#' Running on `hostname`
  echo '#' Started at `date`
  echo -n '# '; cat <<EOF
build-tree --verbose=1 --max-leaves=4200 --cluster-thresh=-1 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/treeacc /data/ac1ssf/langs/wsj-pfstar-with-eval/phones/roots.int /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/questions.qst /data/ac1ssf/langs/wsj-pfstar-with-eval/topo /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/tree 
EOF
) >/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/build_tree.log
time1=`date +"%s"`
 ( build-tree --verbose=1 --max-leaves=4200 --cluster-thresh=-1 /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/treeacc /data/ac1ssf/langs/wsj-pfstar-with-eval/phones/roots.int /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/questions.qst /data/ac1ssf/langs/wsj-pfstar-with-eval/topo /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/tree  ) 2>>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/build_tree.log >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/build_tree.log
ret=$?
time2=`date +"%s"`
echo '#' Accounting: time=$(($time2-$time1)) threads=1 >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/build_tree.log
echo '#' Finished at `date` with status $ret >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/log/build_tree.log
[ $ret -eq 137 ] && exit 100;
touch /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/done.81459
exit $[$ret ? 1 : 0]
## submitted with:
# qsub -S /bin/bash -v PATH -cwd -j y -o /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/build_tree.log -l rmem=6G   /data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/build_tree.sh >>/data/ac1ssf/noisy-wsj-pf/exp-noise/tri4a/q/build_tree.log 2>&1
