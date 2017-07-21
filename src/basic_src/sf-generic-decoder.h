#include "feat/wave-reader.h"
#include "online2/online-feature-pipeline.h"
#include "online2/online-gmm-decoding.h"
#include "online2/onlinebin-util.h"
#include "online2/online-timing.h"
#include "online2/online-endpoint.h"
#include "fstext/fstext-lib.h"
#include "lat/lattice-functions.h"
#include <queue>
#include "online/online-audio-source.h"
#include <write_wav.h>

namespace kaldi {
class SFController;
class FSTSwitcher;

/**
 * Base class for decoders.
 */
class SFGenericDecoder {
public:
	//GenericListener(GenericDecoder *decoder, const char* keyphrase);
	// receives some data from the data source, and decodes it
	/**
         * Initialisation done here
         * @param keyphrase Keyphrase to start adaptation
         */
	virtual void init(const char* keyphrase);
        /**
         * Start decoding
         */
	virtual void start();
        
        /**
         * Stop decoding
         */
	virtual void stop();
        /**
         * This is used to read back from the data stream once the start_listen
         * message has been received - we use this because of the delay from the
         * robot end-of-speech signal
         * @param prevData The RingBuffer storing the last few seconds of audio
         */
	virtual void hearPrev(RingBuffer* prevData);
	
	//virtual void hearEnd(SubVector<BaseFloat> *data);
        
        /**
         * Hear (i.e. add to the decoder input) a chunk of data.
         * @param data The data to add to the decoder input
         */
	virtual void hear(Vector<BaseFloat>* data);
        /**
         * Called when the input is finished
         */
	virtual void input_finished_func();
        
     
        
        /**
         * Set whether the LM is ready for use
         * @param val True iff the LM is ready
         */
	void setLMReady(bool val);
        
        /**
         * Waits for the thread to finish - calls pthread_join
         */
	void waitForFinish();
        /**
         * Called on timeout to allow clean exit
         */
	virtual void timeout() = 0;
        /**
         * Static start function
         * @param arg The instance to start
         */
	static void* startStatic(void* arg);
        
        /**
         * Set the LM using type and index (e.g. questions 2)
         * @param type Type of LM e.g. dialogue, questions, control
         * @param index Numeric index of LM
         */
	//void setLM(string type, int index);
        
        /**
         * Whether or not the LM is ready to run
         */
	//volatile bool lm_ready;
	
protected:
	void start_impl();
	virtual void decode(Vector<BaseFloat>* data) = 0;
	std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace);
	SFController* controller;
	//SingleUtteranceGmmDecoder* utt_gmm_decoder;
	volatile bool fresh_utterance;
	//OnlineGmmAdaptationState adaptation_state;
	//MyOnlineConfig* config;
	volatile bool active;
	volatile bool input_finished;
	volatile bool keyphrase_activated;
	//volatile bool robotSpeaking;
	volatile bool decode_lock;
	WAV_Writer writer;
	//FSTSwitcher* fst_switch;
	//fst::Fst<fst::StdArc> *fst;
	//fst::SymbolTable *word_syms;
	pthread_t* decoder_thread;
	string keyphrase;
	string phase;
	//int question_no;
	std::queue<Vector <BaseFloat>* > *data_to_process;
};

}
