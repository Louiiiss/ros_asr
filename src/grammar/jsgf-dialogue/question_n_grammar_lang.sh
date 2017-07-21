#!/bin/bash

tools=/data/ac1ssf/my-mit/tools
type=$1
no_utts=$2
cdir=$PWD
glob_lang=$PWD/$type/glob_lang
#my_lang=/data/ac1ssf/langs/extended_lang


if [ ! -d $glob_lang ]; then
	mkdir $glob_lang
fi

if [ ! -d $glob_lang/dict ]; then
	mkdir $glob_lang/dict
fi

python n_corpus.py $glob_lang/corpus.txt $glob_lang/vocab.txt $type $no_utts 
#
./make_beep_dict.sh $glob_lang vocab.txt || exit 1
##
cp ../tmp_lang/dict/nonsilence_phones.replace.txt $glob_lang/dict/nonsilence_phones.txt || exit 1
###
##
##
rm $glob_lang/G.fst
##
rm $glob_lang/dict/lexiconp.txt
##

#
#utils/prepare_lang.sh --position-dependent-phones true \
#  $glob_lang/dict '!SIL' $glob_lang/tmp $glob_lang 
#

# #
#

let range=no_utts-1
for base in `seq 0 $range`;
do
my_lang=$PWD/$type/tmp_lang_$base
cd ..
utils/prepare_lang.sh --position-dependent-phones true \
  $glob_lang/dict '!SIL' $my_lang/tmp $my_lang 
 cd $cdir
 rm $my_lang/G.fst
fstcompile  --acceptor=true --isymbols=$my_lang/words.txt --osymbols=$my_lang/words.txt \
--keep_isymbols=false --keep_osymbols=false $type/compiled/$base.fst.txt $my_lang/G.fst || exit 1
done

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