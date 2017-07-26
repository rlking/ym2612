#ifndef VGMPLAYER_H_
#define VGMPLAYER_H_

/* Pinmap (Arduino UNO compatible) */
#define YM_IC (5) // PC5 (= pin A5 for Arduino UNO)
#define YM_CS (4) // PC4 (= pin A4 for Arduino UNO)
#define YM_WR (3) // PC3 (= pin A3 for Arduino UNO)
#define YM_RD (2) // PC2 (= pin A2 for Arduino UNO)
#define YM_A0 (1) // PC1 (= pin A1 for Arduino UNO)
#define YM_A1 (0) // PC0 (= pin A0 for Arduino UNO)
#define YM_CTRL_DDR DDRD
#define YM_CTRL_PORT PORTD
#define YM_DATA_DDR DDRF
#define YM_DATA_PORT PORTF // Whole PORT D for data bus (= pins D0 to D7 for Arduino UNO)

#define PORT_0 0
#define PORT_1 1

void write_ym(uint8_t data);
void write_data(uint8_t reg, uint8_t data, uint8_t port);

#endif /* VGMPLAYER_H_ */
