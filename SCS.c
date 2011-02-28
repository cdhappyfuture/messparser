#include <stdio.h>
#include "myheaders.h"
#include "messparser.h"
#include "messhandler.h"
#include "messages.h"
#include <arpa/inet.h>
#define do_once for(m = 0; m < 1; m++)

int main(int argc, char** argv)
{
    int sock;
    struct sockaddr_in server_addr;	

    char msg_from_server[MAX_MSG_LENGTH];
    char* msg_to_server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        puts("socket creation error");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6667);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)& server_addr, sizeof(server_addr)) < 0)
    {
        puts("connection error");
        return 2;
    }

    f_channel_setup(sock);
    int m;
    do_once
    {
        Message* message = recv_and_deserialize(sock);
        if (message->type == channel_status && char2_to_int(message->parameter[0]->value) == 1)
        {
            puts("Channel 1 setup success. Sending channel_test...");
            f_channel_test(sock);
            if (message = recv_and_deserialize(sock))
            {
                if (message->type == channel_status && char2_to_int(message->parameter[0]->value) == 1)
                {
                    puts("Channel is active. Sending stream setup...");
                    f_stream_setup(sock, 7, 1);
                    if (message = recv_and_deserialize(sock))
                    {
                        if (message->type == stream_status)
                        {
                            puts("Income message: stream status. Sending stream_close_request");
                            f_stream_close_request(sock, 1);
                            if (message = recv_and_deserialize(sock))
                            {
                                if (message->type == stream_close_response)
                                {
                                    puts("Income message: stream_close_response. Sending channel_close...");
                                    //closingstream;
                                    f_channel_close(sock);
                                    puts("Stop work with server.");
                                    continue;
                                }
                            }
                        }
                    }
                }
            }
            puts("Error in income message. Other message expected. Sending channel_close...");
            f_channel_close(sock);
        }
        else
        {
            puts("Channel 1 setup fail");
            puts("Sending channel_close...");
            f_channel_error(sock, unrecoverable_error);
        }
    }
    while(1);
    puts("Shutting down...");
    close(sock);
    return 0;
}

