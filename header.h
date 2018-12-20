#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define PORT 20162
#define BUFFER_SIZE 4096
#define BUFF_SIZE 100 

typedef struct {
        char nickname[20];
        char data[BUFFER_SIZE];
} MsgType;
