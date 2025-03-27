#ifndef __RAYCOM_H
#define __RAYCOM_H

typedef enum {
	STATE_INIT,
	STATE_ADVERTISE,
	STATE_IDLE,
	STATE_TX,
	STATE_RX
} node_state;

#endif // __RAYCOM_H
