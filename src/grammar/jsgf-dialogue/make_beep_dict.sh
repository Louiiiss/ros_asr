#!/bin/bash

# Copyright 2012 Vassil Panayotov
# Apache 2.0

locdata=$1
vocab=$2
beep_dict=../beep
dict_out=$1/dict
tools=../mit_g2p_tools
#tools=tools
echo "=== Preparing the dictionary ..."



if [ ! -f $beep_dict/beep/beep-1.0 ]; then
  echo "--- Downloading Beep dictionary ..."
  mkdir -p $beep_dict 
  cd $beep_dict
  wget http://staffwww.dcs.shef.ac.uk/people/S.Fernando/beep.tar.gz || exit 1;
  tar xvzf beep.tar.gz || exit 1
  
fi


echo "--- Striping stress and pronunciation variant markers from cmudict ..."
perl $beep_dict/beep/scripts/make_baseform.pl \
  $beep_dict/beep/beep-1.0 /dev/stdout |\
  sed -e 's:^\([^\s(]\+\)([0-9]\+)\(\s\+\)\(.*\):\1\2\3:' > $dict_out/beep-plain.txt

echo "--- Searching for OOV words ..."
gawk 'NR==FNR{words[$1]; next;} !($1 in words)' \
  $dict_out/beep-plain.txt $locdata/$vocab |\
  egrep -v '<.?s>' > $dict_out/vocab-oov.txt

gawk 'NR==FNR{words[$1]; next;} ($1 in words)' \
  $locdata/$vocab $dict_out/beep-plain.txt |\
  egrep -v '<.?s>' > $dict_out/lexicon-iv.txt

wc -l $dict_out/vocab-oov.txt
wc -l $dict_out/lexicon-iv.txt

pyver=`python --version 2>&1 | sed -e 's:.*\([2-3]\.[0-9]\+\).*:\1:g'`
if [ ! -f $tools/g2p/lib/python${pyver}/site-packages/g2p.py ]; then
  echo "--- Downloading Sequitur G2P ..."
  echo "NOTE: it assumes that you have Python, NumPy and SWIG installed on your system!"
 # wget -P $tools http://www-i6.informatik.rwth-aachen.de/web/Software/g2p-r1668.tar.gz
  wget -P $tools http://staffwww.dcs.shef.ac.uk/~samf/g2p-r1668.tar.gz
   
 tar xf $tools/g2p-r1668.tar.gz -C $tools
  cd $tools/g2p
  echo '#include <cstdio>' >> Utility.hh # won't compile on my system w/o this "patch"
  python setup.py install --prefix=.
  cd ../..
  if [ ! -f $tools/g2p/lib/python${pyver}/site-packages/g2p.py ]; then
    echo "Sequitur G2P is not found - installation failed?"
    exit 1
  fi
fi

if [ ! -f ../conf/g2p_model ]; then
  echo "--- Downloading a pre-trained Sequitur G2P model ..."
  wget http://sourceforge.net/projects/kaldi/files/sequitur-model4 -O conf/g2p_model
  if [ ! -f conf/g2p_model ]; then
    echo "Failed to download the g2p model!"
    exit 1
  fi
fi

echo "--- Preparing pronunciations for OOV words ..."
export PYTHONPATH=`readlink -f $tools/g2p/lib/python*/site-packages`
python $tools/g2p/g2p.py \
  --model=../conf/g2p_model --apply $dict_out/vocab-oov.txt > $dict_out/lexicon-oov.txt

cat $dict_out/lexicon-oov.txt $dict_out/lexicon-iv.txt |\
  sort > $dict_out/lexicon.txt

echo "--- Prepare phone lists ..."
echo SIL > $dict_out/silence_phones.txt
echo NOISE >> $dict_out/silence_phones.txt
#echo BREATH >> $dict_out/silence_phones.txt
#echo COUGH >> $dict_out/silence_phones.txt
#echo GASP >> $dict_out/silence_phones.txt
#echo LAUGH >> $dict_out/silence_phones.txt
echo SIL > $dict_out/optional_silence.txt

grep -v -w sil $dict_out/lexicon.txt | \
  awk '{for(n=2;n<=NF;n++) { p[$n]=1; }} END{for(x in p) {print x}}' |\
  sort > $dict_out/nonsilence_phones.txt

#echo "--- Adding SIL and NOISE, BREATH, COUGH, GASP, LAUGH to the lexicon ..."
echo "Adding SIL"

echo -e "!SIL\tSIL" >> $dict_out/lexicon.txt
echo -e "!NOISE\tNOISE" >> $dict_out/lexicon.txt
#echo -e "!BREATH\tBREATH" >> $dict_out/lexicon.txt
#echo -e "!COUGH\tCOUGH" >> $dict_out/lexicon.txt
#echo -e "!GASP\tGASP" >> $dict_out/lexicon.txt
#echo -e "!LAUGH\tLAUGH" >> $dict_out/lexicon.txt

# Some downstream scripts expect this file exists, even if empty
touch $dict_out/extra_questions.txt

echo "*** Dictionary preparation finished!"
