#include "Arduino.h"
#include <stdio.h>
#include <util/delay.h>

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
}

#define RINGBUFF_SIZE 1024 * 7
#define SERIAL_READ_SIZE 64

int readChunk(uint8_t *buffer, uint32_t *dataOffset);

char log_buffer[100];

void loop() {
	Serial.begin(1000000);

	uint32_t startTime = 0;
	uint32_t times = 0;
	uint32_t sumTime = 0;
	int bytesRead = 0;
	uint32_t sumBytesRead = 0;
	// ringbuffer
	uint8_t buffer[RINGBUFF_SIZE];
	uint32_t dataOffset = 0;
	//uint32_t readOffset = 0;

	if (Serial.available() > 0) {
		while (true) {
			startTime = micros();
			bytesRead = readChunk(buffer, &dataOffset);
			if (times < 10000) {
				times++;
				sumTime += micros() - startTime;
				sumBytesRead += bytesRead;
			} else {
				if (times != 999999) {
					sprintf(log_buffer,
							"times: %lu average time: %lu average read: %lu dataOffset: %lu\n",
							times, sumTime / times, sumBytesRead / times,
							dataOffset);
					Serial.print(log_buffer);
					times = 0;
					sumBytesRead = 0;
					sumTime = 0;
				}
			}
			/*if (dataOffset >= RINGBUFF_SIZE - SERIAL_READ_SIZE) {
			 sprintf(log_buffer, "buffer full: %lu\n", dataOffset);
			 Serial.print(log_buffer);
			 return;
			 }*/
		}
	}
}

int readChunk(uint8_t *buffer, uint32_t *dataOffset) {
	int bytesRead = 0;
	int bytesUntilOverflow = RINGBUFF_SIZE - *dataOffset;
	if (bytesUntilOverflow < SERIAL_READ_SIZE) {
		uint8_t tmpBuffer[64];
		bytesRead = Serial.readBytes((char *) tmpBuffer, SERIAL_READ_SIZE);
		if (bytesRead > bytesUntilOverflow) {
			memcpy(buffer + *dataOffset, tmpBuffer, bytesUntilOverflow);
			*dataOffset = bytesRead - bytesUntilOverflow;
			memcpy(buffer, tmpBuffer + bytesUntilOverflow, *dataOffset);

		} else {
			memcpy(buffer + *dataOffset, tmpBuffer, bytesRead);
			*dataOffset += bytesRead;
		}
	} else {
		bytesRead = Serial.readBytes((char *) buffer + *dataOffset,
		SERIAL_READ_SIZE);
		*dataOffset += bytesRead;
	}
	return bytesRead;
}
