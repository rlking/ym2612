#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define BUFF_SIZE 1024

int main(void) {

	FILE *f = fopen("C:\\Users\\rlking\\island.vgm", "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	//fseek(f, 0, SEEK_SET);  //same as rewind(f);
	fseek(f, 64, SEEK_SET); // ignore header

	unsigned char *music = malloc(fsize + 1);
	fread(music, fsize, 1, f);
	fclose(f);

	uint32_t nSamples = 0;
	for (int i = 0; i < fsize; i++) {

		if (music[i] == 0x52) {
			//write_data(music[i + 1], music[i + 2], PORT_0);
			printf("PORT_0: %x %x\n", music[i + 1], music[i + 2]);
			i += 2;
		} else if (music[i] == 0x53) {
			//write_data(music[i + 1], music[i + 2], PORT_1);
			printf("PORT_1: %x %x\n", music[i + 1], music[i + 2]);
			i += 2;
		} //0x50	dd	PSG (SN76489/SN76496) write value dd
		else if (music[i] == 0x50) {
			// skip
			i += 1;
		} //0x61	nn nn	Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
		else if (music[i] == 0x61) {
			//All integer values are *unsigned* and written in "Intel" byte order (Little Endian), so for example 0x12345678 is written as 0x78 0x56 0x34 0x12.
			uint16_t wait_n = music[i + 1] << 0 | (music[i + 2] << 8);
			printf("wait 0x61 samples: %d ys: %d\n", wait_n, (wait_n * 1000000)/ 44100);
			nSamples += wait_n;
			//Sleep((wait_n * 1000)/ 44100);
			i += 2;
		} else if (music[i] >= 0x70 && music[i] <= 0x7f) {
			uint16_t samp = music[i] & 0x0f;
			printf("wait 0x7X samples: %d ys: %d\n", samp, (samp * 1000000)/ 44100);
			nSamples += samp;
			//Sleep((samp * 1000)/ 44100);
		} else if (music[i] == 0x66) {
			break;
		} else {
			printf("unbekanntes zeichen %x   pos:%d\n", music[i], i);
		}

	}
	printf("samples: %d ms: %d", nSamples, (nSamples * 1000) / 44100);

	return 0;
}
