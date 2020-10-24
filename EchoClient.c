#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include<ctype.h>

#define MY_PORT		8989
#define MAXBUF		256

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET sockfd , clientfd;
   	struct sockaddr_in self;
   	struct sockaddr_in serv_addr;
	char buffer[MAXBUF];

	printf("\nInitialising Winsock...");

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Initialisation failed.");
        printf("Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

	/*---create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		exit(errno);
	}

        printf("Socket created.\n");

	/*---initialize address/port structure---*/
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(MY_PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");



if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection failed.\n");
        return -1;
    }

    for(;;)
    {
            memset(buffer,0,MAXBUF);

            printf("Message:\n");
            gets(buffer);

            if( strcmp(buffer,"QUIT")==0 || strcmp(buffer,"quit")==0 )
            {
                printf("Client terminated.\n");
                return -1;
            }
            else
            {
                send(sockfd, buffer, strlen(buffer), 0);
                recv(sockfd, buffer, MAXBUF, 0);
                printf("Server's response : %s\n", buffer);

            }
    }

    close(sockfd);
	WSACleanup();

	return 0;
}
