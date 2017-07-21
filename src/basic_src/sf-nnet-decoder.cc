#include "my-online.h"


// receives some data from the data source, and decodes it
namespace kaldi {



SFNnetDecoder::SFNnetDecoder(SFController* controller, SFNnetConfig* root_config, const char* keyphrase) {
	//cout << "SFNnetDecoder constructor" << endl;
	init(keyphrase);
	//cout << keyphrase << endl;

	this->controller = controller;
	this->config = root_config;
	//cout << "About to make feature pipeline" << endl;
	this->feature_pipeline = new OnlineNnet2FeaturePipeline(*(config->feature_info));
	adaptation_state = new OnlineIvectorExtractorAdaptationState(config->feature_info->ivector_extractor_info);

	this -> count = 0;
	feature_pipeline -> SetAdaptationState(*adaptation_state);
	decoder_thread = new pthread_t();
	//cout << "Made pipeline" << endl;
}

void SFNnetDecoder::timeout() {

	//nnet_decoder -> AdvanceDecoding();
	while (decode_lock) {
		controller->my_log("Waiting for decode_lock");
	}
	decode_lock = true;
	controller->my_log("Entering nnet timeout");

	controller->my_log("LM ready, so clearing the decoder");
	//nnet_decoder -> AdvanceDecoding();
	fresh_utterance = true;
	//nnet_decoder->FinalizeDecoding();
	//lm_ready = false;
	if (!controller->always_listen) {
		controller -> should_listen = false;
	}
	feature_pipeline -> InputFinished();
	feature_pipeline = new OnlineNnet2FeaturePipeline(*(config->feature_info));


	//else {
	//	controller->my_log("LM not ready so doing nothing");
	//}
	decode_lock = false;

}

void SFNnetDecoder::finishDecoding() {
	using namespace fst;
	//controller->my_log("nnet finishDecoding");
	int32 num_done = 0;
	double tot_like = 0.0;
	int64 num_frames = 0;
	bool end_of_utterance = true;
	fresh_utterance = true;
	
	Timer timer;
	//controller -> robotSpeaking = true;
	nnet_decoder -> FinalizeDecoding();
	//nnet_decoder -> EstimateFmllr(end_of_utterance);
	CompactLattice clat;
	//controller->my_log("Getting Lattice");
	
	
	nnet_decoder -> GetLattice(end_of_utterance, &clat);
	//clat.Write(//cout);
	string ret = controller->output_string(config->word_syms, clat, &num_frames, &tot_like);
	//cout << "ret '" << ret << "'" << endl;
	//cout << "keyphrase '" << keyphrase << "'" << endl;
	controller -> my_log(ret);


	//if (ret == keyphrase+" ") {
//		keyphrase_activated = true;
		//cout << "got keyphrase '" << keyphrase << "'" << endl;

	//}
	//nnet_decoder -> OutputStats(&timing_stats);

	// In an application you might avoid updating the adaptation state if
	// you felt the utterance had low confidence.  See lat/confidence.h

	bool robotSpeaking = controller->robotSpeaking;
	//if (keyphrase_activated && !robotSpeaking) {
		//cerr << "Getting adaptation state" << endl;
		//feature_pipeline -> GetAdaptationState(adaptation_state);
	//}
	//else if (robotSpeaking) {
		//cerr << "Not getting adaptation state because robot speaking" << endl;
	//}
	//BaseFloat inv_acoustic_scale = 1.0 / config->nnet2_decoding_config.decodable_opts.acoustic_scale;
	//ScaleLattice(AcousticLatticeScale(inv_acoustic_scale), &clat);
	feature_pipeline -> InputFinished();
	  KALDI_LOG << "Adding the various end-of-utterance tasks took the "
                        << "total latency to " << timer.Elapsed() << " seconds.";
	//input_finished = true;
	feature_pipeline = new OnlineNnet2FeaturePipeline(*(config->feature_info));
	//if (keyphrase_activated && !robotSpeaking) {
		//cerr << "Setting adaptation state" << endl;
		//feature_pipeline->SetAdaptationState(*adaptation_state);
	//}

	//else if (robotSpeaking) {
		//	cerr << "Not setting adaptation state because robot speaking" << endl;
	//}
	// we want to output the lattice with un-scaled acoustics.
	//clat_writer.Write(config->utt, clat);
	//KALDI_LOG << "Decoded utterance " << config->utt;
	num_done++;
}

void SFNnetDecoder::decode(Vector<BaseFloat>* data) {
	//cout << "nnet listener decode " << count << endl;
	count++;
	using namespace fst;



	while (decode_lock) {
		controller->my_log("Waiting for decode_lock");
	}
	decode_lock = true;


	/*float avg = 0.0;
		for (int j=0;j<data->Dim();++j) {
			avg+=abs(data->Data()[j]);
		}
		avg=avg/data->Dim();
		cerr << "nnet decode " << avg << endl;*/
	//controller->my_log("Entering nnet decode");
	//int32 num_done = 0;
	//double tot_like = 0.0;
	//int64 num_frames = 0;

	if (fresh_utterance) {
		controller->my_log("Making new SingleUttDecoder");

		nnet_decoder = new SingleUtteranceNnet2Decoder(config->nnet2_decoding_config,
		               *(config->trans_model),
		               *(config->nnet),
		               *(config->decode_fst),
		               this->feature_pipeline);
		
		//cout << "SingleUtterance.. finished." << endl;
		fresh_utterance = false;
	}

	//BaseFloat max = data -> Max();
	//BaseFloat min = data -> Min();
	//cout << " listener decode min = " << min << " max = " << max << endl;

	//cout << "chunk length secs " << config->chunk_length_secs << endl;
	BaseFloat samp_freq = 8000;

	//cout << "decode data " << data -> Dim() << endl;
	//controller->my_log("Accepting waveform");
	feature_pipeline -> AcceptWaveform(samp_freq, *data);



	//controller->my_log("Nnet Advance decoding");
	nnet_decoder -> AdvanceDecoding();


	if (nnet_decoder -> EndpointDetected(config->endpoint_config)) {
		finishDecoding();
	}
	decode_lock = false;

}







}
