#include "block.h"

void top(blockvec A[], blockvec B[], blockmat C){
	//Assume C is buffered on-chip
	#pragma HLS INTERFACE bram port=C storage_type=ram_2p
	//Put DDR interfacing directives for A & B
	#pragma HLS INTERFACE m_axi port=A bundle=gmem0 offset=slave
	#pragma HLS INTERFACE m_axi port=B bundle=gmem1 offset=slave
	#pragma HLS INTERFACE s_axilite port=A bundle=control
	#pragma HLS INTERFACE s_axilite port=B bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control
	#pragma HLS aggregate variable=C
	#pragma HLS DATAFLOW
	hls::stream<blockvec> pipe[2];
	#pragma HLS STREAM variable=pipe depth=8
	//pipe[0] and [1] are fifos used to chain together provider(load_DDR) and consumer(blockmatmul)
	//pipe[0] for Arows, pipe[1] for Bcols

	//"it" is a counter to keep track of the tile/block indices in different kernel calls
	//it = tracks what sub block we are currently on 

	//
	//blockmat C[SIZE*SIZE/(BLOCK_SIZE*BLOCK_SIZE)];
	for (int it=0;it<SIZE*SIZE/(BLOCK_SIZE*BLOCK_SIZE);it++){
		#pragma HLS DATAFLOW
		//call loadDDR and blockmatmul functions chained with fifos in the pipe array
		// *** your code here *** //
		loadDDR(A, B, pipe[0], pipe[1], it);
		blockmatmul(pipe[0], pipe[1], C, it);
	}

	//notes: 
}


 
