#include "my-online.h"

namespace kaldi {
SFNnetThreadedConfig::SFNnetThreadedConfig(int argc, char*argv[]) {
	using namespace kaldi;
	using namespace fst;

	//cerr << "MyNnetConfig" << endl;
	
	typedef kaldi::int32 int32;
	typedef kaldi::int64 int64;
	


	std::string word_syms_rxfilename;

	//	OnlineEndpointConfig endpoint_config;

	// feature_config includes configuration for the iVector adaptation,
	// as well as the basic features.
	// OnlineNnet2FeaturePipelineConfig feature_config;
	// OnlineNnet2DecodingConfig nnet2_decoding_config;

	//  BaseFloat chunk_length_secs = 0.05;
	bool do_endpointing = true;
	bool online = true;
   bool modify_ivector_config = false;
    bool simulate_realtime_decoding = true;
	  const char *usage =
        "Reads in wav file(s) and simulates online decoding with neural nets\n"
        "(nnet2 setup), with optional iVector-based speaker adaptation and\n"
        "optional endpointing.  This version uses multiple threads for decoding.\n"
        "Note: some configuration values and inputs are set via config files\n"
        "whose filenames are passed as options\n"
        "\n"
        "Usage: online2-wav-nnet2-latgen-threaded [options] <nnet2-in> <fst-in> "
        "<spk2utt-rspecifier> <wav-rspecifier> <lattice-wspecifier>\n"
        "The spk2utt-rspecifier can just be <utterance-id> <utterance-id> if\n"
        "you want to decode utterance by utterance.\n"
        "See egs/rm/s5/local/run_online_decoding_nnet2.sh for example\n"
        "See also online2-wav-nnet2-latgen-faster\n";

	ParseOptions* po = new ParseOptions(usage);

	po->Register("chunk-length", &chunk_length_secs,
                "Length of chunk size in seconds, that we provide each time to the "
                "decoder.  The actual chunk sizes it processes for various stages "
                "of decoding are dynamically determinated, and unrelated to this");
    po->Register("word-symbol-table", &word_syms_rxfilename,
                "Symbol table for words [for debug output]");
    po->Register("do-endpointing", &do_endpointing,
                "If true, apply endpoint detection");
    po->Register("modify-ivector-config", &modify_ivector_config,
                "If true, modifies the iVector configuration from the config files "
                "by setting --use-most-recent-ivector=true and --greedy-ivector-extractor=true. "
                "This will give the best possible results, but the results may become dependent "
                "on the speed of your machine (slower machine -> better results).  Compare "
                "to the --online option in online2-wav-nnet2-latgen-faster");
    po->Register("simulate-realtime-decoding", &simulate_realtime_decoding,
                "If true, simulate real-time decoding scenario by providing the "
                "data incrementally, calling sleep() until each piece is ready. "
                "If false, don't sleep (so it will be faster).");
    po->Register("num-threads-startup", &g_num_threads,
                "Number of threads used when initializing iVector extractor.  ");

	feature_config.Register(po);
	nnet2_decoding_config.Register(po);
	
	cout << "Nnet feature config " << feature_config.feature_type << endl;
	endpoint_config.Register(po);
	
	po -> Read(argc, argv);

	for (int i=1;i<=po->NumArgs();++i) {
		cout << i << " " << po->GetArg(i) << endl;
	}
	//cout << "nnet-config reading PO" << endl;
	if (po->NumArgs() != 4) {
		//cout << "args = " << po->NumArgs() << endl;
		po->PrintUsage();
		exit(1);
	}
	
	
	
	
	
		//clat_wspecifier = po.GetArg(3);
	OnlineEndpointRule rule1(false, 3.0, std::numeric_limits<BaseFloat>::infinity(), 0.0);
	//OnlineEndpointRule rule2(true, 0.5, 2.0, 0.0);
    //OnlineEndpointRule rule3(true, 1.0, 8.0, 0.0);

	OnlineEndpointRule rule2(true, 0.2, 4.0, 0.0);
    OnlineEndpointRule rule3(true, 0.4, 16.0, 0.0);



    OnlineEndpointRule rule4(true, 2000.0, std::numeric_limits<BaseFloat>::infinity(), 0.0);
    OnlineEndpointRule rule5(false, 0.0, std::numeric_limits<BaseFloat>::infinity(), 5.0);

	endpoint_config.rule1 = rule1;
	endpoint_config.rule2 = rule2;
	endpoint_config.rule3 = rule3;
	endpoint_config.rule4 = rule4;
	endpoint_config.rule5 = rule5;
	




	std::string nnet2_rxfilename = po->GetArg(1);
	std::string fst_rxfilename = po->GetArg(2);
	wav_rspecifier = po->GetArg(3);
	std::string clat_wspecifier = po->GetArg(4);

	//cout << "read all strings" << endl;

	feature_info = new OnlineNnet2FeaturePipelineInfo(feature_config);

	//cout << "feature info made" << endl;

	
	if (modify_ivector_config) {
      feature_info->ivector_extractor_info.use_most_recent_ivector = true;
      feature_info->ivector_extractor_info.greedy_ivector_extractor = true;
    }
    
	//if (!online) {
		//feature_info->ivector_extractor_info.use_most_recent_ivector = true;
		//feature_info->ivector_extractor_info.greedy_ivector_extractor = true;
		//chunk_length_secs = -1.0;
	//}

	//cout << "about to get trans model" << endl;

	trans_model = new TransitionModel();
	nnet = new nnet2::AmNnet();

	{
		bool binary;
		Input ki(nnet2_rxfilename, &binary);
		trans_model -> Read(ki.Stream(), binary);
		//cout << "read trans_model" << endl;

		nnet -> Read(ki.Stream(), binary);
		//cout << "read nnet" << endl;

	}

	decode_fst = ReadFstKaldi(fst_rxfilename);
	//cout << "Read fst" << endl;
	word_syms = NULL;
	if (word_syms_rxfilename != "") {
		if (!(word_syms = fst::SymbolTable::ReadText(word_syms_rxfilename))) {
			KALDI_ERR << "Could not read symbol table from file " << word_syms_rxfilename;
		}
	}

	//cout << "Finished config" << endl;



}

void SFNnetThreadedConfig::destroy() {
	//cout << "nnet config destroy " << endl;
	delete decode_fst;
	delete word_syms;
}


}




