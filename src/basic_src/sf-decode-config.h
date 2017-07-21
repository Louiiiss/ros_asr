#include "feat/wave-reader.h"
#include "online2/online-nnet2-decoding.h"
#include "online2/online-nnet2-feature-pipeline.h"
#include "online2/onlinebin-util.h"
#include "online2/online-timing.h"
#include "online2/online-endpoint.h"
#include "fstext/fstext-lib.h"
#include "lat/lattice-functions.h"
#include "thread/kaldi-thread.h"

namespace kaldi {
/**
 * The base class for decode configurations.
 */
class SFDecodeConfig {
	friend class WavDataSource;
public:
    /**
     * Delete any unwanted variables on destruction
     */
	virtual void destroy() = 0;
protected:
	fst::Fst<fst::StdArc> *decode_fst;
	fst::SymbolTable *word_syms;
	BaseFloat chunk_length_secs;
	string fst_rxfilename;
	string clat_wspecifier;
	string wav_rspecifier;
	OnlineEndpointConfig endpoint_config;

};


/**
 * Decode configuration for DNN decoder
 * @param argc Number of arguments
 * @param argv Argument array
 */
class SFNnetConfig : public SFDecodeConfig {
//friend class MyNnetDecoder;
//friend class MyNnetListener;
//friend class WavDataSource;
//friend class PortAudioDataSource;
friend class SFNnetDecoder;
public:
	SFNnetConfig(int argc, char* argv[]);
        
        /**
         * Deletes unwanted variables
         */
	void destroy();

protected:
	OnlineSilenceWeighting* silence_weighting;
	OnlineNnet2FeaturePipelineConfig feature_config; 
	OnlineNnet2DecodingConfig nnet2_decoding_config;
	TransitionModel* trans_model;
	OnlineNnet2FeaturePipelineInfo* feature_info;
	
	nnet2::AmNnet* nnet;
	
};

/**
 * Configuration for GMM decoder
 * @param argc Number of arguments
 * @param argv Argument array
 */
class SFGmmConfig : public SFDecodeConfig {
friend class SFGmmDecoder;
	
public:
	SFGmmConfig(int argc, char *argv[]);
	/**
         * Delete unwanted variables
         */
        void destroy();
protected:
	OnlineFeaturePipelineCommandLineConfig feature_cmdline_config;
	OnlineGmmDecodingConfig decode_config;
	OnlineFeaturePipelineConfig* feature_config;
	OnlineFeaturePipeline* pipeline_prototype;
	OnlineGmmDecodingModels* gmm_models;
};

}

// Yarp wrapper later


