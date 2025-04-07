#include <stdint.h>
#include <stdring.h>

#include <raycom.h>
#include <NRF24.h>

typedef enum {
	__MODE_RX,
	__MODE_TX
} nrf_mode_t;

typedef enum {
	__IDLE,
	__TX_REQ,
	__DATA_RX,
	__DATA_TX
} nrf_state_t;

typedef struct {
	uint8_t data[32];
} nrf_frame_t;

typedef enum{
	__FRAME_REQ,
	__FRAME_REQ_ACK,
	__FRAME_DATA,
	__FRAME_END_DATA,
	__FRAME_BACK
} nrf_frame_type_t;

uint8_t common_address[5] = {0x8D,0x4D,0xBD,0xBD,0x3A};
uint8_t MAC_address[6] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60};

nrf_state_t current_state = __IDLE;
nrf_mode_t current_mode = __MODE_RX;
uint8_t common_channel = 10;
uint8_t recv_channel = 0;

uint8_t FRAME_CHECK_ADDRESS(nrf_frame_t frame){
	uint8_t match = 1;
	for(uint8_t i = 0;i < 6;i++){
		if(frame.data[i+1] != MACAddress[i]){
			match = 0;
			break;
		}
	}
	if(match == 1){
		return 1; // Good dest
	}
	match = 1;
	for(uint8_t i = 0;i < 6;i++){
		if(frame.data[i+1] != 0xFF){
			match = 0;
			break;
		}
	}
	if(match == 1){
		return 2; // Broadcast
	}

	return 0; // No match
}

uint8_t NRF_SETUP(nrf_mode_t mode, uint8_t channel, uint8_t* address){
	CE_Disable();

	nrf24_reset (0); // Change later

	switch(channel){
		case common_channel:
			nrf24_WriteReg(EN_AA, 0b00000000);
			nrf24_WriteReg (SETUP_RETR, 0b00000000);
			break;
		default:
			nrf24_WriteReg(EN_AA, 0b00000001);
			nrf24_WriteReg (SETUP_RETR, 0b00000011);
			break;
	}

	switch(mode){
	case __MODE_RX:
		nrf24_WriteReg (EN_RXADDR, 0b00000001);
		nrf24_WriteRegMulti(RX_ADDR_P0, address, 5);
		nrf24_WriteReg (RX_PW_P0, 32);
		nrf24_WriteReg(CONFIG, 0b00111011);
		break;
	case __MODE_TX:
		nrf24_WriteReg(CONFIG, 0b01011000);
		uint8_t address[5] = {0x8D,0x4D,0xBD,0xBD,0x3A};
		nrf24_WriteRegMulti(TX_ADDR, address, 5);
		break;
	default:
		break;
	}

	nrf24_WriteReg (EN_RXADDR, 0b00000001);
	nrf24_WriteReg (SETUP_AW, 0b00000011);

	nrf24_WriteReg (RF_CH, channel);
	nrf24_WriteReg (RF_SETUP, 0b00010110);

	uint8_t config = nrf24_ReadReg(CONFIG);
	nrf24_WriteReg (CONFIG, config | (1<<1));

	CE_Enable();

	return 0;
}

uint8_t NRF_RECEIVE_FRAME(nrf_frame_t* frame){
	uint8_t status = nrf24_ReadReg(STATUS);

	if(status & (1<<6) == 0){
		return 1;
	}

	nrf24_WriteReg(STATUS, status | (1<<6));

	while(nrf24_ReadReg(FIFO_STATUS) & (1<<1)){
		uint8_t RxBuf[32];
		memset(RxBuf, 0, 32);
		NRF24_Receive(RxBuf, 32);
		memcpy(frame->data, RxBuf, 32);
		// Add to queue
	}

	return 0;

}

uint8_t NRF_TRANSMIT_FRAME(nrf_frame_t frame){

	if (isDataAvailable(2) == 1) {
		return 1;
	}
	uint8_t TxBuf[32];
	memset(TxBuf, 0, 32);
	memcpy(TxBuf, frame.data, 32);
	NRF24_Transmit(TxBuf, 32);

	return 0;

}

void NRF24_IRQ_Handler(){
	switch(current_mode){
	case __IDLE:
		NRF_IRQ_IDLE();
		break;
	case __TX_REQ:
		NRF_IRQ_TX_REQ();
		break;
	case __DATA_TX:
		NRF_IRQ_DATA_TX();
		break;
	case __DATA_RX:
		NRF_IRQ_DATA_RX();
		break;
	default:
		break;
	}
}

void NRF_IRQ_IDLE(){
	nrf_frame_t frame;

	if(NRF_RECEIVE_FRAME(&frame) == 1){
		return 0;
	}

	switch(frame.data[0]){
	case __FRAME_REQ:
		if(FRAME_CHECK_ADDRESS(frame) == 1){
			// Check if buffer full
			// Set to TX mode on common channel
			// Send packet
			// Set to RX mode on common channel
		}
		// Take notes
		break;
	case __FRAME_REQ_ACK:
		// Take notes
		break;
	case __FRAME_BACK:
		// Take notes
		break;
	case __FRAME_DATA:
	case __FRAME_END_DATA:
	default:
		break;
	}
	// Recieved a frame in channel
	// Check dest address and ignore or parse
}

void NRF_IRQ_TX_REQ(){
	nrf_frame_t frame;

	if(NRF_RECEIVE_FRAME(&frame) == 1){
		return 0;
	}

	switch(frame.data[0]){
	case __FRAME_REQ:
		break;
	case __FRAME_REQ_ACK:
		// Take notes
		break;
	case __FRAME_BACK:
		// Take notes
		break;
	case __FRAME_DATA:
	case __FRAME_END_DATA:
	default:
		break;
	}
	// Recieved a frame in common channel
}

void NRF_IRQ_DATA_TX(){
	nrf_frame_t frame;

	if(NRF_RECEIVE_FRAME(&frame) == 1){
		return 0;
	}

	switch(frame.data[0]){
	case __FRAME_REQ:
		break;
	case __FRAME_REQ_ACK:
		// Take notes
		break;
	case __FRAME_BACK:
		// Take notes
		break;
	case __FRAME_DATA:
	case __FRAME_END_DATA:
	default:
		break;
	}
	// Data transfer completed in private channel
	// Remove from tx queue, until complete
}

void NRF_IRQ_DATA_RX(){
	nrf_frame_t frame;

	if(NRF_RECEIVE_FRAME(&frame) == 1){
		return 0;
	}

	switch(frame.data[0]){
	case __FRAME_REQ:
		break;
	case __FRAME_REQ_ACK:
		// Take notes
		break;
	case __FRAME_BACK:
		// Take notes
		break;
	case __FRAME_DATA:
	case __FRAME_END_DATA:
	default:
		break;
	}
	// Received a frame in private channel
	// Add to rx queue
}
