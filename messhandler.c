#include "messhandler.h"

int sock;

void f_channel_error(uint16_t, int);
void f_channel_status(Channel*, int);

int ECMG_messhandler(Channel* channel, Message* message, int sock)
{
	if (message->protocol_version != SUPPORTED_PROTOCOL_VERSION)
	{
		f_channel_error(unsupported_protocol_version, sock);
		puts("unsupported_protocol_version");
		return 1;
	}

	if (message->type >= channel_setup && message->type < stream_setup)
	{ /* Если сообщение для channel */
		if (channel->active == 1) 
		{
			puts("channel is active!");
			switch (message->type)
			{
				case channel_test:
					{
						//if (//all rigth )		
						//f_chanel_status(channel, sock);
						//else
							f_channel_error(unrecoverable_error, sock);
						break;
					}
				case channel_error:
				case channel_close:
					{
						// Завершаем работу с данным клиентом
						puts("Вот где собака зарыта");
						return 1; 
						break;
					}	
				case channel_setup:
					{
						puts("Уже существует канал!");
						f_channel_error(too_many_channels_on_this_ECMG, sock);
						break;
					}	
				case channel_status:
					{
						//if (//not all rigth )
							f_channel_error(unknown_ECM_channel_id_value, sock);
						break;
					}
			}
		}
		else
		{
			if (message->type == channel_setup)
			{
				puts("Incoming message: channel_setup. Setting channel...");
				channel->ECM_channel_id = char2_to_int( message->parameter[0]->value );
				channel->active = 1;
				puts("Channel 1 setup success");
				puts("Sending channel_status...");
				//if ( //all rigth )
				//	f_channel_status(channel, sock);
				//else
					f_channel_error(unknown_error, sock);
				puts("Сообщение отправлено");	
			}
			else
			{
				f_channel_error(invalid_message, sock);
			}
		}
	}/*
	else
	{ // Если сообщение для stream 
		if (channel->active)
		{
			if (channel->has_at_least_one_stream)
			{
				switch (message->type)
				{
					case stream_setup:
						{	
							if ( ! there_is_stream_with_income_id(channel,message))
								set_stream(channel,message);
							else
								f_stream_error(unrecoverable_error, sock);
							break;
						}
					case stream_test:
						{
							if ( // All rigth  )
							)	f_stream_status(channel, sock);
							else
								f_stream_error(unrecoverable_error, sock);
							break;
						}
					case stream_close_request:
						{
							if (there_is_stream_with_income_id(channel,message))
							{
								f_stream_close_response(message, sock);
								close_stream(channel, message);
							}
							break;
						}
					case stream_error:
						{
							close_stream(channel, message);
							break;
						}
					case stream_status:
						{
							if ( // not all rigth  )
							)
								f_stream_error(unrecoverable_error, sock);
							break;
						}
					case CW_provision:
						{
							f_ECM_response(message, sock);
							break;
						}
				}
			}
			else
			{
				if (message->type == stream_setup)
				{
					set_stream(channel,message);
					if ( //All rigth  )
					)
						f_stream_status(channel, sock);
					else
						f_stream_error(unrecoverable_error, sock);
				}
				else
				{
					f_stream_error(unrecoverable_error, sock); 
				}	
			}
		}
		else
		{
			f_stream_error(unrecoverable_error, sock);
		}

		return 0;
	} */
	puts("Возвращаем 0");
	return 0;
}
int there_is_stream_with_income_id(Channel* channel, Message* message)
{
	return 0;
	//return 1;
}

/*void set_params(Message* message, uint16_t type, char* value)
{
	Parameter param;
	{ // channel id 
		param.type = ECM_channel_id; // ECM_channel_id
		param.length = 2;
		param.value = int_to_char2(0x0001);
	}	
	message->parameter.push_back(param);
	message->length += sizeof(param.type) + sizeof(param.length) + strlen(param.value);
	switch (message->type)
	{
		case channel_error
		{ // error status 
			param.type = error_status; // error_status
			param.length = 2;
			param.value = int_to_char2(status);
		}	
		message->parameter.push_back(param);
		message->length += 6;
	}
} */
void set_stream(Channel* channel, Message* message)
{
	channel->stream[channel->streams + 1] = ( char2_to_int(message->parameter[1]->value) );
	channel->has_at_least_one_stream = 1;
}

void close_stream(Channel* channel, Message* message)
{
	int i;
	for (i = 0; i < channel->streams; i++)
		if (channel->stream[i] == char2_to_int(message->parameter[1]->value) )
			channel->stream[i]; // закрываем	
}
void f_channel_error(uint16_t status, int sock)
{
	Message* message = malloc(sizeof(Message));
	message->protocol_version = SUPPORTED_PROTOCOL_VERSION;
	{ // Задание данных сообщения
		message->type = channel_error;
		message->length = 0;
		{ // Задание набора параметров 
		}
	}
	char* msg_to_client = malloc(1000);
	strcpy(msg_to_client,"errrorrrrr");
	// msg_to_client = unparse(message);
	send(sock, msg_to_client, strlen(msg_to_client), 0 );
}
