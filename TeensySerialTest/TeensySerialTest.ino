#include "Arduino.h"
#include <stdio.h>
#include <util/delay.h>
#include "VgmPlayer.h"

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
}

#define RINGBUFF_SIZE 1024 * 6
#define SERIAL_READ_SIZE 64

int readChunk();
uint8_t readByte();
void readBytes(int n);

uint8_t buffer[32];
char log_buffer[100];

// ringbuffer
uint8_t rbuff[RINGBUFF_SIZE];
uint32_t dataOffset = 0;
uint32_t readOffset = 0;
uint32_t count = 0;

void loop() {
	Serial.begin(1000000);

	//init
	YM_CTRL_DDR |=
	_BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD) | _BV(YM_A0) | _BV(YM_A1);
	YM_DATA_DDR = 0xff;

	YM_CTRL_PORT |= _BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD); /* IC, CS, WR and RD HIGH by default */
	YM_CTRL_PORT &= ~(_BV(YM_A0) | _BV(YM_A1)); /* A0 and A1 LOW by default */

	/* Reset YM2612 */
	YM_CTRL_PORT &= ~_BV(YM_IC);
	_delay_ms(10);
	YM_CTRL_PORT |= _BV(YM_IC);
	_delay_ms(10);

	dataOffset = 0;
	readOffset = 0;
	count = 0;
	memset(&buffer[0], 0, sizeof(buffer));

	uint32_t startTime = 0;
	uint32_t startTime2 = 0;
	uint32_t times = 0;
	uint32_t sumTime = 0;
	int bytesRead = 0;
	uint32_t sumBytesRead = 0;

	uint32_t size = 0;
	uint32_t i = 0;
	uint32_t waitSamples = 0;

	if (Serial.available() == 0) {
		return;
	}
	while (count <= RINGBUFF_SIZE - SERIAL_READ_SIZE) {
		startTime = micros();
		bytesRead = readChunk();
		times++;
		sumTime += micros() - startTime;
		sumBytesRead += bytesRead;
	}
	sprintf(log_buffer, "times: %lu average time: %lu average read: %lu\n",
			times, sumTime / times, sumBytesRead / times);
	Serial.print(log_buffer);
	times = 0;
	sumBytesRead = 0;
	sumTime = 0;

	readBytes(8);
	if (buffer[0] == 'a' and buffer[1] == 'c' and buffer[2] == 'k'
			and buffer[3] == 'n') {
		size = (((int32_t) buffer[7]) & 0x000000FF)
				| ((((int32_t) buffer[6]) << 8) & 0x0000FF00)
				| ((((int32_t) buffer[5]) << 16) & 0x00FF0000)
				| ((((int32_t) buffer[4]) << 24) & 0xFF000000);
		sprintf(log_buffer, "file size: %ld", size);
		Serial.println(log_buffer);
	} else {
		sprintf(log_buffer, "no ack -> exit");
		Serial.println(log_buffer);
		return;
	}

	//ident check
	readBytes(4);
	if (!(buffer[0] == 0x56 && buffer[1] == 0x67 && buffer[2] == 0x6d
			&& buffer[3] == 0x20)) {
		sprintf(log_buffer, "ident failed!");
		Serial.println(log_buffer);
		return;
	} else {
		sprintf(log_buffer, "ident passed!");
		Serial.println(log_buffer);
	}

	//total samples
	readBytes(20);
	readBytes(4);
	const uint32_t *samples = (uint32_t *) &buffer[0];
	sprintf(log_buffer, "samples: %lu", *samples);
	Serial.println(log_buffer);

	readBytes(36);
	i = 64;
	while (i < size) {
		i++;
		if (count <= 0) {
			sprintf(log_buffer, "starved@%ld", i);
			Serial.println(log_buffer);
			Serial.flush();
			return;
		}

		uint32_t waitMicrosSec = (waitSamples * 10000) / 441;
		if (waitSamples > 0) {
			startTime = micros();
			while (startTime + waitMicrosSec > micros()) {
				if (waitSamples >= 1) {
					if (count <= RINGBUFF_SIZE - SERIAL_READ_SIZE) {
						startTime2 = micros();
						bytesRead = readChunk();
						times++;
						sumTime += micros() - startTime2;
						sumBytesRead += bytesRead;
					}
				}
			}
		}

		buffer[0] = readByte();

		//sprintf(log_buffer, " buffload: %lu i: %ld, available: %d cmd: 0x%x waitSamples: %lu ym: %lu",
		//		count, i, Serial.available(), buffer[0], waitSamples, waitMicrosSec);
		//Serial.println(log_buffer);

		waitSamples = 0;

		if (buffer[0] == 0x4F) {
			//0x4F dd    : Game Gear PSG stereo, write dd to port 0x06
			buffer[1] = readByte();
			i += 1;
		} else if (buffer[0] == 0x52) {
			//0x52 aa dd : YM2612 port 0, write value dd to register aa
			buffer[1] = readByte();
			buffer[2] = readByte();
			write_data(buffer[1], buffer[2], PORT_0);
			//System.out.printf("PORT_0: %x %x\n", music[i + 1], music[i + 2]);
			i += 2;
		} else if (buffer[0] == 0x53) {
			//0x53 aa dd : YM2612 port 1, write value dd to register aa
			buffer[1] = readByte();
			buffer[2] = readByte();
			write_data(buffer[1], buffer[2], PORT_1);
			//System.out.printf("PORT_1: %x %x\n", music[i + 1], music[i + 2]);
			i += 2;
		} else if (buffer[0] == 0x50) {
			//0x50	dd	PSG (SN76489/SN76496) write value dd
			buffer[1] = readByte();
			// skip
			i += 1;
		} else if (buffer[0] == 0x61) {
			buffer[1] = readByte();
			buffer[2] = readByte();
			waitSamples = ((buffer[1] << 0) & 0x000000FF)
					| ((buffer[2] << 8) & 0x0000FF00);
			i += 2;
		} else if (buffer[0] == 0x62) {
			//0x62		wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
			waitSamples = 735;
		} else if (buffer[0] == 0x63) {
			//0x63		wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
			waitSamples = 882;
		} else if (buffer[0] >= 0x70 && buffer[0] <= 0x7f) {
			//0x7n       : wait n+1 samples, n can range from 0 to 15
			waitSamples = 1 + (buffer[0] & 0x0f);
		} else if ((uint8_t) buffer[0] >= 0x80 && (uint8_t) buffer[0] <= 0x8f) {
			//YM2612 port 0 address 2A write from the data bank, then wait n samples;
			buffer[1] = readByte();
			write_data(0x2a, buffer[1], PORT_0);
			waitSamples = buffer[0] & 0x0f;
			i += 1;
		} else if (buffer[0] == 0x66) {
			//0x66       : end of sound data
			sprintf(log_buffer, "end of data");
			Serial.println(log_buffer);
			Serial.flush();
			break;
		} else {
			sprintf(log_buffer, "unbekanntes zeichen: 0x%x / %d pos: %ld",
					buffer[0], buffer[0], i);
			Serial.println(log_buffer);
			Serial.flush();
			return;
		}
	}
	sprintf(log_buffer, "times: %lu average time: %lu average read: %lu\n",
			times, sumTime / times, sumBytesRead / times);
	Serial.print(log_buffer);
}

uint8_t readByte() {
	uint8_t ret = rbuff[readOffset];
	count--;
	readOffset++;
	if (readOffset == RINGBUFF_SIZE) {
		readOffset = 0;
	}
	return ret;
}

void readBytes(int n) {
	for (int i = 0; i < n; i++) {
		buffer[i] = readByte();
	}
}

int readChunk() {
	int bytesRead = 0;
	int bytesUntilOverflow = RINGBUFF_SIZE - dataOffset;
	if (bytesUntilOverflow < SERIAL_READ_SIZE) {
		uint8_t tmpBuffer[64];
		bytesRead = Serial.readBytes((char *) tmpBuffer, SERIAL_READ_SIZE);
		if (bytesRead > bytesUntilOverflow) {
			memcpy(rbuff + dataOffset, tmpBuffer, bytesUntilOverflow);
			dataOffset = bytesRead - bytesUntilOverflow;
			memcpy(rbuff, tmpBuffer + bytesUntilOverflow, dataOffset);

		} else {
			memcpy(rbuff + dataOffset, tmpBuffer, bytesRead);
			dataOffset += bytesRead;
		}
	} else {
		bytesRead = Serial.readBytes((char *) rbuff + dataOffset,
		SERIAL_READ_SIZE);
		dataOffset += bytesRead;
	}
	count += bytesRead;
	return bytesRead;
}

/**
 A1 A0
 0 0 - Writes address to PORT_0
 0 1 - Writes data to the selected PART PORT_0 address
 1 0 - Writes address to PORT_1
 1 1 - Writes data to the selected PART PORT_1 address
 */

void write_data(uint8_t reg, uint8_t data, uint8_t port) {
	if (port == PORT_0) {
		YM_CTRL_PORT &= ~(_BV(YM_A1) | _BV(YM_A0)); // a1 and a0 low
	} else {
		YM_CTRL_PORT &= ~(_BV(YM_A0)); // a0 low
		YM_CTRL_PORT |= _BV(YM_A1); // a1 high
	}
	write_ym(reg);

	if (port == PORT_0) {
		YM_CTRL_PORT &= ~(_BV(YM_A1)); // a1 low
		YM_CTRL_PORT |= _BV(YM_A0); // a0 high
	} else {
		YM_CTRL_PORT |= _BV(YM_A1) | _BV(YM_A0); // a1 and a0 high
	}
	write_ym(data);
}

void write_ym(uint8_t data) {
	YM_CTRL_PORT &= ~_BV(YM_CS); // CS LOW
	YM_DATA_PORT = data;
	//_delay_us(2);
	YM_CTRL_PORT &= ~_BV(YM_WR); // Write data
	//_delay_us(5);
	YM_CTRL_PORT |= _BV(YM_WR);
	//_delay_us(5);
	YM_CTRL_PORT |= _BV(YM_CS); // CS HIGH
}

