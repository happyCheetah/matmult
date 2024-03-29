#include "block.h"

void loadA(blockvec A[],  hls::stream<blockvec> &Arows, int it){
	//identify row-tile index in A
	//read the tile from A[] in DDR to Arows
	for (int i = 0; i < SIZE; i++){
		#pragma HLS PIPELINE
		Arows.write(A[i]);
	}
}


// loadB with buffer
void loadB(blockvec B[], blockvec B_buffer[], int it){
	for (int i = 0; i < SIZE; i++){
		#pragma HLS PIPELINE
		B_buffer[i] = B[i];
	}
}

// ORIGINAL loadB
////lead into blockvec B_buffer[]
//void loadB(blockvec B[], hls::stream<blockvec> &Bcols, int it){
//	//identify column-tile index in B
//	int B_tile_index = it%(SIZE/BLOCK_SIZE);
//	//read the tile from B[] in DDR to Bcols
//	for (int i = 0; i < SIZE; i++){
//		#pragma HLS PIPELINE
//		Bcols.write(B[i]);
//	}
//}	
void loadDDR(blockvec A[], blockvec B[], hls::stream<blockvec> &Arows, blockvec &Bcols[], int it){
	//Assumption 1: Arows and Bcols are matrix tiles of size SIZE*BLOCK_SIZE(e.g. blockvec size) that are on-chip
	//Assumption 2: A and B are blockvec arrays both stored in row-major order
	#pragma HLS aggregate variable=A
	#pragma HLS aggregate variable=B

	//loadA(A, Arows, it);
	//loadB(B, Bcols, it);
	for (int i=0; i<SIZE; i++){
		#pragma HLS PIPELINE
		blockvec arow = A[i];
		Arows.write(arow);
		B_buffer[i] = B[i];
	}
}

// void blockmatmul(hls::stream<blockvec> &Arows, blockvec Bcols[], blockmat & C, int it) {
void blockmatmul(hls::stream<blockvec> &Arows[], blockvec Bcols[], blockvec C[], int it) {
#pragma HLS aggregate variable=C
	//Fill in the code for outer product between a row-tile of A and a column-tile of B to produce a blockmat of C
	//n means row index in Arows
	//m means col index in bcols 
	
	blockvec row, col;

	for(int n=0;n<SIZE;n++ ){
		row = Arows.read();
		for (int k=0;k<SIZE;k++){
		#pragma HLS PIPELINE
			//Fully unrolled loop 
			for(int m=0;m<SIZE;m++){
			#pragma HLS unroll
				col = Bcols[m];
				//C.out([n*SIZE+m]) = Arows[n*SIZE+k] * Bcols[k*SIZE+m];
				// C.out[n*SIZE+m] += row.a[k] * col.a[k];
				C[n].a[m] += row.a[k] * col.a[k];
			}
		}
	}
	//read a blockvec from column-tile of B when needed using "Bcols.read()"
	//You can use "#pragma HLS dependence variable=C inter false" to resolve false dependency
}


