#include "header.h"

main(int argc, char** argv)
{
        struct sockaddr_in connectSocket;
        int connectFD=socket(AF_INET, SOCK_STREAM, 0);
        if (argc != 2)
        {
                printf("Usage: %s IPv4-address\n", argv[0]);
                exit(1);
        }
        memset(&connectSocket, 0, sizeof(connectSocket));
        connectSocket.sin_family=AF_INET;
        inet_aton(argv[1], (struct in_addr*) &connectSocket.sin_addr.s_addr);
        connectSocket.sin_port=htons(PORT);

        if(connect(connectFD, (struct sockaddr*) &connectSocket, sizeof(connectSocket)) == -1)
        {
                printf("Connect Error.\n");
                exit(1);
        }
        else
        {
                int readBytes, writtenBytes;
                char receiveBuffer[BUFFER_SIZE];
                MsgType msg;

                printf("닉네임 : ");
                fgets(msg.nickname, 20, stdin);
                msg.nickname[strlen(msg.nickname) - 1] = '\0';
                while (1)
                {
                        //서버에 문자열을 보낸 뒤 서버가 보낸 echo를 받아 출력.
                        printf("보낼 말 : ");
                        fgets(msg.data,BUFF_SIZE,stdin);
                        write(connectFD,(char *)&msg, sizeof(msg));
                }
        }
        close(connectFD);
}
