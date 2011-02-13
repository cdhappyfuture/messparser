#include "myheaders.h"
#include "messparser.h"

void free_mes(Message* m)
{
	int i;
	for (i = 0; i < m->params; i++)
		free(m->parameter[i]->value);
	free(m->parameter);
	free(m);
}		
uint16_t char2_to_int(char* bytes) //рабочая
{
	uint16_t var = (uint16_t)bytes[1]*256 + (uint16_t)bytes[0];
	return var;
}

char* int_to_char2(uint16_t var)
{
	char* str = calloc(3, sizeof(char));
	var = var;
	str[0] = ((char*)&var)[0];
	str[1] = ((char*)&var)[1];
	str[2] = '\0';
	return str;	
}
Message* parse(int input)
{
	Message* message = malloc(sizeof(Message));
	char buf[100];
	{ /* Получение данных о message */
		recv(input,buf,5,0);
		message->protocol_version = (unsigned int)*buf;
		message->type = char2_to_int(&buf[1]);
		message->length = char2_to_int(&buf[3]);
	}

	int i = 0;
	int mes_value_curr_size = 0;

	Parameter param[MAX_PARAMS];
	while (1)
	{
		{ /* Получение данных об очередном параметре */
			recv(input,buf,4,0);
			param[i].type = char2_to_int(buf);	
			param[i].length = char2_to_int(&buf[2]);
			mes_value_curr_size += sizeof(param[i].type) + sizeof(param[i].length);
		}

		if (param[i].length + mes_value_curr_size > message->length)
			break;
		{ /* Получение значения параметра */
			param[i].value = malloc(param[i].length + 1);
			recv(input, param[i].value, param[i].length, 0);
		}
		if ((param[i].length + mes_value_curr_size) == message->length)
		{
			break;
		}
		i++;
	}
	{ /* Кладем принятые параметры в сообщение */
		message->params = i;
		message->parameter = calloc(i, sizeof(Parameter*));
		int j;
		for (j = 0; j <= i; j++ )
		{
			message->parameter[j] = malloc(sizeof(Parameter));
			*message->parameter[j] = param[j];
			message->parameter[j]->value = malloc(param[j].length + 1);
			memcpy((void*)message->parameter[j]->value, (void*)param[j].value, message->parameter[j]->length);
			mes_value_curr_size += param[j].length;
		}
	}	
	return message;
}

char* unparse(Message* message)
{
	int message_size;
	message_size = message->length + sizeof(message->length) + sizeof(message->protocol_version) + sizeof(message->type);
	char* str = calloc(message_size + 1, sizeof(char));
	str[0] = message->protocol_version;
	memcpy((void*)&str[1], (void*)&message->type, sizeof(message->type));
	memcpy((void*)&str[3], (void*)&message->length,  sizeof(message->length));
	int i,j = 5;
	for (i = 0; i < message->params; i++)
	{
		memcpy((void*)&str[j], (void*)&message->parameter[i]->type, sizeof(message->parameter[i]->type));
		j += sizeof(message->parameter[i]->type);
		memcpy((void*)&str[j], (void*)&message->parameter[i]->length, sizeof(message->parameter[i]->type));
		j += sizeof(message->parameter[i]->length);
		memcpy(&str[j], message->parameter[i]->value, message->parameter[i]->length);
		j += message->parameter[i]->length;
	}
	str[j] = '\0';
	return str;
}	
