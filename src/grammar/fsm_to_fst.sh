#!/bin/bash

fst=$1
syms=$fst.syms

#my-fst $syms $fst > log$DATE.txt

cd compiled

echo "Making FST from FSM..."
fstcompile --isymbols=$syms --acceptor=true $fst.fsm $fst.fst
fstdeterminize $fst.fst $fst.fst
fstminimize $fst.fst $fst.fst
fstrmepsilon $fst.fst $fst.fst
fstprint --isymbols=$syms --acceptor=true $fst.fst > $fst.fst.txt
echo "Finished making FST from FSM..."
#fstdraw --isymbols=$syms --acceptor=true $fst.fst $fst.dot
#dot -Tps $fst.dot > $fst.ps
#ps2pdf $fst.ps
