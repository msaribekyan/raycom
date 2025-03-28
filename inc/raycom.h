#ifndef __RAYCOM_H
#define __RAYCOM_H

#include <stdint.h>

typedef enum {
	STATE_INIT,
	STATE_ADVERTISE,
	STATE_IDLE,
	STATE_TX,
	STATE_RX
} node_state_t;

void nRF_init(uint8_t address);
void nRF_send(uint8_t* data, uint16_t size);

#endif // __RAYCOM_H
