#include "my-online.h"
#include <write_wav.h>

#include <algorithm>


namespace kaldi {


	void DataSource::waitForFinish() {
		pthread_join( *src_thread, NULL);
	}

//WavDataSource::WavDataSource(const char* base_name, string wav_r, string utt, MyOnlineDecoder* decoder, const char* output_path) {
WavDataSource::WavDataSource(string utt, SFController* controller) {
	
	//cout << "base name " << base_name << endl;
	
	
	
	//base_name = controller->gen_config->base_name;
	//read_chunk_lengths(base_name, output_path);
	//cout << "wav_r " << wav_r << endl;
	//this->wav_rspecifier = wav_r;
	src_thread = new pthread_t();
	//cout << "WaV constructor" << endl;
	wav_rspecifier = new string("scp:wav.scp");
	this->utt = utt;
	this->controller = controller;
	this->wav_reader = new RandomAccessTableReader<WaveHolder>(*wav_rspecifier);
}



void WavDataSource::read_chunk_lengths(const char* base_name, const char* output_path) {
	string out_str(output_path);
	string path=out_str+"/chunks/";
	
	path = path + base_name;
	path = path + ".chunk.txt";
	std::ifstream infile(path.c_str());
	int32 chunk_length;
	while (infile >> chunk_length)
	{
		//cout << chunk_length << endl;
		chunk_lengths.push(chunk_length);	
    }
}

void WavDataSource::start() {
	//cout << "wav start " << endl;
	int iret = pthread_create( src_thread, NULL, WavDataSource::startStatic, this);
	if(iret)
	{
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
		exit(EXIT_FAILURE);
	}
}


void* WavDataSource::startStatic(void* arg) {
	//cout << " wav start static " << endl;
	((WavDataSource*)arg)->start_impl();
	return NULL;
}

void WavDataSource::start_impl() {
	// simulate online input by splitting data into chunks and sending
	//cout << "Starting wav source" << endl;
	active = true;
	//cout << "utt " << utt << endl;
	
	if (wav_reader->IsOpen()) {
		//cout << "wav reader open" << endl;
	}
	else {
	KALDI_ERR << "wav reader not open";
	
	}
	if (wav_reader->HasKey(utt)) {
		//cout << "Found key for " << utt << endl;
		
	}
	else {
          KALDI_WARN << "Did not find audio for utterance " << utt;
          return;
   }
        
	const WaveData &wave_data = wav_reader->Value(utt);
	//cout << "wave data created " << endl;
	//cout << wave_data.Data().NumRows() << endl;
	SubVector<BaseFloat> data(wave_data.Data(), 0);
	//cout << "Made 1st sub vector " << endl;
	//BaseFloat samp_freq = wave_data.SampFreq();
	//int32 chunk_length = int32(samp_freq * controller->decode_config->chunk_length_secs);
	//int32 chunk_length = 7904;
	//if (chunk_length == 0) chunk_length = 1;

	int32 samp_offset = 0;
	int32 chunk_length = 800;
	
	while (samp_offset < data.Dim()) {
		
		/*int32 read_chunk_length = 8000;
		
		if (!chunk_lengths.empty()) {
			read_chunk_length = chunk_lengths.front();
			chunk_lengths.pop();
		}
		chunk_length = std::min(chunk_length, read_chunk_length);
		//cout << chunk_length << endl;
		*/
		
		//if (!chunk_lengths.empty()) {
			//chunk_length = chunk_lengths.front();
			//chunk_lengths.pop();
		//}
		
		//cout << "reading samp " << samp_offset << endl;
		int32 samp_remaining = data.Dim() - samp_offset;
		int32 num_samp = chunk_length < samp_remaining ? chunk_length
		                 : samp_remaining;

		SubVector<BaseFloat> wave_part(data, samp_offset, num_samp);
		//cout << "Made 2nd sub vector " << endl;
		Vector<BaseFloat> d(wave_part);
		controller->hear(&d);
		samp_offset += num_samp;
		
		//cout << "num_samp " << num_samp << endl;
		//float wait_time = 1000000.0 * num_samp / 16000.0;
		//cout << "waiting " << wait_time << endl;
		//usleep(wait_time);
		if (samp_offset == data.Dim()) {
			// no more input. flush out last frames
			controller->input_finished();
		}
	}

	cout << "Sent all data from WAV. Now just keeping busy for simulation " << endl;
	//usleep(2000000);
	//decoder->terminate();
	//while (active) {
	// just pretend to be busy to simulate online input
	// cout << "source active" << endl;
	//	usleep(100000);
	//}

	//cout << "Finished source" << endl;

	/*cout << "Start receiving" << endl;
	const char* data = "test";
	decoder->sendData(data);*/


}

void WavDataSource::stop() {
	//cout << "Stopping wav data source" << endl;
	DataSource::stop();
	active = false;
}


//PortAudioDataSource::PortAudioDataSource(MyOnlineDecoder* decoder, const char* output_path, const char* record_wav) {
PortAudioDataSource::PortAudioDataSource(SFController* controller, const char* output_path, const char* record_wav) {
		
//cout << "Base class constructor" << endl;
src_thread = new pthread_t();
		
au_src = new OnlinePaSource(kTimeout, kSampleFreq, kPaRingSize, kPaReportInt);
	
	this->controller = controller;
	this->output_path = output_path;
	this->record_wav = string(record_wav);
}

void PortAudioDataSource::start() {
	int iret = pthread_create( src_thread, NULL, PortAudioDataSource::startStatic, this);
	if(iret)
	{
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
		exit(EXIT_FAILURE);
	}
}

void* PortAudioDataSource::startStatic(void* arg) {
	((PortAudioDataSource*)arg)->start_impl();
	return NULL;
}

void PortAudioDataSource::start_impl() {
	//cout << "Starting pa source" << endl;
	active = true;
	
	string date = controller -> get_date_string();
	string out_str(output_path);
	string day = controller -> get_day_string();
	string str = out_str+"/"+day+"/speech/"+date+".wav";
	WAV_Writer writer;
	cerr << "Freq " << kSampleFreq << endl;
	if (record_wav=="ON") {
		Audio_WAV_OpenWriter(&writer, str.c_str(), kSampleFreq, 1);
	}
	int32 chunk_size = kTimeout * kSampleFreq / 1000;
	Vector<BaseFloat> vec_data(chunk_size);
	short data[chunk_size];
	int sampleCount = 0;
	

	while (active) {
		//cout << "source active" << endl;
		//cout << sampleCount << endl;
		au_src -> Read(&vec_data);
		
		
		//cout << " Read data " << vec_data.Dim() << endl;
		controller -> add_chunk(vec_data.Dim());
		//cout << " add chunk " << vec_data.Dim() << endl;
		
		sampleCount += vec_data.Dim();




		//SubVector<BaseFloat> wave_part(vec_data, 0, vec_data.Dim());

		//BaseFloat max = wave_part.Max();
		//BaseFloat min = wave_part.Min();
		//cout << "pasource start min " << min << " max " << max << endl;
		//cout << " Read data " << wave_part.Dim() << endl;

		//cerr << "pa source hearing data " << vec_data.Dim() << endl;
		controller -> hear(&vec_data);

		for (int i=0;i<vec_data.Dim();++i) {
			data[i] = (short)vec_data.Data()[i];
			//cout << data[i] <<  "\t" << vec_data.Data()[i] << endl;

		}
		
		if (record_wav=="ON") {
			Audio_WAV_WriteShorts( &writer, data, vec_data.Dim());
		}
		//cout << "Return from hear " << endl;
	}

	if (record_wav=="ON") {
		Audio_WAV_CloseWriter(&writer);
	}

}

void PortAudioDataSource::stop() {
	//cout << "Stopping PA data source" << endl;
	DataSource::stop();
	active = false;
}


}



