#ifndef VGMPLAYER_H_
#define VGMPLAYER_H_

#define YM_IC 5
#define YM_CS 4
#define YM_WR 3
#define YM_RD 2
#define YM_A0 1
#define YM_A1 0
#define YM_CTRL_DDR DDRD
#define YM_CTRL_PORT PORTD
#define YM_DATA_DDR DDRF
#define YM_DATA_PORT PORTF

#define PORT_0 0
#define PORT_1 1

void write_ym(uint8_t data);
void write_data(uint8_t reg, uint8_t data, uint8_t port);

#endif /* VGMPLAYER_H_ */
