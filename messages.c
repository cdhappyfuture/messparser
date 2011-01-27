#include "messages.h"

void alloc_params(Message* message)
{
	message->parameter = calloc(message->params, sizeof(Parameter*));
	int i;
	for (i = 0; i < message->params; i++)
		message->parameter[i] = malloc(sizeof(Parameter));
}	

void setup_and_send(int sock, Message* message)
{	
	message->protocol_version = SUPPORTED_PROTOCOL_VERSION;
	message->type = htons(message->type);
	int i;
	for (i = 0; i < message->params; i++ )
		message->length += sizeof(message->parameter[i]->type) + sizeof(message->parameter[i]->length) +	
			ntohs(message->parameter[i]->length);
	message->length = htons(message->length);
	set_param(0, message, ECM_channel_id, STD_VALUE_LEN, 0x0001);
	char* msg_to_server = unparse(message);
	send(sock, msg_to_server, sizeof(message->protocol_version) + sizeof(message->type) + 
		sizeof(message->length) + ntohs(message->length), 0); // отсылаем получившееся сообщение
}
void set_param(int i, Message* message, uint16_t type, uint16_t len, uint16_t value)
{	
	message->parameter[i]->type = htons(type);
	message->parameter[i]->value = malloc(len + 1);
	message->parameter[i]->length = htons(len);
	value = htons(value);
	memcpy((void*)message->parameter[i]->value, (void*)&value, len);
}
//----------------------------------------------------------------------------------------
void f_channel_setup(int sock)
{		
	Message* message = malloc(sizeof(Message));
	message->type = channel_setup;
	message->params = 1;
	alloc_params(message);
	setup_and_send(sock, message);
	free_mes(message);	
}
void f_channel_error(int sock, uint16_t status)
{
	Message* message = malloc(sizeof(Message));
	message->type = channel_error;
	message->params = 2;
	alloc_params(message);
	set_param(1, message, error_status, STD_VALUE_LEN, status);
	setup_and_send(sock, message);
	free_mes(message);
}
void f_channel_close(int sock)
{
	Message* message = malloc(sizeof(Message));
	message->type = channel_close;
	message->params = 1;
	alloc_params(message);
	setup_and_send(sock, message);
	free_mes(message);
}
	
void f_channel_status(int sock)
{
	Message* message = malloc(sizeof(Message));
	message->type = channel_status;
	message->params = 10;
	alloc_params(message);
	// тут задаем параметры
	setup_and_send(sock, message);
	free_mes(message);
}
void f_channel_test(int sock)
{
	Message* message = malloc(sizeof(Message));
	message->type = channel_test;
	message->params = 1;
	alloc_params(message);
	setup_and_send(sock, message);
	free_mes(message);	
}
//--------------------------------------------------------------------------------
void f_stream_setup(int sock, uint16_t stream_id, uint16_t ecm_id)
{
	Message* message = malloc(sizeof(Message));
	message->type = stream_setup;
	message->params = 4;
	alloc_params(message);
	set_param(1, message, ECM_stream_id, STD_VALUE_LEN, stream_id);
	set_param(2, message, ECM_id, STD_VALUE_LEN, ecm_id);
	//Еще один параметр
	setup_and_send(sock, message);
	free_mes(message);	
}
void f_stream_test(int sock, uint16_t stream_id)
{
	Message* message = malloc(sizeof(Message));
	message->type = stream_test;
	message->params = 2;
	alloc_params(message);
	set_param(1, message, ECM_stream_id, STD_VALUE_LEN, stream_id);
	setup_and_send(sock, message);
	free_mes(message);	
}
void f_stream_status(int sock, uint16_t stream_id, uint16_t ecm_id)
{
	Message* message = malloc(sizeof(Message));
	message->type = stream_status;
	message->params = 4;
	alloc_params(message);
	set_param(1, message, ECM_stream_id, STD_VALUE_LEN, stream_id);
	set_param(2, message, ECM_id, STD_VALUE_LEN, ecm_id);
	//Еще один параметр
	setup_and_send(sock, message);
	free_mes(message);	
}
void f_stream_close_request(int sock, uint16_t stream_id)
{
	Message* message = malloc(sizeof(Message));
	message->type = stream_close_request;
	message->params = 2;
	alloc_params(message);
	set_param(1, message, ECM_stream_id, STD_VALUE_LEN, stream_id);
	setup_and_send(sock, message);
	free_mes(message);	
}
void f_stream_close_response(int sock, uint16_t stream_id)
{
	Message* message = malloc(sizeof(Message));
	message->type = stream_close_response;
	message->params = 2;
	alloc_params(message);
	set_param(1, message, ECM_stream_id, STD_VALUE_LEN, stream_id);
	setup_and_send(sock, message);
	free_mes(message);	
}
void f_CW_provision(int sock, uint16_t stream_id)
{
//??????????????
}
void f_ECM_response(int sock, uint16_t stream_id)
{
//??????????????
}
void f_stream_error(int sock, uint16_t stream_id, uint16_t er_status)
{
	Message* message = malloc(sizeof(Message));
	message->type = stream_error;
	message->params = 3;
	alloc_params(message);
	set_param(1, message, ECM_stream_id, STD_VALUE_LEN, stream_id);
	set_param(2, message, error_status, STD_VALUE_LEN, er_status);
	setup_and_send(sock, message);
	free_mes(message);	
}
