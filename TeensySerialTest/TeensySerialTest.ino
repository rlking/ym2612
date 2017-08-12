#include "Arduino.h"
#include <stdio.h>
#include <util/delay.h>
#include "VgmPlayer.h"

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
}

#define RINGBUFF_SIZE 1024 * 7
#define SERIAL_READ_SIZE 32

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
	uint32_t startPlayback = 0;
	uint32_t times = 0;
	uint32_t times2 = 0;
	uint32_t sumTime = 0;
	uint32_t sumTime2 = 0;
	uint32_t sumSamples = 0;
	int bytesRead = 0;
	uint32_t sumBytesRead = 0;

	uint32_t samples = 0;
	uint32_t waitSamples = 0;

	if (Serial.available() == 0) {
		_delay_ms(2000);
		return;
	}
	//fill ring buffer
	while (count <= RINGBUFF_SIZE - SERIAL_READ_SIZE) {
		startTime = micros();
		bytesRead = readChunk();
		times++;
		sumTime += micros() - startTime;
		sumBytesRead += bytesRead;
		if(bytesRead == 0) {
			break;
		}
	}
	sprintf(log_buffer, "times: %lu average time: %lu average read: %lu",
			times, sumTime / times, sumBytesRead / times);
	Serial.println(log_buffer);
	times = 0;
	sumBytesRead = 0;
	sumTime = 0;

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
	memcpy(&samples, &buffer[0], sizeof(uint32_t));
	sprintf(log_buffer, "samples: %lu", samples);
	Serial.println(log_buffer);

	readBytes(36);
	startPlayback = millis();
	startTime = micros();
	while (sumSamples < samples) {
		if (count <= 3) {
			bytesRead = readChunk();
			if (bytesRead <= 0 && count == 0) {
				sprintf(log_buffer, "starved");
				Serial.println(log_buffer);
				Serial.flush();
				return;
			}
		}
		if (waitSamples > 0) {
			sumSamples += waitSamples;
			double totalTime3 = startTime + sumSamples * 22.67573696;
			while (totalTime3 > micros()) {
				if (count <= RINGBUFF_SIZE - SERIAL_READ_SIZE) {
					//startTime2 = micros();
					bytesRead = readChunk();
					//times2++;
					//sumTime2 += micros() - startTime2;
					//sumBytesRead += bytesRead;
				}
			}
		}

		buffer[0] = readByte();

		//sprintf(log_buffer, " buffload: %lu i: %ld, available: %d cmd: 0x%x waitSamples: %lu ym: %lu",
		//		count, i, Serial.available(), buffer[0], waitSamples, waitMicrosSec);
		//Serial.println(log_buffer);

		waitSamples = 0;

		//times++;
		//startTime2 = micros();
		switch (buffer[0]) {
		case 0x4F: {
			//0x4F dd    : Game Gear PSG stereo, write dd to port 0x06
			buffer[1] = readByte();
			break;
		}
		case 0x52: {
			//0x52 aa dd : YM2612 port 0, write value dd to register aa
			buffer[1] = readByte();
			buffer[2] = readByte();
			write_data(buffer[1], buffer[2], REGISTER_1);
			break;
		}
		case 0x53: {
			//0x53 aa dd : YM2612 port 1, write value dd to register aa
			buffer[1] = readByte();
			buffer[2] = readByte();
			write_data(buffer[1], buffer[2], REGISTER_2);
			break;
		}
		case 0x50: {
			//0x50	dd	PSG (SN76489/SN76496) write value dd
			buffer[1] = readByte();
			// skip
			break;
		}
		case 0x61: {
			buffer[1] = readByte();
			buffer[2] = readByte();
			waitSamples = ((buffer[1] << 0) & 0x000000FF)
					| ((buffer[2] << 8) & 0x0000FF00);
			break;
		}
		case 0x62: {
			//0x62		wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
			waitSamples = 735;
			break;
		}
		case 0x63: {
			//0x63		wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
			waitSamples = 882;
			break;
		}
		case 0x70:
		case 0x71:
		case 0x72:
		case 0x73:
		case 0x74:
		case 0x75:
		case 0x76:
		case 0x77:
		case 0x78:
		case 0x79:
		case 0x7a:
		case 0x7b:
		case 0x7c:
		case 0x7d:
		case 0x7e:
		case 0x7f: {
			//0x7n       : wait n+1 samples, n can range from 0 to 15
			waitSamples = 1 + (buffer[0] & 0x0f);
			break;
		}
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8a:
		case 0x8b:
		case 0x8c:
		case 0x8d:
		case 0x8e:
		case 0x8f: {
			//YM2612 port 0 address 2A write from the data bank, then wait n samples;
			write_data(0x2a, readByte(), REGISTER_1);
			waitSamples = buffer[0] & 0x0f;
			break;
		}
		case 0x66: {
			//0x66       : end of sound data
			sprintf(log_buffer, "end of data");
			Serial.println(log_buffer);
			Serial.flush();
			break;
		}
		default: {
			sprintf(log_buffer, "unkown command: 0x%x / %d", buffer[0],
					buffer[0]);
			Serial.println(log_buffer);
			Serial.flush();
			return;
		}
		}
		//sumTime += micros() - startTime2;
	}
	/*sprintf(log_buffer, "read times: %lu average time: %lu average read: %lu\n",
			times2, sumTime2 / times2, sumBytesRead / times2);
	Serial.print(log_buffer);
	sprintf(log_buffer, "loop times: %lu average time: %luns\n", times,
			(uint32_t) ((sumTime / (float) times) * 1000));
	Serial.print(log_buffer);
	sprintf(log_buffer, "calc time: %lu realTime: %lu", samples * 10 / 441,
			(millis() - startPlayback));
	Serial.print(log_buffer);*/
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

/*
 A1 A0
 0  0  write address to register 1
 0  1  write data to register 1
 1  0  write address to register 2
 1  1  write data to register 2
 */

void write_data(uint8_t reg, uint8_t data, uint8_t port) {
	if (port == REGISTER_1) {
		YM_CTRL_PORT &= ~(_BV(YM_A1) | _BV(YM_A0));
	} else {
		YM_CTRL_PORT &= ~(_BV(YM_A0));
		YM_CTRL_PORT |= _BV(YM_A1);
	}
	write_ym(reg);
	if (port == REGISTER_1) {
		YM_CTRL_PORT &= ~(_BV(YM_A1));
		YM_CTRL_PORT |= _BV(YM_A0);
	} else {
		YM_CTRL_PORT |= _BV(YM_A1) | _BV(YM_A0);
	}
	write_ym(data);
}

void write_ym(uint8_t data) {
	YM_CTRL_PORT &= ~_BV(YM_CS);
	YM_DATA_PORT = data;
	YM_CTRL_PORT &= ~_BV(YM_WR);
	YM_CTRL_PORT |= _BV(YM_WR);
	YM_CTRL_PORT |= _BV(YM_CS);
}

