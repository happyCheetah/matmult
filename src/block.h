
#include "hls_stream.h"
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

const int SIZE = 32;
const int BLOCK_SIZE = 32;

typedef struct {
	int a[BLOCK_SIZE]; 
} blockvec;

typedef struct {
	int out[BLOCK_SIZE][BLOCK_SIZE]; //one output block of matrix C
} blockmat;

void loadDDR(blockvec A[], blockvec B[], hls::stream<blockvec> &Arows, hls::stream<blockvec> &Bcols, int it);
void blockmatmul(hls::stream<blockvec> &Arows, hls::stream<blockvec> &Bcols,
blockmat & ABpartial, int it);
