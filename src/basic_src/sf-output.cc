#include "my-online.h"
#include <fstream> 

namespace kaldi {

//MyOnlineASROutput::MyOnlineASROutput(MyOnlineDecoder *decoder, char* source, const char* base_name, const char* output_path) {
SFOutput::SFOutput(SFController *controller, char* source, const char* base_name, const char* output_path) {
	//cout << "Output constructor " << source << " " << base_name << " " << output_path << endl; 	
	this -> controller = controller;
	string date = controller -> get_date_string();
	string day = controller -> get_day_string();
	string out_str(output_path);
	string str;
	if (!strcmp(source, "pa")) {
		str = out_str+"/"+day+"/text/"+date+".txt";
		out_file.open(str.c_str(),  std::fstream::out);
		str = out_str+"/"+day+"/chunks/"+date+".chunk.txt";
		chunk_lengths.open(str.c_str(),  std::fstream::out);
		str = out_str+"/"+day+"/controls/"+date+".control.txt";
		controls.open(str.c_str(),  std::fstream::out);
		
		if (out_file.is_open() && chunk_lengths.is_open() && controls.is_open()) {
			controller -> my_log("Successfully opened out files in " + out_str);
			//cerr << "Successfully opened out files in " << out_str << endl;
		}
		else {
			controller -> my_log("Failed to open files in " + out_str);
			//cerr << "Failed to open files in " << out_str << endl;
		}
		//cout << "Opened stuff in " << out_str << endl;
		
	}
	else if (!strcmp(source, "wav")) {
		string base_str(base_name);
		str = out_str+"/"+day+"/text-wav-eval/"+base_str+".txt";
		out_file.open(str.c_str(),  std::fstream::out);
		if (out_file.is_open()) {
			cerr << "Successfully opened " << str << endl;
		}
		else {
			cerr << "Failed to open " << str << endl;
			std::exit(1);
		}
	}
	gettimeofday(&start, NULL);
}
/*

void SFOutput::write_cut_prev(int cut) {
	char cur_time[84];
	controller -> get_timestamp(cur_time);
	controls << cur_time << " Cut at " << cut << endl;
}*/

void SFOutput::write_readback(MatrixIndexT size) {
	char cur_time[84];
	controller -> get_timestamp(cur_time);
	controls << cur_time << " Read back " << size << endl;
}


void SFOutput::write_segment(MatrixIndexT start, MatrixIndexT end, bool val) {
	char cur_time[84];
	controller -> get_timestamp(cur_time);
	controls << cur_time << " " << start << " " << end << " " << val << endl;
}

string SFOutput::output_string(fst::SymbolTable* word_syms,
                                      CompactLattice clat, int64* tot_num_frames, double* tot_like, MatrixIndexT num_samples) {
	if (clat.NumStates() == 0) {
		KALDI_WARN << "Empty lattice.";
		return "";
	}
	CompactLattice best_path_clat;
	CompactLatticeShortestPath(clat, &best_path_clat);

	Lattice best_path_lat;
	ConvertLattice(best_path_clat, &best_path_lat);

	double likelihood;
	LatticeWeight weight;
	int32 num_frames;
	std::vector<int32> alignment;
	std::vector<int32> words;
	GetLinearSymbolSequence(best_path_lat, &alignment, &words, &weight);
	num_frames = alignment.size();
	likelihood = -(weight.Value1() + weight.Value2());
	*tot_num_frames += num_frames;
	*tot_like += likelihood;
	KALDI_VLOG(2) << "Likelihood per frame for utterance is " << (likelihood / num_frames) << " over " << num_frames << " frames.";

	//double t1=start.tv_sec+(start.tv_usec/1000000.0);

	struct timeval now;
	gettimeofday(&now, NULL);
	//double t2=now.tv_sec+(now.tv_usec/1000000.0);
	//double elapsed = t2 - t1;

	char cur_time[84];
	controller -> get_timestamp(cur_time);

	out_file << cur_time << " " <<  num_samples << ' ';
	//out_file << elapsed << ' ';
	if (word_syms!=NULL) {
		//std::cerr << utt << ' ';
		//std::cout << utt << ' ';
		string ret = "";
		for (size_t i = 0; i < words.size(); i++) {
			std::string s = word_syms->Find(words[i]);
			if (s == "")
				KALDI_ERR << "Word-id " << words[i] << " not in symbol table.";
			//std::cerr << s << ' ';
			std::cout << s << ' ';
			out_file << s << ' ';
			ret += s+' ';
		}
		//std::cerr << std::endl;
		std::cout << std::endl;
		out_file << std::endl;
		return ret;
	}
	return "";
}

void SFOutput::add_chunk(int size) {
		char cur_time[84];
	controller -> get_timestamp(cur_time);
//	cout << "output add_chunk" << endl;
	chunk_lengths << cur_time << " " <<  size << endl;
}


}





