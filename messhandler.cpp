#include "messhandler.h"

int sock;

void f_channel_error(uint16_t);
void f_channel_status(Channel*);

Channel::Channel(): active(false), has_at_least_one_stream(false)
{ }

int ECMG_messhandler(Channel* channel, Message* message, int s)
{
	sock = s; /* инициализируем глобальную переменную,
		     чтобы не передавать ее каждый раз в функцию */
	if (message->protocol_version != SUPPORTED_PROTOCOL_VERSION)
		f_channel_error(unsupported_protocol_version);

	if (message->type >= channel_setup && message->type < stream_setup)
	{ /* Если сообщение для chennel */
		if (channel->active) 
		{
			switch (message->type)
			{
				case channel_test:
					{
						if (/*all rigth */)
							f_chanel_status(channel);
						else
							f_channel_error(unrecoverable_error);
						break;
					}
				case channel_error:
				case channel_close:
					{
						/* Завершаем работу с данным клиентом */
						return 1; 
						break;
					}	
				case channel_setup:
					{
						f_channel_error(too_many_channels_on_this_ECMG);
						break;
					}	
				case channel_status:
					{
						if (/*not all rigth */)
							f_channel_error(unknown_ECM_channel_id_value);
						break;
					}
			}
		}
		else
		{
			if (message->type == channel_setup)
			{
				std::cout << "Incoming message: channel_setup. Setting channel..." << std::endl;
				channel->ECM_channel_id = char2_to_int( message->parameter[0].value );
				channel->active = true;
				std::cout << "Channel 1 setup success" << std::endl;
				std::cout << "Sending channel_status..." << std::endl;
				if ( /*all rigth */)
					f_channel_status(channel);
				else
					f_channel_error(unknown_error);
			}
			else
			{
				f_channel_error(invalid_message);
			}
		}
	}
	else
	{ /* Если сообщение для stream */
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
								f_stream_error(unrecoverable_error);
							break;
						}
					case stream_test:
						{
							if ( /* All rigth */ )
								f_stream_status(channel);
							else
								f_stream_error(unrecoverable_error);
							break;
						}
					case stream_close_request:
						{
							if (there_is_stream_with_income_id(channel,message))
							{
								f_stream_close_response(message);
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
							if ( /* not all rigth */ )
								f_stream_error(unrecoverable_error);
							break;
						}
					case CW_provision:
						{
							f_ECM_response(message);
							break;
						}
				}
			}
			else
			{
				if (message->type == stream_setup)
				{
					set_stream(channel,message);
					if ( /*All rigth */ )
						f_stream_status(channel);
					else
						f_stream_error(unrecoverable_error);
				}
				else
				{
					f_stream_error(unrecoverable_error); 
				}	
			}
		}
		else
		{
			f_stream_error(unrecoverable_error);
		}

		return 0;
	}
}
bool there_is_stream_with_income_id(Channel* channel, Message* message)
{
	return 0;
	//return 1;
}

void f_set_params(Message* message, uint16_t type, char* value)
{
	Parameter param;
	{ /* channel id */
		param.type = ECM_channel_id; // ECM_channel_id
		param.length = 2;
		param.value = int_to_char2(0x0001);
	}	
	message->parameter.push_back(param);
	message->length += sizeof(param.type) + sizeof(param.length) + strlen(param.value);
	switch (message->type)
	{
		case channel_error
		{ /* error status */
			param.type = error_status; // error_status
			param.length = 2;
			param.value = int_to_char2(status);
		}	
		message->parameter.push_back(param);
		message->length += 6;
	}
}
void set_stream(Channel* channel, Message* message)
{
	channel->stream.push_back( char2_to_int(message->parameter[1].value) );
	channel->has_at_least_one_stream = true;
}

void close_stream(Channel* channel, Message* message)
{
	for (int i = 0; i < channel->stream.size(); i++)
		if (channel->stream[i] == char2_to_int(message->parameter[1].value) )
			channel->stream[i]; // закрываем	
}
void f_channel_error(uint16_t status)
{
	Message* message = new Message;
	{ /* Задание данных сообщения */
		message->type = channel_error;
		message->length = 0;
		{ /* Задание набора параметров */
		}
	}
	char* msg_to_client = new char[1000];
	strcpy(msg_to_client,"errrorrrrr");
	// msg_to_client = unparse(message);
	send(sock, msg_to_client, strlen(msg_to_client), 0 );
}
