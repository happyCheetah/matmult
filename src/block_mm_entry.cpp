#include "block.h"

//top lvl module. 
//note given no host code, skip host related step in compiling process

int n = 8;
void top(blockvec A[], blockvec B[],blockvec C[]){
	//Assume C is buffered on-chip
	#pragma HLS INTERFACE bram port=C storage_type=ram_2p
	//Put DDR interfacing directives for A & B
	// #pragma HLS INTERFACE m_axi port=A bundle=gmem0 offset=slave
	// #pragma HLS INTERFACE m_axi port=B bundle=gmem1 offset=slave
	// #pragma HLS INTERFACE s_axilite port=A bundle=control
	// #pragma HLS INTERFACE s_axilite port=B bundle=control
	// #pragma HLS INTERFACE s_axilite port=return bundle=control
	// #pragma HLS aggregate variable=C
	//Stream<blockvec> pipe[2];
	hls::stream<blockvec> pipe[2];
	#pragma HLS STREAM variable=pipe depth=8
	//pipe[0] and [1] are fifos used to chain together provider(load_DDR) and consumer(blockmatmul)
	//pipe[0] for Arows, pipe[1] for Bcols

	blockvec C_onchip[SIZE];
	
	//"it" is a counter to keep track of the tile/block indices in different kernel calls
	for (int it=0;it<n;it++){
		#pragma HLS DATAFLOW
		//call loadDDR and blockmatmul functions chained with fifos in the pipe array
		// *** your code here *** //
		loadDDR(A, B, pipe[0], pipe[1],SIZE);
		//blockmatmul(pipe[0], pipe[1], C_onchip, SIZE);
		blockmatmul(A, B, C_onchip, SIZE);
		
	}
// Final goal:
	// for (int it=0;it<n;it++){
	// 	#pragma HLS DATAFLOW
	// 	loadDDR(A, B pipe_A, buffer_B, it);
	// 	loadDDR(D, pipe_D, SIZE2);
	// 	blockmatmul(pipe_A, buffer_B, buffer_C, it);
	//	blockmatmul(pipe_D, buffer_C, buffer_E, it);
	// 	writeDDR(buffer_E, E);
	// }

	
}


