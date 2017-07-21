//#include "tmp.h"
#include <iostream>

namespace kaldi {

    /**
     * Used for storing buffer of previous audio in case we want to read back
     */
class RingBuffer {
protected:
	int buffer_len;
	std::deque<Vector<BaseFloat> > arr;

public:
    /**
     * Constructor with the length of the buffer required
     * @param length Length in chunks
     */
	RingBuffer(int length);
        /**
         * Add a new chunk of data
         * @param foo The data to store
         */
	void insert(Vector<BaseFloat>* foo);
        
        /**
         * Read the data at a given index
         * @param i The index to read
         * @return The data at that index
         */
	Vector<BaseFloat>* read(int i);
	
        /**
         * The length of the data in chunks
         * @return The length
         */
	int length();
        
        /**
         * The total size of all the chunks (in samples)
         * @return The total size
         */
	MatrixIndexT totalSize();

};
}




