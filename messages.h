#include "myheaders.h"
#include "messparser.h"
#include "messhandler.h"
enum 
{
	STD_VALUE_LEN = 0x0002
};

void alloc_params(Message*);
void setup_and_send(int, Message*);
void set_param(int, Message*, uint16_t, uint16_t, uint16_t);

void f_channel_setup(int);
void f_channel_error(int, uint16_t);
void f_channel_close(int);
void f_channel_status(int);
void f_channel_test(int);
void f_stream_setup(int, uint16_t, uint16_t);
void f_stream_test(int, uint16_t);
void f_stream_status(int, uint16_t, uint16_t);
void f_stream_close_request(int, uint16_t);
void f_stream_close_response(int, uint16_t);
void f_CW_provision(int, uint16_t);
void f_ECM_response(int, uint16_t);
void f_stream_error(int, uint16_t strID, uint16_t error_status);
