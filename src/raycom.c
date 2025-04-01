#include <stdint.h>

#include <raycom.h>
#include <NRF24.h>

#define MSG_TYPE_DISC 		0x0100
#define MSG_TYPE_DISC_ACK 	0x0200
#define MSG_TYPE_TABLE 		0x0300
#define MSG_TYPE_PING 		0x0400
#define MSG_TYPE_PING_ACK 	0x0500
#define MSG_TYPE_CLEAR		0x0600
#define MSG_TYPE_CLEAR_ACK	0x0700

// Message declarations
typedef struct {
	uint16_t message_conf;
	uint8_t[30] message;
} message_t;

// Queue setup
typedef struct message_queue_node{
	struct message_queue_node* next;
	message_t message_packet;
} message_queue_node;

message_queue_node* message_queue_head = NULL;

uint8_t message_queue_node_push(message_t message){
	message_queue_node* new_message_queue_node;
	new_message_queue_node = (message_queue_node*) malloc(sizeof(message_queue_node));

	new_message_queue_node->next = message_queue_head;
	new_message_queue_node->message = *message;

	message_queue_head = new_message_queue_node;

	return 0;
}

uint8_t message_queue_node_pop(message_t* message){
	if(message_queue_head == NULL){
		return 1;
	}

	top_message_queue_node = message_queue_head;
	message_queue_head = top_message_queue_node->next;

	*message = message_queue_head->message;

	free(top_message_queue_node);
	return 0;
}

// Module setup
uint8_t node_address = 0;
node_state_t node_state = STATE_IDLE;

void nRF_init(uint8_t address){
	node_address = address;
}

void nRF_send(uint8_t* data, uint16_t size){
	
}

void nRF_common_queue(){
	if(node_state == STATE_IDLE){
		return;
	}

	return;
}
