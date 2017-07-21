#!/bin/bash

. path.sh || exit 1;

export PATH=$PWD/utils/:$KALDI_ROOT/tools/openfst/bin:$KALDI_ROOT/src/fstbin:$KALDI_ROOT/src/bin:$PATH