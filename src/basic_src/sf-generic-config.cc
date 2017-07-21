#include "my-online.h"

namespace kaldi {
SFGenericConfig::SFGenericConfig(int argc, char* argv[]) {
	utt = argv[f(argc, 0)];
	source = argv[f(argc, 1)];
	output_path = argv[f(argc, 2)];
	keyphrase = argv[f(argc, 3)];
	record_wav = argv[f(argc, 4)];
	read_pipe = atoi(argv[f(argc, 5)]);
	cout << "config read_pipe " << read_pipe << endl;

	should_listen = argv[f(argc, 6)];
	decode_type = argv[f(argc,7)];
	//interactive = argv[f(argc, 8)];
	std::ifstream infile("wav.scp");
	wav_rspecifier = (char*)"scp:wav.scp";
	string key, val;
	infile >> key;
	infile >> val;
	int dotindex = val.find_last_of(".");
	int slashindex = val.find_last_of("/");
	string rawname = val.substr(slashindex+1, dotindex-slashindex-1);
	//this->base_name = rawname.c_str();
	base_name = new char[rawname.length()+1];
	std::strcpy (base_name, rawname.c_str());
	//this->base_name = (char*)"base_name";
	//cout << "base name " << this->base_name << endl;
}
int SFGenericConfig::f(int num, int x) {
	return num - (NUM_EXTRA_ARGS - x);
}


}
