#include <stdio.h>

#include <raycom.h>
#include <NRF24.h>

uint8_t node_address = 0;
node_state_t node_state = STATE_INIT;

void nRF_init(uint8_t address){
	node_address = address;
}

void nRF_send(uint8_t* data, uint16_t size){
	
}
