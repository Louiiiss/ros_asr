#!/bin/bash

. path.sh || exit 1;

#touch path.test
if [ $KALDI_ROOT = "/path/to/kaldi" ]; then
	echo "Must set KALDI_ROOT in path.sh to point to Kaldi install location";
	exit 0;
fi

if [  -f $KALDI_ROOT/src/kaldi.mk ]; then
	cp $KALDI_ROOT/src/kaldi.mk .
else
	echo "KALDI_ROOT/src/kaldi.mk not found, please set path correctly.";
	exit 0;
fi

echo "KALDI_ROOT=$KALDI_ROOT" > path.mk

sed -i.bak 's/-Wall -I\.\./-Wall -I$(KALDI_ROOT)\/src/g' kaldi.mk

if [ $USER = "samf" ] || [ $USER = "ac1ssf" ]; then
	svn changelist ignore-on-commit path.sh
fi

#export KALDI_ROOT=/usr/local/extras/kaldi/kaldi-Nov15/kaldi-trunk

#export PATH=$PWD/utils #:$KALDI_ROOT/src/bin:$KALDI_ROOT/tools/openfst/bin:$KALDI_ROOT/src/fstbin/:$KALDI_ROOT/src/gmmbin/:$KALDI_ROOT/src/featbin/:$KALDI_ROOT/src/lm/:$KALDI_ROOT/src/sgmmbin/:$KALDI_ROOT/src/sgmm2bin/:$KALDI_ROOT/src/fgmmbin/:$KALDI_ROOT/src/latbin/:$KALDI_ROOT/src/nnetbin:$KALDI_ROOT/src/nnet2bin/:$KALDI_ROOT/src/kwsbin:$KALDI_ROOT/src/online2bin/:$KALDI_ROOT/src/my-online2bin/:$KALDI_ROOT/src/ivectorbin/:$PWD:$PATH





