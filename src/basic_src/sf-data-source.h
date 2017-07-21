

#include "feat/wave-reader.h"
#include "online2/online-feature-pipeline.h"
#include "online2/online-gmm-decoding.h"
#include "online2/onlinebin-util.h"
#include "online2/online-timing.h"
#include "online2/online-endpoint.h"
#include "fstext/fstext-lib.h"
#include "lat/lattice-functions.h"
#include "online-audio-source.h"



namespace kaldi {
    //class MyOnlineDecoder;
    //class GenericDecoder;


    class SFController;

    /**
     * The base class for audio data sources.
     */
    class DataSource {
        friend class SFController;
    public:
        /**
         * Start reading from the source, and if enabled record to file
         */
        virtual void start() = 0;

        /**
         * 
         * Stop reading/recording from the source.
         */
        virtual void stop() {
            active = false;
        }

        /**
         * Convenience function, waits for the source thread to finish
         * calls pthread_join
         */
        void waitForFinish();

    protected:
        //GenericDecoder* decoder;
        //MyOnlineDecoder *decoder;
        SFController* controller;
        volatile bool active;
        pthread_t* src_thread;
    };


    // note : the wav data source is intended to simulate a real, live
    // audio source. Therefore we split into chunks and send, to see
    // if the decoder can cope.

    /**
     * This data source reads from WAV files. For compatibility with Kaldi, we use
     * a wav.scp file, and the WAV file is specified by the utterance id.
     * 
     * This also reads in chunk lengths from file, enabling the class to replicate
     * a previous run exactly.
     * 
     * @param utt The utterance id
     * @param controller The controller instance
     */
    class WavDataSource : public DataSource {
    public:
        //	WavDataSource(const char* basename, string wav_rspecifier, string utt, MyOnlineDecoder *decoder, const char* output_path);
        WavDataSource(string utt, SFController *controller);

        /**
         * Implementation of start
         */
        void start();

        /**
         * Convenience static function for starting thread 
         * 
         * @param arg The instance of the WavDataSource to start
         * @return NULL
         */
        static void* startStatic(void* arg);

        /**
         * Stop the data source reading/recording thread
         */
        void stop();
    private:
        void start_impl();
        void read_chunk_lengths(const char* base_name, const char* platform);
        RandomAccessTableReader<WaveHolder>* wav_reader;
        string* wav_rspecifier;
        string utt;
        std::queue<int> chunk_lengths;
    };

    /*struct PortAudioConfig {
            int32 kTimeout; // half second
            // Input sampling frequency is fixed to 16KHz
            int32 kSampleFreq;
            // PortAudio's internal ring buffer size in bytes
            int32 kPaRingSize;
            // Report interval for PortAudio buffer overflows in number of feat. batches
            int32 kPaReportInt;
    };*/

    /**
     * This data source reads microphone input from PortAudio.
     * 
     * @param controller The controller instance
     * @param output_path Where to put recorded audio (if enabled)
     * @param record_wav Whether or not the audio should be recorded
     */
    class PortAudioDataSource : public DataSource {
    public:
        //	PortAudioDataSource(MyOnlineDecoder *decoder, const char* output_path, const char* record_wav);
        PortAudioDataSource(SFController *controller, const char* output_path, const char* record_wav);
        /**
         * Start reading / recording
         */
        void start();

        /**
         * Static function used to start the thread
         * @param arg The instance to run
         * @return NULL
         */
        static void* startStatic(void* arg);
        /**
         * Stop reading/recording
         */
        void stop();
    private:
        void start_impl();
        OnlinePaSource *au_src;
        const char* output_path;
        const static int32 kTimeout = 50; // half second
        // Input sampling frequency is fixed to 16KHz
        const static int32 kSampleFreq = 16000;
        // PortAudio's internal ring buffer size in bytes
        const static int32 kPaRingSize = 32768;
        // Report interval for PortAudio buffer overflows in number of feat. batches
        const static int32 kPaReportInt = 4;
        string record_wav;
    };


}


