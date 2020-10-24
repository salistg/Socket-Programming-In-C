//////////////////////////  Server.c ////////////////

#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include<ctype.h>
#include<string.h>
#include<time.h>

#define MY_PORT		8989
#define MAXBUF		256

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET sockfd , clientfd;

    char recbuff[MAXBUF], sendbuff[MAXBUF], stringt[MAXBUF];
    int i, length;

    struct sockaddr_in self;
    struct tm * timeinfo;
    time_t rawtime; //a data type that stores system time values





    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
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
	/* bzero(&self, sizeof(self));*/
	self.sin_family = AF_INET;
	self.sin_port = htons(MY_PORT);
	self.sin_addr.s_addr = INADDR_ANY;

	/*---assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
	{
		perror("socket--bind");
		exit(errno);
	}

        puts("Bind done");

	/*---make it a "listening socket"---*/
	if ( listen(sockfd, 20) != 0 )
	{
		perror("socket--listen");
		exit(errno);
	}

        puts("Waiting for incoming connections...");

	/*--forever... ---*/
	while (1)
	{	struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);

		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

		//printing the information of the connection
		printf("\nInformation of this connection:\n");
		printf("IP address\n"); //printing the IP address
		printf("%s\n", inet_ntoa(client_addr.sin_addr));
		printf("Port number\n"); //printing the port number
		printf("%d", ntohs(client_addr.sin_port));

        //this will reset the buffer and helps avoid junk values
		memset(sendbuff, 0, MAXBUF);
		memset(recbuff, 0, MAXBUF);

		// a loop while the length of the message is greater than 0
		while(length > 0)
		{

			length = recv(clientfd, recbuff, MAXBUF, 0); //receiving a request from the client and assigning it to 'length'

            //to prevent the server from displaying the length of the enter click
			if (recbuff[0]!='\r'){
            //to display the length of the message on the server
            printf("\nThe length of the message: %d\n", length);
            }



			time(&rawtime); //time function
			timeinfo = gmtime(&rawtime);

            //if the received buffer is TIME then
			if(strncmp(recbuff, "TIME", 4) == 0){

				//if the user enters the command TIME PST
				if(strncmp(recbuff, "TIME PST", 8)== 0){

					timeinfo->tm_hour -= 8; //offset time is -8 hours behind
					mktime(timeinfo);
					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				} //if the user enters the command TIME MST
				else if(strncmp(recbuff, "TIME MST", 8)== 0){

					timeinfo->tm_hour -= 7;
					mktime(timeinfo);
					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				} //if the user enters the command TIME CST
				else if(strncmp(recbuff, "TIME CST", 8)== 0){

					timeinfo->tm_hour -= 6;
					mktime(timeinfo);
					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				} //if the user enters the command TIME EST
				else if(strncmp(recbuff, "TIME EST", 8)== 0){

					timeinfo->tm_hour -= 5;
					mktime(timeinfo);
					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				}//if the user enters the command TIME GMT
				else if(strncmp(recbuff, "TIME GMT", 8)== 0){

					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				}////if the user enters the command TIME CET
				else if(strncmp(recbuff, "TIME CET", 8)== 0){

					timeinfo->tm_hour += 1;
					mktime(timeinfo);
					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				} 	//if the user enters the command TIME MSK
				else if(strncmp(recbuff, "TIME MSK", 8)== 0){

					timeinfo->tm_hour += 3;
					mktime(timeinfo);
					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				} 	//if the user enters the command TIME JST
				else if(strncmp(recbuff, "TIME JST", 8)== 0){

					timeinfo->tm_hour += 9;
					mktime(timeinfo);
					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				} 	//if the user enters the command TIME AEST
				else if(strncmp(recbuff, "TIME AEST", 9)== 0){

					timeinfo->tm_hour += 10;
					mktime(timeinfo);
					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				} 	//if the user enters the command TIME
				else if((strcmp(recbuff, "TIME") == 0)||(strcmp(recbuff, "TIME ") == 0)){

					timeinfo = localtime(&rawtime);
					send(clientfd, asctime(timeinfo), (int)strlen(asctime(timeinfo)), 0); //sending the response back to the client

				} //if any other code is displayed, it will show ERROR
				else{

					strcpy(sendbuff, "ERROR");
					send(clientfd, sendbuff, (int)strlen(sendbuff), 0); //sending the response back to the client

				}
			}
			else{

				for(i = 0;recbuff[i] != '\0';++i){

					sendbuff[length - 1 - i] = toupper(recbuff[i]);

				}

				send(clientfd, sendbuff, length, 0);

			}


			memset(sendbuff, 0, MAXBUF);
			memset(recbuff, 0, MAXBUF);

		}

		/*---close connection---*/
		close(clientfd);
	}

	/*---clean up (should never get here!)---*/
	close(sockfd);
        WSACleanup();
	return 0;
}
