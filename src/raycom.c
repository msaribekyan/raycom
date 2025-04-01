#include <stdint.h>

#include <raycom.h>
#include <NRF24.h>

// Message declarations
typedef enum {
	MSG_ADVERTISE,
	MSG_ACK_AD,
	MSG_TABLE,
	MSG_CHECK,
	MSG_ACK_CHECK,
	MSG_CLEARANCE,
	MSG_ACK_CLEARANCE
} message_type_t;

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
node_state_t node_state = STATE_INIT;

void nRF_init(uint8_t address){
	node_address = address;
}

void nRF_send(uint8_t* data, uint16_t size){
	
}
