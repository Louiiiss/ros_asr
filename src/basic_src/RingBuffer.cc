//#include "tmp.h"
#include <iostream>
#include "my-online.h"

namespace kaldi {


RingBuffer::RingBuffer(int length) {
	buffer_len = length;
//	idx=0;
}
void RingBuffer::insert(Vector<BaseFloat>* foo) {
	//cout << "Inserting " << foo << endl;
	//arr[idx++] = foo;
	
//	Vector<BaseFloat> copy(*foo);
	arr.push_back(*foo);
	if (arr.size()>buffer_len) {
		arr.pop_front();
	}
//	idx=0;
//	idx+=1;
//	idx%=buffer_len;
}

Vector<BaseFloat>* RingBuffer::read(int i) {
	//Vector<BaseFloat> foo = arr[idx];
//	idx=(idx+1)%buffer_len;
	Vector<BaseFloat>* foo = &arr[i];
//	arr.pop_front();
	return foo;
}

int RingBuffer::length() {

	return buffer_len;
}

MatrixIndexT RingBuffer::totalSize() {
	MatrixIndexT total = 0;
	for (int i=0;i<buffer_len;++i) {
		Vector<BaseFloat>* p = &arr[i];
		total+=p->Dim();
	}
	return total;
}

}





