#include "myheaders.h"
#include "messparser.h"
#include "messhandler.h"
#include "messages.h"


int ECMG_messhandler(Channel* channel, Message* message, int sock)
{
	if (message->protocol_version != SUPPORTED_PROTOCOL_VERSION)
	{
		f_channel_error(sock, unsupported_protocol_version);
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
						f_channel_error(sock, unrecoverable_error);
						break;
					}
				case channel_error:
				case channel_close:
					{
						// Завершаем работу с данным клиентом
						return 1; 
						break;
					}	
				case channel_setup:
					{
						puts("Уже существует канал!");
						f_channel_error(sock, too_many_channels_on_this_ECMG);
						break;
					}	
				case channel_status:
					{
						//if (//not all rigth )
						f_channel_error(sock, too_many_channels_on_this_ECMG);
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
				printf("CID = %i\n", channel->ECM_channel_id);
				printf("CL = %i\n", message->parameter[0]->length);
				printf("PARAM_0_VALUE = %i\n", char2_to_int(message->parameter[0]->value));
				channel->active = 1;
				puts("Channel 1 setup success");
			/*	puts("Sending channel_status...");
				
				if ( 1 )
					f_channel_status(sock);
				else
					f_channel_error(sock, unrecoverable_error);
				puts("Сообщение отправлено");	*/
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
								f_stream_error(sock, unrecoverable_error);
							break;
						}
					case stream_test:
						{
							if ( 1  )
								f_stream_status(sock, 1,1);
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
							if ( 0  )
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
					if ( 1  )
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
	return 0;
}
int there_is_stream_with_income_id(Channel* channel, Message* message)
{
	return 0;
	//return 1;
}

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
