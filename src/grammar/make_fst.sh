#!/bin/bash

tools=/data/ac1ssf/my-mit/tools


#my_lang=/data/ac1ssf/langs/extended_lang

base=$1
lang=$2

fstcompile  --acceptor=true --isymbols=$lang/words.txt --osymbols=$lang/words.txt \
--keep_isymbols=false --keep_osymbols=false compiled/$base.fst.txt $lang/G.fst || exit 1

#  fstrmepsilon > $my_lang/G.fst
#fstisstochastic $my_lang/G.fst 
#
#cat $my_lang/lm.arpa | \
#  utils/find_arpa_oovs.pl $my_lang/words.txt > $my_lang/tmp/oovs.txt
####
#cat $my_lang/lm.arpa | \
#  grep -v '<s> <s>' | \
#  grep -v '</s> <s>' | \
#  grep -v '</s> </s>' | \
#  arpa2fst - | fstprint | \
#  utils/remove_oovs.pl $my_lang/tmp/oovs.txt | \
#  utils/eps2disambig.pl | utils/s2eps.pl | fstcompile --isymbols=$my_lang/words.txt \
#    --osymbols=$my_lang/words.txt  --keep_isymbols=false --keep_osymbols=false | \
#  fstrmepsilon > $my_lang/G.fst
#fstisstochastic $my_lang/G.fst