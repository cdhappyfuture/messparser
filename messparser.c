#include "messparser.h"

void free_mes(Message* m)
{
	int i;
	for (i = 0; i < m->params; i++)
		free(m->parameter[i]->value);
	free(m->parameter);
}		
uint16_t char2_to_int(char* bytes)
{
	uint16_t var = (unsigned int)bytes[0]*256 + (unsigned int)bytes[1];
	return var;
}

char* int_to_char2(uint16_t var)
{
	char* str = calloc(2, sizeof(char));
	str[0] = ((char*)&var)[0];
	str[1] = ((char*)&var)[1];
	return str;	
}
Message* parse(int input)
{
	Message* message = malloc(sizeof(Message));
	char buf[100];
	puts("Начинаем парсить");
	{ /* Получение данных о message */
		recv(input,buf,5,0);
		message->protocol_version = (unsigned int)*buf;
		message->type = char2_to_int(&buf[1]);
		message->length = char2_to_int(&buf[3]);
	}
	printf("Message length = %i\n", message->length);

	int i = 0;
	int mes_value_curr_size = 0;

	Parameter param[MAX_PARAMS];
	while (1)
	{
		puts("Начало while(1)");	
		{ /* Получение данных об очередном параметре */
			recv(input,buf,4,0);
			puts("Получили 4 байта");
			param[i].type = char2_to_int(buf);	
			param[i].length = char2_to_int(&buf[2]);
			printf("Param[%i] length = %i\n", i, param[i].length);
			mes_value_curr_size += sizeof(param[i].type) + sizeof(param[i].length);
		}

		if (param[i].length + mes_value_curr_size > message->length)
			break;
		puts("Получаем значение параметра");
		{ /* Получение значения параметра */
			param[i].value = malloc(param[i].length + 1);
			recv(input, param[i].value, param[i].length, 0);
		}
		puts("Получили");
		if ((param[i].length + mes_value_curr_size) == message->length)
		{
			puts("Длина сообщения сошлась");
			break;
		}
		printf("Длина сообщения не сошлась: message->length = %i, param[i]length = %i, cursize = %i", message->length, param[i].length, mes_value_curr_size);
		i++;
	}
	{ /* Кладем принятые параметры в сообщение */
		message->params = i;
		message->parameter = calloc(i, sizeof(Parameter*));
		int j;
		puts("Записываем данные в месаж");
		for (j = 0; j <= i; j++ )
		{
			message->parameter[j] = malloc(sizeof(Parameter));
			*message->parameter[j] = param[j];
			message->parameter[j]->value = malloc(param[j].length + 1);
			strcpy(message->parameter[j]->value, param[j].value);
			mes_value_curr_size += param[j].length;
		}
	}	
	return message;
}

char* unparse(Message* message)
{
	/**/
}	
