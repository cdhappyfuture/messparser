#ifndef _MESSPARSER_H_
#define _MESSPARSER_H_
#include "myheaders.h"

struct Parameter
{
	uint16_t type;
	uint16_t length;
	char* value;
};
	
struct Message
{
	Message();
	unsigned char protocol_version;
	uint16_t type;
	uint16_t length;
	std::vector<Parameter> parameter;
};

uint16_t char2_to_int(char* bytes);
char* int_to_char2(uint16_t var);

struct Message* parse(int);

#endif
