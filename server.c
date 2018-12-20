#include "header.h"
#define LISTEN_QUEUE_SIZE 5

void childHandler(int signal)
{
        int status;
        pid_t spid;

        //WNOHANG : 자식 프로세스가 종료되지 않아도 부모는 자신 일 한다.
        while((spid = waitpid(-1, &status, WNOHANG)) > 0)
        {
                printf("PID         : %d\n", spid);
                printf("Exit Value  : %d\n", WEXITSTATUS(status));
                printf("Exit Stat   : %d\n", WIFEXITED(status));
        }
}

main()
{
        //childHandler 함수가 SIGCHLD 시그널을 처리할 수 있도록 시그널 설치
        signal(SIGCHLD, (void *)childHandler);
        struct sockaddr_in listenSocket;

        memset(&listenSocket, 0, sizeof(listenSocket));
        listenSocket.sin_family=AF_INET;
        listenSocket.sin_addr.s_addr=htonl(INADDR_ANY);
        listenSocket.sin_port=htons(PORT);
        int listenFD = socket(AF_INET, SOCK_STREAM, 0);
        int connectFD;

        ssize_t receivedBytes;
        char readBuff[BUFFER_SIZE];
        char sendBuff[BUFFER_SIZE];
        pid_t pid;
        MsgType msg;
        char serverNickname[20];

        if (bind(listenFD, (struct sockaddr *) &listenSocket, sizeof(listenSocket)) == -1) {
                printf("Bind Error.\n");
                exit(1);
        }
        if (listen(listenFD, LISTEN_QUEUE_SIZE) == -1) {
                printf("Listen fail.\n");
                exit(1);
        }
        printf("Waiting for clients...\n");

        while (1)
        {
                struct sockaddr_in connectSocket, peerSocket;
                socklen_t connectSocketLength=sizeof(connectSocket);
                while((connectFD = accept(listenFD, (struct sockaddr*)&connectSocket, (socklen_t *)&connectSocketLength)) >= 0)
                {
                        getpeername(connectFD, (struct sockaddr*)&peerSocket, &connectSocketLength);
 			char peerName[sizeof(peerSocket.sin_addr) + 1] = { 0 };
                        sprintf(peerName, "%s", inet_ntoa(peerSocket.sin_addr));

                        // 접속이 안되었을 때 출력 x
                        if(strcmp(peerName,"0.0.0.0") != 0)
                        printf("Client : %s\n", peerName);

                        if (connectFD < 0)
                        {
                                printf("Server: accept failed\n");
                                exit(1);
                        }
                        // 클라이언트가 접속할 때마다 fork를 통해 child process를 생성해 echo를 발생.
                        pid = fork();


                        // 자식 서버일 때
                        if(pid == 0)
                        {
                                // 리스닝 소켓은 닫아준다.
                                close(listenFD);
                                ssize_t receivedBytes;

                                // read할 값이 있다면 계속 읽어들인다.
                                while((receivedBytes = read(connectFD, (char *)&msg, sizeof(msg))) > 0)
                                {
                                        printf("%lu bytes read\n", receivedBytes);
                                        printf("[%s] : %s\n", msg.nickname, msg.data);
                                        write(connectFD, (char *)&msg, sizeof(msg));
                                }
                                // 클라이언트가 종료되면 해당 자식 프로세스의 커넥팅 소켓도 종료
                                close(connectFD);
                                exit(0);
                        }
                        // 부모 서버라면 커넥팅 소켓을 닫아준다. 
                        else
                                close(connectFD);
                }

        }
        close(listenFD);
}
