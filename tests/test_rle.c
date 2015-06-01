#include <rle.h>
#include <stdio.h>

int main(){

	FILE *wFileIn;
	FILE *wFileOut;
	FILE *wFileOut_tmp;

	wFileIn = fopen("data/blanc.bmp", "rb");
	wFileOut_tmp = fopen("data/blanc.rle", "wb");
	wFileOut = fopen("data/blanc_res.bmp", "wb");
	
	coderRle(wFileIn, wFileOut_tmp);
	fclose(wFileOut_tmp);
	wFileOut_tmp = fopen("data/blanc.rle", "rb");
	decoderRle(wFileOut_tmp, wFileOut);
	
	fclose(wFileIn);
	fclose(wFileOut_tmp);
	fclose(wFileOut);
	
	return 0;
}
