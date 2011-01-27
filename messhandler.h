#ifndef _MESSHANDLER_H_
#define _MESSHANDLER_H_
#include "myheaders.h"

enum /* Другие константы */
{
	MAX_STREAMS = 10,
	MAX_MSG_LENGTH = 2000,
	SUPPORTED_PROTOCOL_VERSION = 0x03
};

enum /* Коды типов сообщений */
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

enum /* Коды типов параметров */
{
	ECM_channel_id = 0x000E,
	ECM_stream_id = 0x000F,
	ECM_id = 0x0019,
	error_status = 0x7000
};

enum /* Коды статусов ошибок */
{
	invalid_message = 0x0001,
	unsupported_protocol_version = 0x0002,
	unknown_message_type_value = 0x0003,
	message_too_long = 0x0004,
	unknown_Super_CAS_id_value = 0x0005,
	unknown_ECM_channel_id_value = 0x0006,
	unknown_ECM_stream_id_value = 0x0007,
	too_many_channels_on_this_ECMG = 0x0008,
	too_many_ECM_streams_on_this_channel = 0x0009,
	too_many_ECM_streams_on_this_ECMG = 0x000A,
	not_enough_control_words_to_compute_ECM = 0x000B,
	ECMG_out_of_storage_capacity = 0x000C,
	ECMG_out_of_computational_resources = 0x000D,
	unknown_parameter_type_value = 0x000E,
	inconsistent_length_for_DVB_parameter = 0x000F,
	missing_mandatory_DVB_parameter = 0x0010,
	invalid_value_for_DVB_parameter = 0x0011,
	unknown_ECM_id_value = 0x0012,
	ECM_channel_id_value_already_in_use = 0x0013,
	ECM_stream_id_value_already_in_use = 0x0014,
	unknown_error = 0x7000,
	unrecoverable_error = 0x7001
};

struct _Channel
{
	int active;
	int has_at_least_one_stream;
	uint16_t ECM_channel_id;
	uint16_t stream[MAX_STREAMS];
	int streams;
	int section_TSpkt_flag;
	uint16_t AC_delay_start;
	uint16_t AC_delay_stop;
	uint16_t delay_start;
	uint16_t delay_stop;
	uint16_t transition_delay_start;
	uint16_t transition_delay_stop;
	uint16_t ECM_rep_period;
	uint16_t max_streams;
	uint16_t min_CP_duration;
	int lead_CW;
	int CW_per_msg;
	uint16_t max_comp_time;
};
typedef struct _Channel Channel;

int ECMG_messhandler(Channel*, Message*, int);
int SCS_messhandler(Channel*, Message*, int);

#endif
