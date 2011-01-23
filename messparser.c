#include "messparser.h"

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
	Message* message = calloc(sizeof(Message));
	char buf[10];

	{ /* Получение данных о message */
		recv(input,buf,5,0);
		message->protocol_version = (unsigned int)*buf;
		message->type = char2_to_int(&buf[1]);
		message->length = char2_to_int(&buf[3]);
	}

	int i = 0;
	int mes_value_curr_size = 0;

	while (message->length >= mes_value_curr_size + 4)
	{
		Parameter param[MAX_PARAMS];
		{ /* Получение данных об очередном параметре */
			recv(input,buf,4,0);
			param.type = char2_to_int(buf);	
			param.length = char2_to_int(&buf[2]);
			mes_value_curr_size += sizeof(param.type) + sizeof(param.length);
		}

		if (param.length + mes_value_curr_size > message->length)
			break;

		{ /* Получение значения параметра */
			param[i].value = calloc(param.length + 1);
			recv(input, param[i].value, param[i].length, 0);
		}
		i++;
	}
	{ /* Кладем принятые параметры в сообщение */
		message->parameter = malloc(i, sizeof(Parameter*));
		int j;
		for (j = 0; j < i; j++ )
		{
			message->parameter[j] = calloc(sizeof(Parameter));
			*message->parameter[j] = param[j];
			message->parameter[j].value = calloc(param[j].length + 1);
			strcpy(message->parameter[j].value, param[j].value);
			mes_value_curr_size += param[j].length;
		}
	}	
	return message;
}

char* unparse(Message* message)
{
	/**/
}	
