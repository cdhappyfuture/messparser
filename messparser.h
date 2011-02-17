#ifndef _MESSPARSER_H_
#define _MESSPARSER_H_
#include "myheaders.h"
#define MAX_PARAMS 10

struct _Parameter
{
	uint16_t type;
	uint16_t length;
	char* value;
};
typedef struct _Parameter Parameter;

struct _Message
{
	unsigned char protocol_version;
	uint16_t type;
	uint16_t length;
	int params;
	Parameter** parameter;
};
typedef struct _Message Message;

typedef struct timeval Time;

uint16_t char2_to_int(char* bytes);
char* int_to_char2(uint16_t var);


void free_mes(Message*);
Message* recv_and_deserialize(int);
void serialize_and_send(Message*, int);

#endif
