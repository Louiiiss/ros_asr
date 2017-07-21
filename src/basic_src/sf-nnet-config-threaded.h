#include "feat/wave-reader.h"
#include "online2/online-nnet2-decoding-threaded.h"
#include "online2/online-nnet2-feature-pipeline.h"
#include "online2/onlinebin-util.h"
#include "online2/online-timing.h"
#include "online2/online-endpoint.h"
#include "fstext/fstext-lib.h"
#include "lat/lattice-functions.h"
#include "thread/kaldi-thread.h"

namespace kaldi {


/**
 * Decode configuration for DNN threaded decoder
 * @param argc Number of arguments
 * @param argv Argument array
 */
class SFNnetThreadedConfig : public SFDecodeConfig {
//friend class MyNnetDecoder;
//friend class MyNnetListener;
//friend class WavDataSource;
//friend class PortAudioDataSource;
friend class SFNnetThreadedDecoder;
public:
	SFNnetThreadedConfig(int argc, char* argv[]);
        
        /**
         * Deletes unwanted variables
         */
	void destroy();

protected:
	OnlineSilenceWeighting* silence_weighting;
	OnlineNnet2FeaturePipelineConfig feature_config; 
	OnlineNnet2DecodingThreadedConfig nnet2_decoding_config;
	TransitionModel* trans_model;
	OnlineNnet2FeaturePipelineInfo* feature_info;
	
	nnet2::AmNnet* nnet;
	
};


}

// Yarp wrapper later


