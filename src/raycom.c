#include <stdint.h>
#include <stdring.h>

#include <raycom.h>
#include <NRF24.h>

#define MSG_TYPE_DISC 		0x01
#define MSG_TYPE_DISC_ACK 	0x02
#define MSG_TYPE_TABLE 		0x03
#define MSG_TYPE_PING 		0x04
#define MSG_TYPE_PING_ACK 	0x05
#define MSG_TYPE_CLEAR		0x06
#define MSG_TYPE_CLEAR_ACK	0x07
#define MSG_TYPE_MESSAGE	0x08

// Message declarations
typedef struct {
	uint8_t[32] data;
} message_t;

message_t empty_message(){
	message_t message;
	memset(message.data, 0, 32);
	return message;
}

// Queue setup
typedef struct message_queue_node{
	message_t message_packet;
	struct message_queue_node* next;
} message_queue_node;

message_queue_node* message_queue_head = NULL;
message_queue_node* message_queue_tail = NULL;

uint8_t message_enqueue(message_t message){
	message_queue_node* new_message_queue_node;
	new_message_queue_node = (message_queue_node*) malloc(sizeof(message_queue_node));

	new_message_queue_node->next = NULL;
	new_message_queue_node->message = message;
	
	message_queue_tail->next = new_message_queue_node;
	message_queue_tail = new_message_queue_node;

	return 0;
}

uint8_t message_dequeue(message_t* message){
	if(message_queue_head == NULL){
		return 1;
	}

	top_message_queue_node = message_queue_head;
	message_queue_head = top_message_queue_node->next;

	if(message_queue_head->next == NULL){
		message_queue_tail = NULL;
	}

	*message = top_message_queue_node->message;

	free(top_message_queue_node);
	return 0;
}

// Module setup
uint8_t node_address = 0;
node_state_t node_state = STATE_IDLE;

void nRF_init(uint8_t address){
	node_address = address;
}
uint8_t RF_request_clearance(uint8_t dest, uint8_t channel){
	message_t message = empty_message();
	message.data[0] = MSG_TYPE_CLEAR;
	message.data[1] = dest;
	message.data[2] = node_address;

	message_queue_node_push(message);

	return 0;
}

void nRF_common(){
	if(node_state == STATE_IDLE){
		return;
	}

	return;
}
