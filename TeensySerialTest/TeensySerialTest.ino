#include "Arduino.h"
#include <stdio.h>
#include <util/delay.h>
#include "VgmPlayer.h"

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
}

#define RINGBUFF_SIZE 1024 * 7
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
	uint32_t startPlayback = 0;
	uint32_t times = 0;
	uint32_t sumTime = 0;
	uint32_t sumSamples = 0;
	int bytesRead = 0;
	uint32_t sumBytesRead = 0;

	uint32_t size = 0;
	uint32_t samples = 0;
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
	memcpy(&samples, &buffer[0], sizeof(uint32_t));
	sprintf(log_buffer, "samples: %lu", samples);
	Serial.println(log_buffer);

	readBytes(36);
	i = 64;
	startPlayback = millis();
	startTime = micros();
	while (i < size) {
		i++;
		if (count <= 5) {
			bytesRead = readChunk();
			if (bytesRead <= 0) {
				sprintf(log_buffer, "starved@%ld", i);
				Serial.println(log_buffer);
				Serial.flush();
				return;
			}
		}

		if (waitSamples > 0) {
			sumSamples += waitSamples;
			double totalTime = startTime
					+ sumSamples * (double) (1000000 / 44100);
			double totalTime2 = startTime + sumSamples * 23;
			double totalTime3 = startTime
					+ (double) sumSamples * (double) 22.67573696;
			while (totalTime2 > micros()) {
				//if (waitSamples >= 1) {
				if (count <= RINGBUFF_SIZE - SERIAL_READ_SIZE) {
					//startTime2 = micros();
					bytesRead = readChunk();
					//times++;
					//sumTime += micros() - startTime2;
					//sumBytesRead += bytesRead;
				}
				//}
			}
			/*Serial.println("oida");
			 Serial.println(totalTime);
			 Serial.println(totalTime2);
			 Serial.println(totalTime3);
			 Serial.println(micros());*/

		}

		buffer[0] = rbuff[readOffset];
		next();

		//sprintf(log_buffer, " buffload: %lu i: %ld, available: %d cmd: 0x%x waitSamples: %lu ym: %lu",
		//		count, i, Serial.available(), buffer[0], waitSamples, waitMicrosSec);
		//Serial.println(log_buffer);

		waitSamples = 0;

		times++;
		startTime2 = micros();
		switch (buffer[0]) {
		case 0x4F: {
			//0x4F dd    : Game Gear PSG stereo, write dd to port 0x06
			buffer[1] = rbuff[readOffset];
			next();
			i += 1;
			break;
		}
		case 0x52: {
			//0x52 aa dd : YM2612 port 0, write value dd to register aa
			buffer[1] = rbuff[readOffset];
			next();
			buffer[2] = rbuff[readOffset];
			next();
			write_data(buffer[1], buffer[2], PORT_0);
			//System.out.printf("PORT_0: %x %x\n", music[i + 1], music[i + 2]);
			i += 2;
			break;
		}
		case 0x53: {
			//0x53 aa dd : YM2612 port 1, write value dd to register aa
			buffer[1] = rbuff[readOffset];
			next();
			buffer[2] = rbuff[readOffset];
			next();
			write_data(buffer[1], buffer[2], PORT_1);
			//System.out.printf("PORT_1: %x %x\n", music[i + 1], music[i + 2]);
			i += 2;
			break;
		}
		case 0x50: {
			//0x50	dd	PSG (SN76489/SN76496) write value dd
			buffer[1] = rbuff[readOffset];
			next();
			// skip
			i += 1;
			break;
		}
		case 0x61: {
			buffer[1] = rbuff[readOffset];
			next();
			buffer[2] = rbuff[readOffset];
			next();
			waitSamples = ((buffer[1] << 0) & 0x000000FF)
					| ((buffer[2] << 8) & 0x0000FF00);
			i += 2;
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
			write_data(0x2a, rbuff[readOffset], PORT_0);
			next();
			waitSamples = buffer[0] & 0x0f;
			i += 1;
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
			sprintf(log_buffer, "unbekanntes zeichen: 0x%x / %d pos: %ld",
					buffer[0], buffer[0], i);
			Serial.println(log_buffer);
			Serial.flush();
			return;
		}
		}
		sumTime += micros() - startTime2;
	}
	sprintf(log_buffer, "times: %lu average time: %lu\n", times,
			sumTime / times/*, sumBytesRead / times*/);
	Serial.print(log_buffer);
	sprintf(log_buffer, "calc time: %lu realTime: %lu", samples / 44100,
			(millis() - startPlayback) / 1000);
	Serial.print(log_buffer);
}

uint8_t readByte() {
	uint8_t ret = rbuff[readOffset];
	next();
	return ret;
}

void next() {
	count--;
	readOffset++;
	if (readOffset == RINGBUFF_SIZE) {
		readOffset = 0;
	}
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

