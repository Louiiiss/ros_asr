
#include <string>
#include <fstream>  

namespace kaldi {

class SFOutput {
	friend class SFController;
public:
	//MyOnlineASROutput(MyOnlineDecoder *decoder, char* source, const char* base_name, const char* platform);
/**
 * Constructor for output
 * @param controller Controller instance
 * @param source Whether the source is WAV or microphone
 * @param base_name Base filename for output
 * @param output_path The path for output/log files
 */
    SFOutput(SFController *controller, char* source, const char* base_name, const char* output_path);

    /**
     * Record a new chunk size
     * @param size Size of chunk
     */
	void add_chunk(int size);
        
        /**
         * Output most likely string given a probability lattice and word symbol table
         * @param word_syms Word symbol table
         * @param clat Probability lattice
         * @param num_frames Number of frames
         * @param tot_like Total likelihood
         * @param num_samples Number of samples
         * @return 
         */
	string output_string(fst::SymbolTable *word_syms, CompactLattice clat, int64* num_frames, double* tot_like, MatrixIndexT num_samples);
	
        /**
         * Log a new segment
         * @param start The start index of the segment (number of samples)
         * @param end End index (number of samples)
         * @param val Whether we are listening or not
         */
        void write_segment(MatrixIndexT start, MatrixIndexT end, bool val);
	//void write_cut_prev(int cut);
        /**
         * If we read back into the buffer
         * @param size Size of the readback
         */
	void write_readback(MatrixIndexT size);
protected:
	//MyOnlineDecoder* decoder;
	SFController* controller;

	std::fstream out_file;
	std::fstream controls;
	std::fstream chunk_lengths;
	struct timeval start;
};

}



