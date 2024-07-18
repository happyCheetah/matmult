#include "block.h"

//complete
void loadA(blockvec A[],  hls::stream<blockvec> &Arows, int it){
	//identify row-tile index in A
	int A_tile_index = int(it/(SIZE/BLOCK_SIZE));
	//read the tile from A[] in DDR to Arows
	for (int i = 0; i < SIZE; i++){
		#pragma HLS PIPELINE
		//Arows.write(A[/* *** your code here *** */]);
		Arows.write(A[i]);
	}
}

//complete
void loadB(blockvec B[], hls::stream<blockvec> &Bcols, int it){
	//identify column-tile index in B
	int B_tile_index = it%(SIZE/BLOCK_SIZE);
	//read the tile from B[] in DDR to Bcols
	for (int i = 0; i < SIZE; i++){
		#pragma HLS PIPELINE
		//Bcols.write(B[/* *** your code here *** */]);
		Bcols.write(B[i]);
	}
}

//complete
void loadDDR(blockvec A[], blockvec B[], hls::stream<blockvec> &Arows, hls::stream<blockvec> &Bcols, int it){
	//Assumption 1: Arows and Bcols are matrix tiles of size SIZE*BLOCK_SIZE(e.g. blockvec size) that are on-chip
	//Assumption 2: A and B are blockvec arrays both stored in row-major order
	#pragma HLS aggregate variable=A
	#pragma HLS aggregate variable=B

	loadA(A, Arows, it);
	loadB(B, Bcols, it);
}

void blockmatmul(hls::stream<blockvec> &Arows, hls::stream<blockvec> &Bcols, blockmat & ABpartial, int it) {
#pragma HLS aggregate variable=C
	//Fill in the code for outer product between a row-tile of A and a column-tile of B to produce a blockmat of C
	//pipeline the outer loop and fully unroll inner loops
	//read a blockvec from column-tile of B when needed using "Bcols.read()"
	//You can use "#pragma HLS dependence variable=C inter false" to resolve false dependency

	

	int counter = it % (SIZE/BLOCK_SIZE);
	static int A_buffer[BLOCK_SIZE][SIZE];

	if (counter==0){
		loadA: for (int i = 0; i < SIZE; i++){
			//tempA contains a[block_size]
			blockvec tempA = Arows.read();
			for (int j = 0; j < BLOCKSIZE; j++){
				A_buffer[j][i] = tempA.a[j]; 
			}
		}
	}

	int AB[BLOCK_SIZE][BLOCK_SIZE] = {0};
	partialsum: for(int k = 0; k < SIZE; k++){
		blockvec tempB = Bcols.read(); 
		for(int i = 0; i < BLOCK_SIZE; i++){
			for(int j = 0; j < BLOCK_SIZE; j++){
				AB[i][j] = AB[i][j] + A_buffer[i][k] * tempB.a[j];
			}
		}
	}

	output: for(int i = 0; i < BLOCK_SIZE; i++){
		for(int j = 0; j < BLOCK_SIZE; j++){
			ABpartial.out[i][j] = AB[i][j]; 
		}
	}
}


