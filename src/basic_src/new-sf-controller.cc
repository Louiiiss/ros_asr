#include "my-online.h"
#include <signal.h>

// receives some data from the data source, and decodes it
namespace kaldi {

SFController* SFController::instance;

void SFController::my_log(string s) {
	char currentTime[84];
	get_timestamp(currentTime);
	while (writeLock) {}
	
	writeLock = true;
	cerr << currentTime << " " << s << endl;
	writeLock = false;
}

void SFController::get_timestamp(char* currentTime) {
	timeval curTime;
	gettimeofday(&curTime, NULL);
	int milli = curTime.tv_usec/1000;
	char buffer[80];
	strftime(buffer, 80, "%d-%m-%Y-%H:%M:%S", localtime(&curTime.tv_sec));
	//char currentTime[84]="";
	sprintf(currentTime, "%s:%03d", buffer, milli);
	//return currentTime;
}

SFController::SFController(int argc, char* argv[]) {
	this -> argc = argc;
	this -> argv = argv;
	this -> writeLock = false;
	for (int i=0;i<argc;++i) {
		//cerr << argv[i] << endl;
	}
	this -> startListening = argv[argc-2];

	this->read_pipe = atoi(argv[argc-3]);
	//cerr << "read pipe " << read_pipe << endl;
	string write = "read pipe ";
	write.append(argv[argc-3]);
	my_log(write);
	//my_buffer = new BaseFloat[8000];
	this -> should_listen = false;
	this -> always_listen = false;
	this -> sample_count = 0;
	controller_thread = new pthread_t();
	this -> prevData = new RingBuffer(14);
	/*	cerr << "decode_type " << decode_type << endl;
		cerr << "should_listen " << should_listen << endl;
		cerr << "read_pipe " << read_pipe << endl;
		cerr << "record_wav " << record_wav << endl;
		cerr << "keyphrase " << keyphrase << endl;
		cerr << "output_path " << output_path << endl;
		cerr << "source "<< source << endl;
		cerr << "utt " << utt << endl;
		cerr << "interactive " << interactive << endl;*/

}

void SFController::hear(Vector<BaseFloat>* data) {
	
	using namespace std;
	//cout << "controller hear "<< data->Dim() << endl;
	MatrixIndexT start = sample_count;
	MatrixIndexT end = sample_count + data->Dim();
	sample_count += data->Dim();
	//for (int i=0;i<data->Dim();++i) {
	//	my_buffer[i] = data->Data()[i];
	//}
	
	if (should_listen && !robotSpeaking) {
		
		//cerr << "decode hear should_listen !robotSpeaking" << endl;
		//if (prevRobotSpeaking) {
			// flush out the last bit of data
			// lets try 0.3 seconds..
			/*if (prevData->Dim()>0) {
			//	ostringstream ss;
		//		ss.clear();
			//	ss << "sending prev data " << prevData->Dim();
				//my_log(ss.str());
				
				output->write_readback(prevData->Dim());
				
				SubVector<BaseFloat> sub(*prevData, 0, prevData->Dim());
				decoder->hearEnd(&sub);
			}*/
			//
			//cerr << "prev robot speaking, about to write readback" << endl;

			//output->write_readback(prevData->totalSize());
			
			//cerr << "prev robot speaking, about to hear prev" << endl;
			
			//if (asrEnabled) {
			//	decoder->hearPrev(prevData);
			//}
			
		//}

		//cerr << "now hearing data and writing segment" << endl;
		
		if (asrEnabled) {
			decoder -> hear(data);
		}
		output -> write_segment(start, end, true);
		

	}
	else {
		
		//cerr << "either !should_listen or robot speaking" << endl;

		//cerr << "about to write segment" << endl;

		output -> write_segment(start, end, false);
		
		/*float avg = 0.0;
		for (int j=0;j<data->Dim();++j) {
			avg+=abs(data->Data()[j]);
		}
		avg=avg/data->Dim();
		cerr << "Inserting "  << " " << avg << endl;
		*/
		
		//cerr << "writing prevData to buffer" << endl;

		prevData->insert(data);
	}
	
	//Vector<BaseFloat> vec_data(*data);
	//prevData = new Vector<BaseFloat>(*data);
	
	//prevData->CopyFromVec(*data);
	//ostringstream ss;
	//ss.clear();
	//ss << "storing prev data " << prevData->Dim();
	//my_log(ss.str());
	prevRobotSpeaking = robotSpeaking;
	//my_log("Storing prevData");
	
}

void SFController::refresh_date() {
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	char buffer[80];
	strftime(buffer,80,"%d_%m_%Y_%H_%M_%S",now);
	date_string = new string(buffer);
	char daybuf[80];
	strftime(daybuf,80,"%d_%m_%Y", now);
	day_string = new string(daybuf);
}

void SFController::input_finished() {
	//cout << "input finished"<<endl;
	decoder->input_finished_func();
	decoder->stop();
	//listener -> input_finished_func();
	//listener -> stop();
}

void SFController::add_chunk(int size) {
	//cout << "adding chunk" << endl;
	output->add_chunk(size);
}

std::string SFController::get_date_string() {
	return *date_string;
}

std::string SFController::get_day_string() {
	return *day_string;
}

void SFController::startDecoder() {
	//cout << "SFController startDecoder" << endl;
	//cout << "type" << gen_config->decode_type << endl;
	if (!strcmp(gen_config->decode_type, "nnet")) {
		//cout << "About to open the SFNnetDecoder" << endl;
//		decoder = new SFNnetDecoder(this, (SFNnetConfig*)decode_config, gen_config->keyphrase);
		decoder = new SFNnetThreadedDecoder(this, (SFNnetThreadedConfig*)decode_config, gen_config->keyphrase);
	
	}
	else if (!strcmp(gen_config->decode_type, "gmm")) {
		decoder = new SFGmmDecoder(this, (SFGmmConfig*)decode_config, gen_config->keyphrase);
		//cout << "GMM not implemented yet" << endl;
	}
	else {
		//cout << "Wrong decoder type, must be nnet or gmm" << endl;
	}
	/*if (!strcmp(startListening, "true")) {
		decoder->initListen(true);
}
	else {
		decoder->initListen(false);
}*/


	decoder->start();
}


void SFController::startSource() {
	//cout << "Starting source" << endl;
	if (!strcmp(gen_config->source, "wav")) {
		//cout << "Starting WAV source " << endl;
		src = new WavDataSource(gen_config->utt, this);
	}
	else if (!strcmp(gen_config->source, "pa")) {
		src = new PortAudioDataSource(this, gen_config->output_path, gen_config->record_wav);
	}
	else {
		my_log("WRONG DATA SOURCE, must be PA or WAV");
		//cerr << "WRONG DATA SOURCE, must be PA or WAV" << endl;
	}
	src -> start();
}


std::vector<std::string>& SFController::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

void SFController::waitForFinish() {
	pthread_join( *controller_thread, NULL);
}

std::vector<std::string> SFController::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

bool SFController::startswith(string prefix, string str) {
	return str.substr(0, prefix.size()) == prefix;
}


void SFController::waitForDecoderFinish() {
	decoder -> waitForFinish();
}

void SFController::waitForSourceFinish() {
	src -> waitForFinish();
}
void SFController::readConfig() {

	//for (int i=0;i<argc;++i) {
	//cout << argv[i] << endl;
	//}

	gen_config = new SFGenericConfig(argc, argv);

	if (!strcmp(gen_config->decode_type, "nnet")) {
		//cerr << "Loading NNET config " << NUM_EXTRA_ARGS << endl;
		my_log("Loading NNET config " + NUM_EXTRA_ARGS);
		//decode_config = new SFNnetConfig(argc - NUM_EXTRA_ARGS, argv);
		decode_config = new SFNnetThreadedConfig(argc - NUM_EXTRA_ARGS, argv);
	
	}
	else {
		
		my_log("Loading GMM config " + NUM_EXTRA_ARGS); 
	//	cerr << "Loading GMM config " << NUM_EXTRA_ARGS << endl;
		decode_config = new SFGmmConfig(argc - NUM_EXTRA_ARGS, argv);

	}

}

void* SFController::startStatic(void* arg) {
	((SFController*)arg)->start_impl();
	return NULL;
}

void SFController::start() {

	int iret = pthread_create( controller_thread, NULL, SFController::startStatic, this);
	if(iret)
	{
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
		exit(EXIT_FAILURE);
	}
	//cout << "start" << endl;
}

int diff_ms(timeval t1, timeval t2)
{
    return (((t1.tv_sec - t2.tv_sec) * 1000000) + 
            (t1.tv_usec - t2.tv_usec))/1000;
}

void SFController::start_impl() {


	while (true) {
		char buf[255];
		//cerr << "Waiting to read from YARP" << endl;
		int size = read(read_pipe, buf, sizeof(buf));
		std::string str(buf, size-1);
		//cerr << "'" << str << "'" << endl;
		my_log("Yarp message "+str+" received");
		vector<string> splits = split(str, ' ');
		string key = splits[0];

		if (key=="speech_event") {
			string str = splits[1];
			if (str=="start_of_speech") {
				//cerr << "robot start speaking" << endl;
				this->robotSpeaking = true;
				//decoder -> shouldListen(true);
				//if (asrEnabled) {
				//	decoder -> setRobotSpeaking(true);
				//}
			}
			//			else if (str=="robotSpeakEnd") {
			//else if (str=="end_of_speech") {
			else if (str=="SPEECH_END") {
				
				//cerr << "robot end speaking" << endl;
				this->robotSpeaking = false;
				
				//if (asrEnabled) {
				//	decoder -> setRobotSpeaking(false);
				//}
				//	timeval cur_time;
				//gettimeofday(&cur_time, NULL);
				//int diff = diff_ms(cur_time, prev_event_time);
				//cerr << "Event time difference = " << diff << endl;
			
				//decoder -> shouldListen(false);
			}
			//prev_event = str;
			//gettimeofday(&prev_event_time, NULL);
		}
		else if (asrEnabled && key=="lm_change") {

			//string type = splits[1];
			//int index = atoi(splits[2].c_str());
			my_log("ignoring LM");
			//cerr << "lm_change " << type << " " << splits[2] << endl;
			//decoder -> setLM(type, index);

		}
		else if (key=="speech_override") {
			string str = splits[1];
			if (str=="start_listen") {
				//cerr << "override start listen" << endl;
				this -> should_listen = true;
				//this->robotSpeaking = true;
				//decoder -> shouldListen(true);
				//decoder -> setRobotSpeaking(true);
			}
			else if (str=="end_listen") {
				//cerr << "override end listen" << endl;
				//decoder -> setRobotSpeaking(false);
				if (!this->always_listen) {
					this->should_listen = false;
				}
				//decoder -> shouldListen(false);
			}
			else if (str=="always_listen") {
				//cerr << "override end listen" << endl;
				//decoder -> setRobotSpeaking(false);
				this->always_listen = true;
				this->should_listen = true;
				//decoder -> shouldListen(false);
			}
			else if (str=="timeout") {
				//decoder -> setLMReady(false);
				//this->should_listen = false;
				if (asrEnabled) {
					decoder -> timeout();
				}
			}
		}

		else if (key=="button_control") {
			string button = splits[1];
			//cerr << "button_control " << button << endl;
			if (button=="new_person") {
				asrEnabled = true;
				//cerr << "init for new person" << endl;
				should_listen = false;
				refresh_date();
				openOutput();
				this -> sample_count = 0;
				startDecoder();
				startSource();

			}
			
			if (button=="new_woz" || button=="baby_woz") {
				// need to ignore messages like LM etc...
				should_listen = false;
				asrEnabled = false;
				refresh_date();
				openOutput();
				this->sample_count = 0;
				startSource();
			}
			if (button=="start_test") {
				asrEnabled = true;
				should_listen = true;
				refresh_date();
				openOutput();
				startDecoder();
				startSource();
				my_log("test");
				//cerr << "test" << endl;
			}
			if (button=="stop_test") {
				should_listen = false;
				src->stop();
				
				decoder->stop();
				//decoder->setLM("dialogue", 0);
			}
			if (button=="terminate") {
				//cerr << "terminate received" << endl;
				src->stop();
				if (asrEnabled) {
					decoder->stop();
				}
			}


		}




		else if (key=="terminate") {
			terminate();
		}


		//decoder -> sendMessage(str);
		usleep(1000);
		//usleep(100000);
	}

}



void SFController::destroy() {
	//cout << "Doing destroy "<<endl;
	decode_config -> destroy();
}

void SFController::openOutput() {
	//cout << "out path " << gen_config -> output_path << endl;

	//cout << "base name " << gen_config -> base_name << endl;
	output = new SFOutput(this, gen_config->source, gen_config->base_name, gen_config->output_path);
}

string SFController::output_string(fst::SymbolTable *word_syms, CompactLattice clat, int64* num_frames, double* tot_like) {
	//cout << " orig output " << endl;
	return output -> output_string(word_syms, clat, num_frames, tot_like, sample_count);
}

void SFController::handle(int s) {

	SFController::instance -> terminate();
	printf("Caught signal %d\n",s);
	//exit(1);
}

void SFController::terminate() {
	src -> stop();
	if (asrEnabled) {
		decoder -> stop();
	}
	my_log("Terminated successfully");
	//cout << "Terminated successfully" << endl;
}

void SFController::simStart() {
	asrEnabled = true;
	should_listen = true;
	robotSpeaking = false;
	always_listen = true;
	refresh_date();
	openOutput();
	startDecoder();
	//decoder->setLM("dialogue", 0);
	startSource();
	waitForSourceFinish();
	waitForDecoderFinish();
}
}




int main(int argc, char *argv[]) {
	using namespace kaldi;
	using namespace std;


	//cout << "Starting my-online-controller" << endl;
	//int read_pipe = atoi(argv[0]); // the other process write

	//cerr << "Starting SF controller. This process reads input from the pipe, to allow interactively controlling the recogniser." << endl;
	
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = SFController::handle;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);

	SFController* controller = new SFController(argc, argv);
	SFController::instance = controller;

	// we have either received input from NNet or GMM decoder script.
	// we need to do different steps depending on which
	controller->my_log("Starting SF controller main");
	controller->readConfig();

	controller->simStart();


	//controller->start();

	//controller->waitForFinish();

	/*

	controller->startDecoder();

	//usleep(5000000);

	controller->startSource();



	//cout << "Started source" << endl;

	controller->waitForSourceFinish();
	controller->waitForDecoderFinish();
	*/

	//cout << "Got to here!" << endl;

	//controller->destroy();

	//cout << "sf-controller.cc Starting my-nnet-controller" << endl;

	//int read_pipe = atoi(argv[0]); // the other process write


	//exit;

	//char* startListening = argv[argc-1];
	//controller = new MyNnetController(argc, argv);


	/*pthread_t decoder_thread, controller_thread;
	int iret = pthread_create( &decoder_thread, NULL, start_decoder, NULL);
	if(iret)
{
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
		exit(EXIT_FAILURE);
}*/
	/*iret = pthread_create( &controller_thread, NULL, start_controller, NULL);
	if(iret)
{
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
		exit(EXIT_FAILURE);
}*/

	//pthread_join( decoder_thread, NULL);
	//pthread_join( controller_thread, NULL);


	return 0;
}