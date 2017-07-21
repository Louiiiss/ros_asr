#include "feat/wave-reader.h"
#include "online2/online-feature-pipeline.h"
//#include "online2/online-gmm-decoding.h"
#include "online2/onlinebin-util.h"
#include "online2/online-timing.h"
#include "online2/online-endpoint.h"
#include "fstext/fstext-lib.h"
#include "lat/lattice-functions.h"
#include "online2/online-nnet2-decoding.h"
#include <queue>
#include "online/online-audio-source.h"

namespace kaldi {
/**
 * Constructor for GMM decoder.
 * 
 * @param controller The controller instance
 * @param root_config The config
 * @param keyphrase The starting keyphrase
 */
class SFGmmDecoder : public SFGenericDecoder {
public:
	SFGmmDecoder(SFController *controller, SFGmmConfig* root_config, const char* keyphrase);
	// receives some data from the data source, and decodes it
	//void start();
        /**
         * Clean exit for timeout
         */
	void timeout();
protected:
	//static void* startStatic(void *arg);
	SFGmmConfig* config;
	void decode(Vector<BaseFloat>* data);
	SingleUtteranceGmmDecoder* utt_gmm_decoder;
	OnlineGmmAdaptationState adaptation_state;

	
};

}
