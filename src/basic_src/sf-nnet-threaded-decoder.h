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
 * Constructor for DNN decoder
 * @param controller The controller instance
 * @param root_config The configuration file
 * @param keyphrase The starting keyphrase
 */
class SFNnetThreadedDecoder : public SFGenericDecoder {
public:
	SFNnetThreadedDecoder(SFController *controller, SFNnetThreadedConfig* root_config, const char* keyphrase);
	// receives some data from the data source, and decodes it
	//void start();
        /**
         * Clean exit for timeout
         */
	void timeout();
protected:
	//static void* startStatic(void *arg);
	SFNnetThreadedConfig* config;
	void finishDecoding();
	void decode(Vector<BaseFloat>* data);
	SingleUtteranceNnet2DecoderThreaded* nnet_decoder;
	OnlineIvectorExtractorAdaptationState* adaptation_state;
	//OnlineNnet2FeaturePipeline* feature_pipeline;
	int count;
};

}
