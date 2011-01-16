#include "messhandler.h"

void f_channel_error(int, uint16_t);
void f_channel_status(int, Channel*);

Channel::Channel(): active(false)
{ }

int messhandler(Channel* channel, Message* message, int sock)
{
	std::cout << "eeeeeeeeee" << std::endl;
	std::cout << "Protocol version: " <<  (int)message->protocol_version << std::endl;
	if (message->protocol_version != supported_protocol_version)
		f_channel_error(sock, unsupported_protocol_version);
	else if (channel->active) 
	{
		//
	}
	else
	{
		if (message->type == channel_setup)
		{
			std::cout << "Incoming message: channel_setup. Setting channel..." << std::endl;
			{ /* Установка канала */
				channel->ECM_channel_id = char2_to_int( message->parameter[0].value );
				channel->active = true;
			}	
			std::cout << "Channel 1 setup success" << std::endl;
			std::cout << "Sending channel_status..." << std::endl;
			{ // Посылка клиенту channel_status 
				f_channel_status(sock, channel);
			} 	
		}
		else
		{
			f_channel_error(sock, invalid_message);
		}
	}
	return 0;
}

void f_channel_error(int sock, uint16_t status)
{
	Message* message = new Message;
	{ /* Задание данных сообщения */
		message->type = channel_error;
		message->length = 0;
		{ /* Задание набора параметров */
			Parameter param;
			{ /* channel id */
				param.type = ECM_channel_id; // ECM_channel_id
				param.length = 2;
				param.value = int_to_char2(0x0001);
			}	
			message->parameter.push_back(param);
			message->length += 6;
			{ /* error status */
				param.type = error_status; // error_status
				param.length = 2;
				param.value = int_to_char2(status);
			}	
			message->parameter.push_back(param);
			message->length += 6;
		}
	}
	char* msg_to_client = new char[1000];
	strcpy(msg_to_client,"errrorrrrr");
	// msg_to_client = unparse(message);
	send(sock, msg_to_client, strlen(msg_to_client), 0 );
}

void f_channel_status(int sock, Channel* channel)
{
	Message* message = new Message;
	{ /* Задание данных сообщения */
		message->type = channel_status;
		message->length = 0;
		{ /* Задание набора параметров */
			Parameter param;
			{ /* channel id */
				param.type = ECM_channel_id; // ECM_channel_id
				param.length = 2;
				param.value = int_to_char2(0x0001);
			}	
			message->parameter.push_back(param);
			message->length += 6;
			{ /* error status */
				param.type = 0x0001; // error_status
				param.length = 2;
				param.value = int_to_char2(10);
			}	
			message->parameter.push_back(param);
			message->length += 6;
		}
	}
	char* msg_to_client = new char[1000];
	strcpy(msg_to_client,"errrorrrrr");
	// msg_to_client = unparse(message);
	send(sock, msg_to_client, strlen(msg_to_client), 0 );
}
