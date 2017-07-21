#include "feat/wave-reader.h"
#include "online2/online-nnet2-decoding.h"
#include "online2/onlinebin-util.h"
#include "online2/online-timing.h"
#include "online2/online-endpoint.h"
#include "fstext/fstext-lib.h"
#include "lat/lattice-functions.h"
#include "thread/kaldi-thread.h"

namespace kaldi {

    /**
     * Generic configuration, independent of the decoder
     */
    class SFGenericConfig {
//friend class MyNnetDecoder;
//friend class MyNnetListener;
friend class WavDataSource;
//friend class PortAudioDataSource;
friend class SFController;
friend class SFGenericDecoder;
public:
    /**
     * Constructor
     * @param argc Number of arguments
     * @param argv Array of arguments
     */
	SFGenericConfig(int argc, char* argv[]);
	// temporary - need to move to  wav data source and get parse options somehow
	//string get_wav_rspecifier();
protected:
	int f(int num, int x);
	char* utt; 
	char* source;
	const char* output_path;
	char* keyphrase;
	char* record_wav;
	//char* read_pipe;
	char* should_listen;
	char* decode_type;
	char* interactive;
	int read_pipe;
	char * base_name;
	char * wav_rspecifier;
};

}

// Yarp wrapper later


