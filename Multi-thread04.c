#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
void *connection_handler(void *);
int main(int argc , char *argv[])
{
int socket_desc , new_socket , c , new_sock[100];
struct sockaddr_in server , client;
char *message;
//Create socket
socket_desc = socket(AF_INET , SOCK_STREAM , 0);
if (socket_desc == -1)
{
printf("Could not create socket");
}
//Prepare the sockaddr_in structure
server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons( 8888 );
//Bind
if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
{
puts("bind failed");
return 1;
}
puts("bind done");
//Listen
listen(socket_desc , 3);
//Accept and incoming connection
puts("Waiting for incoming connections...");
c = sizeof(struct sockaddr_in);
pthread_t sniffer_thread[100];
int i=0;
while(1)
{
if((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))<0)
{
   puts("new_socket < 0");
   break;
}
puts("Connection accepted");
//Reply to the client
new_sock[i] = new_socket;
if( pthread_create( &sniffer_thread[i] , NULL ,  connection_handler , &new_sock[i]) < 0)
{
perror("could not create thread");
return 1;
}
//Now join the thread , so that we dont terminate before the thread
//pthread_join( sniffer_thread , NULL);
i++;
puts("Handler assigned");
}
puts("Over");
if (new_socket<0)
{
perror("accept failed");
return 1;
}
return 0;
}
/*
* This will handle connection for each client
* */
void *connection_handler(void *socket_desc)
{
//Get the socket descriptor
    int sock = *(int*)socket_desc;
    int n;
    char client_buffer[256],my_buffer[256],buffer[100];

    if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
    int read_size;
    int i , p , num ;
    int no[10] ;
    int guess[4] , tmp , counterA , counterB , j , k ,count=1;
 
     bzero(my_buffer,256);
//Send some messages to the client
     while(count>0)
    {
	    for ( i = 0 ; i < 10 ; ++i )
	            no[i] = i;
	
	    srand( time(NULL) ) ;
	    // 由電腦隨機產生一組四個不同的數字
	    for ( i = 0 ; i < 4 ; ++i ) {
	            p = rand() % 10 ;
	            if ( p == i ) continue ;
	            tmp = no[i] ;
	            no[i] = no[p] ;
	            no[p] = tmp ;
	    }
	    printf("thread %d ANS:",sock);
	    for(i=3;i>=0;i--)
	            printf("%d",no[i]);
	    printf("\n");
	    bzero(buffer,100);
   	    bzero(client_buffer,256);	
	    sprintf(buffer,"Game%d:Please enter your guess number : ",count);
            if(count==1)
            {
              if((n=write(sock , buffer , strlen(buffer)))<0)
              {
                perror("ERROR writing to socket");
                exit(1);
              }
            } 
	    else
            { 
              strcat(my_buffer,buffer);
              if((n=write(sock , my_buffer , strlen(my_buffer)))<0)
	      {
	        perror("ERROR writing to socket");
	        exit(1);		
	      }
            } 
	    int count2=0;
	    while(1)
	    {
	        // 把輸入的數字分解成 千位, 百位, 十位, 個位, 並把結果放入一個陣>列中
		if ((read_size = recv(sock, client_buffer, 256, 0))<=0)
		{
		   printf("thread %d is out of line\n",sock);
                   return 0;
                }
	        int i,pointer;
	        for(i=0,pointer=3;client_buffer[i]!=0&&pointer>=0;i++)
	        {
	            if(client_buffer[i]>='0'&&client_buffer[i]<='9')
	               guess[pointer--]=client_buffer[i]-'0';
	        }
	        // 判斷輸入數字與答案的符合程度
	        counterA = counterB = 0;
	        for( j = 0 ; j < 4 ; ++j )
	                for( k = 0 ; k < 4 ; ++k )
	                        if( no[j] == guess[k] )
	                                if( j == k )
	                                        counterA++;
	                                else
	                                        counterB++;
	
                 bzero(my_buffer,256);
	        if( counterA == 4 )
	        {
                  sprintf(my_buffer,"Right number!!\n");
                  count++;
                  break;
	        } 
		  else if(count2>4)
		{
                 sprintf(my_buffer,"Game over!!ANS is:%d%d%d%d\n",no[3],no[2],no[1],no[0]);
	          count++;
                  break;				
		}
		else {
	            sprintf(my_buffer,"Hint%d:%dA%dB",++count2,counterA,counterB); 			
	    }
	        //Send the message back to client
	        bzero(client_buffer,256);	
	        bzero(buffer,100);
	        sprintf(buffer,"\nGame%d:Please enter your guess number : ",count);
	        strcat(my_buffer,buffer);
	        if((n=write(sock , my_buffer , strlen(my_buffer)))<0)
		{
	           perror("ERROR writing to socket");
		   exit(1);		
	    } 
        }
	}   
//Free the socket pointer
pthread_exit(NULL);
return 0;
}
