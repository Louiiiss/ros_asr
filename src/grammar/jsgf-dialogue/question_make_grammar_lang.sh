#!/bin/bash

tools=/data/ac1ssf/my-mit/tools
type=$2
cdir=$PWD
my_lang=$PWD/$type/tmp_lang_$1
#my_lang=/data/ac1ssf/langs/extended_lang


base=$1

if [ ! -d $my_lang ]; then
	mkdir $my_lang
fi

if [ ! -d $my_lang/dict ]; then
	mkdir $my_lang/dict
fi


python make_corpus.py $my_lang/corpus.txt $my_lang/vocab.txt $type/compiled/$base.fst.txt


#
./make_beep_dict.sh $my_lang vocab.txt || exit 1
#
cp ../tmp_lang/dict/nonsilence_phones.replace.txt $my_lang/dict/nonsilence_phones.txt || exit 1
##
#
#
rm $my_lang/G.fst
#
rm $my_lang/dict/lexiconp.txt
#
cd ..

utils/prepare_lang.sh --position-dependent-phones true \
  $my_lang/dict '!SIL' $my_lang/tmp $my_lang 

  cd $cdir
 #  
fstcompile  --acceptor=true --isymbols=$my_lang/words.txt --osymbols=$my_lang/words.txt \
--keep_isymbols=false --keep_osymbols=false $type/compiled/$base.fst.txt $my_lang/G.fst || exit 1

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