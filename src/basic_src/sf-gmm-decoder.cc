#include "my-online.h"


// receives some data from the data source, and decodes it
namespace kaldi {



SFGmmDecoder::SFGmmDecoder(SFController* controller, SFGmmConfig* root_config, const char* keyphrase) {
	//cout << "SFNnetDecoder constructor" << endl;
	init(keyphrase);
	//cout << "gmm constructor keyphrase " << keyphrase << endl;
	//this->keyphrase = string(keyphrase);

	this->controller = controller;
	this->config = root_config;
	decoder_thread = new pthread_t();
	//lm_ready = true;

	//cout << "About to make feature pipeline" << endl;
	//this->feature_pipeline = new OnlineNnet2FeaturePipeline(*(config->feature_info));
	//cout << "Made pipeline" << endl;
}

void SFGmmDecoder::timeout() {
	// HAVE TO FILL THIS IN THE SAME AS NNET_DECODER
}

void SFGmmDecoder::decode(Vector<BaseFloat>* data) {
	//cout << "nnet listener decode" << endl;
	using namespace fst;
	//cout << "decode "<< this->keyphrase << endl;
	int32 num_done = 0;
	double tot_like = 0.0;
	int64 num_frames = 0;

	if (fresh_utterance) {
		//cout << "SingleUtterance.." << endl;
		utt_gmm_decoder = new SingleUtteranceGmmDecoder(config->decode_config,
		                  *(config->gmm_models),
		                  *(config->pipeline_prototype),
		                  *(config->decode_fst),
		                  adaptation_state);
		//cout << "SingleUtterance.. finished." << endl;
		fresh_utterance = false;
	}

	//BaseFloat max = data -> Max();
	//BaseFloat min = data -> Min();
	//cout << " listener decode min = " << min << " max = " << max << endl;

	//cout << "chunk length secs " << config->chunk_length_secs << endl;
	BaseFloat samp_freq = 16000;
	//cout << "decode data " << data -> Dim() << endl;
	utt_gmm_decoder -> FeaturePipeline().AcceptWaveform(samp_freq, *data);


	utt_gmm_decoder -> AdvanceDecoding();

	if (utt_gmm_decoder -> EndpointDetected(config->endpoint_config)) {

		bool end_of_utterance = true;
		bool rescore_if_needed = true;
		fresh_utterance = true;
		controller -> robotSpeaking = true;

		utt_gmm_decoder -> FinalizeDecoding();
		utt_gmm_decoder -> EstimateFmllr(end_of_utterance);
		CompactLattice clat;
		utt_gmm_decoder -> GetLattice(rescore_if_needed, end_of_utterance, &clat);
		//clat.Write(//cout);
		string ret = controller->output_string(config->word_syms, clat, &num_frames, &tot_like);
		controller->my_log(ret);
		//std::cout << "got ret '" << ret << "'" << std::endl;
		string keyphrase_str(keyphrase);
		//std::cout << "keyphrase '" << keyphrase_str << "'" << std::endl;
		if (ret == keyphrase+" ") {
			//std::cout << "got keyphrase " << keyphrase << std::endl;
			keyphrase_activated = true;
		}
		//utt_gmm_decoder -> OutputStats(&timing_stats);

		// In an application you might avoid updating the adaptation state if
		// you felt the utterance had low confidence.  See lat/confidence.h
		if (keyphrase_activated) {
			utt_gmm_decoder -> GetAdaptationState(&adaptation_state);
		}
		// we want to output the lattice with un-scaled acoustics.
		if (config->decode_config.acoustic_scale != 0.0) {
			BaseFloat inv_acoustic_scale = 1.0 / config->decode_config.acoustic_scale;
			ScaleLattice(AcousticLatticeScale(inv_acoustic_scale), &clat);
		}
		//clat_writer.Write(config->utt, clat);
		//KALDI_LOG << "Decoded utterance " << config->utt;
		num_done++;

	}
}









}
