#ifndef _MESSHANDLER_H_
#define _MESSHANDLER_H_
#include "myheaders.h"
#include "messparser.h"

enum
{
	MAX_MSG_LENGTH = 2000
};
enum
{
	channel_setup = 	0x0001,
	channel_test = 		0x0002,
	channel_status = 	0x0003,
	channel_close = 	0x0004,
	channel_error = 	0x0005,
	stream_setup = 		0x0101,
	stream_test = 		0x0102,
	stream_status = 	0x0103,
	stream_close_request = 	0x0104,
	stream_close_response = 0x0105,
	stream_error = 		0x0106,
	CW_provision = 		0x0201,
	ECM_response = 		0x0202
};

enum
{
	ECM_channel_id = 0x000E,
	error_status = 0x7000
};
enum
{
	supported_protocol_version = 0x03
};

enum
{
	invalid_message = 0x0001,
	unsupported_protocol_version = 0x0002
};

struct Channel
{
	Channel();
	bool active;
	uint16_t ECM_channel_id;
	bool section_TSpkt_flag;
	uint16_t AC_delay_start;
	uint16_t AC_delay_stop;
	uint16_t delay_start;
	uint16_t delay_stop;
	uint16_t transition_delay_start;
	uint16_t transition_delay_stop;
	uint16_t ECM_rep_period;
	uint16_t max_streams;
	uint16_t min_CP_duration;
	bool lead_CW;
	bool CW_per_msg;
	uint16_t max_comp_time;
};

int messhandler(Channel*, Message*, int);

#endif
