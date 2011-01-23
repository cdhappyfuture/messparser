#include "messparser.h"

Message::Message(): protocol_version(0x03)
{ }

uint16_t char2_to_int(char* bytes)
{
	uint16_t var = (unsigned int)bytes[0]*256 + (unsigned int)bytes[1];
	return var;
}
char* int_to_char2(uint16_t var)
{
	char* str = new char[2];
	str[0] = ((char*)&var)[0];
	str[1] = ((char*)&var)[1];
	return str;	
}
struct Message* parse(int input)
{
	struct Message* message = new Message;
	char buf[3];

	{ /* Получение данных о message */
		recv(input,buf,1,0);
		message->protocol_version = (unsigned int)*buf;
		recv(input,buf,2,0);
		message->type = char2_to_int(buf);
		recv(input,buf,2,0);
		message->length = char2_to_int(buf);
	}

	int i = 0;
	int mes_value_curr_size = 0;

	while (message->length >= mes_value_curr_size + 4)
	{
		Parameter param;
		{ /* Получение данных об очередном параметре */
			recv(input,buf,2,0);
			param.type = char2_to_int(buf);	
			recv(input,buf,2,0);
			param.length = char2_to_int(buf);
			mes_value_curr_size += 4;
		}

		if (param.length + mes_value_curr_size > message->length)
			break;

		{ /* Получение значения параметра */
			param.value = new char[param.length];
			recv(input, param.value, param.length, 0);
			message->parameter.push_back(param);
			mes_value_curr_size += message->parameter[i].length;
			i++;
		}	
	}
	return message;
}

char* unparse(Message* message)
{
	/**/
}	
