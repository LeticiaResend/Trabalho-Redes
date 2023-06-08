/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

void error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	int sockfd, portno, newsockfd;
	//int newsockfd, portno, clilen, cli_addr;
	struct sockaddr_in serv_addr;
	//int n;

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
		error("ERROR opening socket");

	memset(&serv_addr, 0, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	listen(sockfd,15);

	fd_set master;
	FD_ZERO(&master);

	FD_SET(sockfd, &master);

	while(1)
	{
		fd_set copy = master;

		int socketCount=select(FD_SETSIZE, &copy, NULL, NULL, NULL);

		for (int i=0; i < socketCount; i++)
		{
			int sock= copy.__fds_bits[i];
			if(sock == sockfd)
			{
				// Accept a new connection
				newsockfd=accept(sockfd, NULL, NULL);

				// Add a new connection to the list of connected clients
				FD_SET(newsockfd, &master);
				// Send welcome 
				char message[]="Welcome";
				send(newsockfd, message,strlen(message)+1,0 );
			}
			else
			{
				// Receive message
				char buffer[256];
				int bytesIn = recv(sock, buffer, 256, 0);

				if ( bytesIn <=0 )
				{
					//Drop the client
					close(sock);
					FD_CLR(sock, &master);
				}
				else
				{
					// Send message to the other clients (BROADCAST)
					for (int i=0; i< FD_SETSIZE; i++)
					{
						int outSock = master.__fds_bits[i];
						if(outSock!= sockfd && outSock!= sock){
							send(outSock, buffer, bytesIn, 0);
						}
					}

				}
			}
		}

	}


	// clilen = sizeof(cli_addr);
	// newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
	// if (newsockfd < 0)
	// 	error("ERROR on accept");

	// memset(buffer, 0, 256);
	// n = recv(newsockfd,buffer,255,0);
	// if (n < 0) error("ERROR reading from socket");

	// n = send(newsockfd,"I got your message",18,0);
	// if (n < 0) error("ERROR writing to socket");

	// printf("Here is the message: %s\n",buffer);

	return EXIT_SUCCESS;
}

