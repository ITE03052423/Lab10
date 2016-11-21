#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h> //inet_addr
int main(int argc , char *argv[])
{
	int socket_desc;
	struct sockaddr_in server;
	char server_reply[2000], message[10];
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	int ProtNum;
	server.sin_addr.s_addr = inet_addr("127.0.0.2");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	puts("Connected\n");
	//Send some data
	memset(server_reply,0,sizeof(server_reply));
	if( recv(socket_desc, server_reply , 2000 , 0) > 0)
   {
       puts(server_reply); 
   }
   else puts("recv failed\n");
   memset(server_reply,0,sizeof(server_reply));
        memset(message, 0, sizeof(message));
	while(1)
	{
	//   scanf("%s",message);
	   gets(message);
	   if( send(socket_desc , message , strlen(message) , 0) < 0)
	   {
	      puts("Send failed");
	      return 1;
	   }
	   //Receive a reply from the server
	   memset(server_reply,0,sizeof(server_reply));
	   //fflush(stdin);
	   if( recv(socket_desc, server_reply , 2000 , 0) > 0)
	   {
	       puts(server_reply); 
	   }
            memset(message, 0, sizeof(message));
	}
	return 0;
}
