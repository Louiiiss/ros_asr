all:
#KALDI_ROOT=/home/samf/new-kaldi/kaldi-trunk

include ../path.mk

EXTRA_CXXFLAGS += -Wno-sign-compare -I../portaudio/install/include/ -I../portaudio/qa/loopback/src

# need to add the Kaldi root variable to make this portable
# otherwise user will need to copy over the kaldi.mk from their Kaldi install

include ../kaldi.mk

EXTRA_LDLIBS = ../portaudio/qa/loopback/src/write_wav.o ../portaudio/install/lib/libportaudio.a
#endif

UNAME=$(shell uname)
ifeq ($(UNAME), Linux)
  ifneq ($(wildcard ../portaudio/install/include/pa_linux_alsa.h),)
    EXTRA_LDLIBS += -lasound -lrt
  else
    EXTRA_LDLIBS += -lrt
  endif
endif

LDFLAGS += $(CUDA_LDFLAGS)
LDLIBS += $(CUDA_LDLIBS)


LIBNAME = my-online

#BINFILES = fst-switcher
BINFILES = new-sf-controller

#BINFILES = my-nnet-controller #my-online-decoder my-online-controller
#
#
#OBJFILES = generic-decoder.o generic-listener.o \
#  my-online-controller.o \
#  my-online-data-source.o my-online-output.o my-online-config.o \
#  my-online-decoder.o my-online-listener.o online-audio-source.o \
#  my-nnet-listener.o my-nnet-decoder.o my-nnet-config.o \
#  my-nnet-controller.o

# ignoring sf-controller for now
 
OBJFILES = sf-generic-config.o  sf-decode-config.o sf-nnet-config-threaded.o \
		sf-data-source.o online-audio-source.o sf-generic-decoder.o \
		sf-nnet-threaded-decoder.o sf-nnet-decoder.o sf-output.o sf-gmm-decoder.o  \
		new-sf-controller.o RingBuffer.o

TESTFILES =

ADDLIBS = $(KALDI_ROOT)/src/online2/kaldi-online2.a $(KALDI_ROOT)/src/ivector/kaldi-ivector.a \
           $(KALDI_ROOT)/src/nnet2/kaldi-nnet2.a $(KALDI_ROOT)/src/lat/kaldi-lat.a \
          $(KALDI_ROOT)/src/decoder/kaldi-decoder.a  $(KALDI_ROOT)/src/cudamatrix/kaldi-cudamatrix.a \
          $(KALDI_ROOT)/src/feat/kaldi-feat.a $(KALDI_ROOT)/src/transform/kaldi-transform.a $(KALDI_ROOT)/src/gmm/kaldi-gmm.a \
          $(KALDI_ROOT)/src/thread/kaldi-thread.a $(KALDI_ROOT)/src/hmm/kaldi-hmm.a $(KALDI_ROOT)/src/tree/kaldi-tree.a \
          $(KALDI_ROOT)/src/matrix/kaldi-matrix.a $(KALDI_ROOT)/src/fstext/kaldi-fstext.a \
          $(KALDI_ROOT)/src/util/kaldi-util.a $(KALDI_ROOT)/src/base/kaldi-base.a 

include ../default_rules.mk
