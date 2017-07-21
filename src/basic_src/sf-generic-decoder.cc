#include "my-online.h"


// receives some data from the data source, and decodes it
namespace kaldi {


std::string SFGenericDecoder::ReplaceString(std::string subject, const std::string& search,
        const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void SFGenericDecoder::init(const char* my_keyphrase) {
	//cout << my_keyphrase << endl;
	//cout << "generic listener init" << endl;

	this->data_to_process = new std::queue<Vector <BaseFloat>* >();
	//cout << "Made data_to_process "  << endl;

	//cout << "Made keyphrase and data_to_process " << keyphrase << endl;

	this->keyphrase = string(my_keyphrase);
	this->keyphrase = ReplaceString(this->keyphrase, "_", " ");
	//cout << "init " << this->keyphrase;
	this->decode_lock = false;
	//decoder_thread = new pthread_t();
	//cout << data_to_process << endl;
	
	/*std::ifstream ifs;
	ifs.open ("fst_config_gen.txt", std::ifstream::in);
	
	if (ifs.fail()) {
		cerr << "Failed to open fst_config_gen.txt" << endl;
		std::exit(0);
	}
	std::string type;
	int no;
	//char c = ifs.get();
	*/
	//fst_switch = new FSTSwitcher("jsgf-dialogue");
	//cerr << "About to start reading FSTs" << endl;
	//std::map<std::string,int> config;
	/*while (ifs >> type >> no) {
		cerr << "FST got " << type << " " << no << endl;
		config[type] = no;
		current_fst = fst_switch -> get_fst(type, 0);
		current_word_syms = fst_switch -> get_word_syms(type, 0);
	}*/
	

	input_finished = false;
	keyphrase_activated = false;
	fresh_utterance = true;
	active = false;
	//fst_switch = new FSTSwitcher("grammar/jsgf-dialogue");


}

//void SFGenericDecoder::setRobotSpeaking(bool val) {
//	this->robotSpeaking = val;
//}

//void SFGenericDecoder::setQuestionNo(int question_no) {
//	this->question_no = question_no;
//}


void SFGenericDecoder::waitForFinish() {
	pthread_join( *decoder_thread, NULL);
}

void SFGenericDecoder::start() {
	//cout << "SFGenericDecoder start" << endl;
	int iret = pthread_create( decoder_thread, NULL, SFGenericDecoder::startStatic, this);
	if(iret)
	{
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
		exit(EXIT_FAILURE);
	}
}

void* SFGenericDecoder::startStatic(void* arg) {
	((SFGenericDecoder*)arg)->start_impl();
	return NULL;
}

void SFGenericDecoder::start_impl() {
	active = true;
	//cout << "starting listener " << endl;
	//cout << data_to_process << endl;

	//string date = controller -> get_date_string();
	//string out_str(controller->gen_config->output_path);
	//string str = out_str+"/speech/"+date+".wav";
	//const int32 kSampleFreq = 16000;
	//Audio_WAV_OpenWriter(&writer, str.c_str(), kSampleFreq, 1);
	while (active) {
		//cout << "listener active " << active << endl;

		while (!data_to_process -> empty()) {
			//	cout << "processing queue" << endl;
			//	cout << "sizeof queue " << sizeof(*data_to_process) << endl;
			Vector<BaseFloat>* data = data_to_process -> front();

			//BaseFloat max = data.Max();
			//BaseFloat min = data.Min();
			//cout << "listener start loop min " << min << " max " << max << endl;
			//cout << "sizeof first data " << sizeof(data) << endl;

			//cout << "more to decode " << data.Dim() << endl;
			data_to_process->pop();
			decode(data);
		}

		//if (input_finished) {
			//utt_gmm_decoder-> FeaturePipeline().InputFinished();
		//}
	}

	//Audio_WAV_CloseWriter(&writer);
}







void SFGenericDecoder::stop() {
	active = false;
}

void SFGenericDecoder::hearPrev(RingBuffer* buf) {
	cerr << "Hear prev" << endl;
	for (int i=0;i<buf->length();++i) {
		Vector<BaseFloat>* prev = buf->read(i);
		/*float avg = 0.0;
		for (int j=0;j<prev->Dim();++j) {
			avg+=abs(prev->Data()[j]);
		}
		avg=avg/prev->Dim();
		cerr << "Hear prev " << i << " " << avg << endl;
		*/
		
		//data_to_process -> push(prev);
		decode(prev);
	}
}

/*
void SFGenericDecoder::hearEnd(SubVector<BaseFloat> *data) {
	//cout << "genericlistener::hear data dim " << data -> Dim() << endl;
	using namespace std;
	//short short_data[data->Dim()];

	int i=0;
	BaseFloat* buf = new BaseFloat[800];
	for (int j=0;j<800;++j) {
		buf[j] = 0;
	}

	while (i<data->Dim()) {
		BaseFloat f = data->Data()[i];
		buf[i%800]=f;
		
		if (i>0 && i%800==0) {
			float avg = 0.0;
			for (int j=0;j<800;++j) {
				avg+=abs(buf[j]);
			}
			avg=avg/800.0;
			if (avg<700) break;
			//cerr << "avg " << avg << endl;
		}
		++i;
	}
	int32 cut = i;
	
	//ostringstream ss;
	//ss.clear();
//	ss << "storing prev data " << prevData->Dim();
//	cerr << "Cut at " << cut << endl;
	//ss << "Cut at " << cut << endl;
	//controller->my_log(ss.str());
	
	controller->output->write_cut_prev(cut);
	//for (int i=cut;i<data->Dim();++i) {
	//	short_data[i-cut] = (short)data->Data()[i];
		//cout << data[i] <<  "\t" << vec_data.Data()[i] << endl;

//	}
	

	SubVector<BaseFloat> part(*data, cut, data->Dim()-cut);	
	data_to_process -> push(part);

	//Audio_WAV_WriteShorts( &writer, short_data, data->Dim()-cut);

}*/

void SFGenericDecoder::hear(Vector<BaseFloat>* data) {
	//cout << "genericlistener::hear data dim " << data -> Dim() << endl;
	
	/*float avg = 0.0;
		for (int j=0;j<data->Dim();++j) {
			avg+=abs(data->Data()[j]);
		}
		avg=avg/data->Dim();
		cerr << "Hear normal "  << " " << avg << endl;*/
	//data_to_process -> push(data);

		decode(data);
	//short short_data[data->Dim()];


	//for (int i=0;i<data->Dim();++i) {
	//	short_data[i] = (short)data->Data()[i];
		//cout << data[i] <<  "\t" << vec_data.Data()[i] << endl;

	//}
	//for (int i=0;i<data->Dim();++i) {
	//short_data[i] = (short)data->Data()[i];
	//cout << data[i] <<  "\t" << vec_data.Data()[i] << endl;

	//}
	//Audio_WAV_WriteShorts( &writer, short_data, data->Dim());

}


void SFGenericDecoder::input_finished_func() {
	input_finished = true;
}


}
