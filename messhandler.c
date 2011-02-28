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
			switch (message->type)
			{
				case channel_test:
					{
                        puts("Income message: channel_test. Sending channel_status...");
						//if (//all rigth )		
						f_channel_status(sock);
						// else f_channel_error(sock, unrecoverable_error);
						break;
					}
				case channel_error:
				case channel_close:
					{
						// Завершаем работу с данным клиентом
						puts("Incoming message channel_error or channel close. Closing connection");
						return 1; 
						break;
					}	
				case channel_setup:
					{
						puts("Channel alredy exists! Sending channel_error...");
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
				channel->active = 1;
				if (channel->active == 1)
				{
					printf("Channel %i setup success\n", channel->ECM_channel_id);
					puts("Sending channel_status...");
					f_channel_status(sock);
				}
				else
				{
					puts("Error setuping channel. Sending channel_error...");
					f_channel_error(sock, unrecoverable_error);
				}
			}
			else
			{	
				puts("Channel is not setuped. Sending channel error...");
				f_channel_error(invalid_message, sock);
			}
		}
	}
	else
	{ // Если сообщение для stream 
		uint16_t cur_stream = char2_to_int(message->parameter[1]->value);
		if (channel->active)
		{
			if (channel->has_at_least_one_stream)
			{
				switch (message->type)
				{
					case stream_setup:
						{	
							if ( ! there_is_stream_with_income_id(channel, cur_stream))
							{
								puts("Income message: stream_setup. Setuping stream");
								set_stream(channel,message);
								printf("Stream %i setup success. Sending stream_status...", cur_stream);
                                f_stream_status(sock, 7, 1);
							}
							else
                            {
                                puts("Error: There is stream with inclome id. Sending stream_error...");
								f_stream_error(sock, cur_stream, unrecoverable_error);
                            }
							break;
						}
					case stream_test:
						{
							if ( 1  )
                            {
                                puts("Income message: stream_test. Sending stream_status");
								f_stream_status(sock, 1,1);
                            }
							else
								f_stream_error(sock, cur_stream, unrecoverable_error);
							break;
						}
					case stream_close_request:
						{
                            puts("Income message: stream_close_request.");
							if (there_is_stream_with_income_id(channel,message))
							{
                                puts("Sending stream close_response...");
								f_stream_close_response(sock, cur_stream);
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
								f_stream_error(sock, cur_stream, unrecoverable_error);
							break;
						}
					case CW_provision:
						{
							f_ECM_response(sock, cur_stream);
							break;
						}
				}
			}
			else
			{
				if (message->type == stream_setup)
				{
					puts("Income message stream setup");
					set_stream(channel,message);
					puts("Stream setup success");
					if ( 1  ) // если стрим успешно установлен, заглушка
					{
						puts("Sending stream_status...");
						f_stream_status(sock, char2_to_int(message->parameter[1]->value),
                                char2_to_int(message->parameter[2]->value));
					}
					else
					{
						puts("Error occured while setuping stream. Sending stream_error");	
						f_stream_error(sock, cur_stream, unrecoverable_error);
					}
				}
				else
				{
					f_stream_error(sock, cur_stream, unrecoverable_error);
				}
				return 0;
			}
		}
		else
		{
			f_stream_error(sock, cur_stream, unrecoverable_error);
		}
		return 0;
	} 
	return 0;
}
int there_is_stream_with_income_id(Channel* channel, Message* message)
{
	return 1;
	//return 0;
}

void set_stream(Channel* channel, Message* message)
{
	channel->stream[++channel->streams] = ( char2_to_int(message->parameter[1]->value) );
	printf("Setting stream %i\n",channel->stream[channel->streams]);
	channel->has_at_least_one_stream = 1;
}

void close_stream(Channel* channel, Message* message)
{
	int i;
	for (i = 0; i < channel->streams; i++)
		if (channel->stream[i] == char2_to_int(message->parameter[1]->value) )
			channel->stream[i] = 0;
//			channel->has_at_least_one_stream = 0;
}
