// Client side C program for socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
// define Port No
#define PORT 8888
#define size 1000000
void yellow()
{
	printf("\033[0;33m");
}
void purple()
{
	printf("\033[1;35m");
}
void white()
{
	printf("\033[1;37m");
}

int get_message(char name[], char *buffer, int p)
{
	FILE *file = fopen(name, "r");
	// FILE *fp = fopen(name, "r");
	// fseek(fp, 0, SEEK_END); 
	if (file != NULL)
	{
		// append the new line character at the end of the file if it is not there
		// if(ftell(fp)==0)
		// {	p = 0;
		// 	return p;
		// }
		// fseek(fp, 0, SEEK_SET); 
		// fclose(fp);
		

		FILE *fileptr;
		fileptr = fopen(name, "a+");
		fseek(fileptr, -1, SEEK_END);
		if (fgetc(fileptr) != '\n')
			fprintf(fileptr, "\n");
		fclose(fileptr);

		size_t newLen = fread(buffer, sizeof(char), size, file);
		if (ferror(file) != 0)
		{
			fputs("Error reading file", stderr);
		}
		else
		{
			buffer[newLen++] = '\0';
		}
		fclose(file);
	}
	else
	{
		p = 0;
		return p;
	}
	return p;
}

void put_message(char name[], char *buffer)
{
	FILE *file = fopen(name, "w");
	fputs(buffer, file);
	fclose(file);
}

int receive_all(int sock, char *output, int maxlen)
{
	int arg = 0;
	int total = 0;
	bool stop = false;
	do
	{
		int valread = read(sock, output + total, maxlen - total);
		if (valread < 0)
			return -1;
		else if (valread == 0)
			return 0;
		total = total + valread;
		if (output[total - 1] == '#')
			stop = true;

	} while (!stop);
	output[total - 1] = '\0';
	return total - 1;
}

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;

	int cid=shmget(IPC_PRIVATE,sizeof(int),0777 | IPC_CREAT);
	int cid2=shmget(IPC_PRIVATE,sizeof(int),0777 | IPC_CREAT);
	int *x=shmat(cid,0,0);
	int *x1=shmat(cid2,0,0);
	*x=0;
	*x1=0;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	char buff[1024];
	bzero(buff, 1024);
	valread = read(sock, buff, 1024);
	yellow();
	printf("\n%s\n", buff);
	if (buff[0] == '^')
	{
		exit(0);
	}
	while (1)
	{
		char str[256],message[256];
		bzero(message, 256);
		char *hello;

		int cid=fork();
		if(cid==0)
		{	char msg[256];
			bzero(msg, 256);
			valread = read(sock, msg, 256);
			*x1=1;
			if(msg[0]=='~')
			{ 	(*x)=1;
				yellow();
				printf("\nEnter The Response in (YES/NO )only\n");
			}
			exit(0);
		}
		white();
		printf("\nPlease write the query which you want to be performed by server\n");

		fgets(str, 256, stdin);
		valread = send(sock, str, strlen(str), 0);
		while((*x1)!=1 && (*x) !=1) 
		{

		}
		*x1=0;

		char s[256], s1[256], s2[256], s3[256];
		int k1=0;
		int k2=0;
		int k3=0;

		yellow();
		if((*x)==1)
		{	(*x)=0;
			if(strcmp(str,"YES\n")==0)
				printf("\nYou Granted  the permission to server\n\n");
			else
				printf("\nYou denied the Permission to server\n\n");
			continue;
		}

		//list of all users
		if(strcmp(str,"/users\n")==0)
		{ 
			char *buffer;
			buffer = (char *)malloc(sizeof(char) * size);
			bzero(buffer, size);

			valread = receive_all(sock, buffer, size);
			yellow();
			printf("\nHere is The List Of All Active Clients-Id\n");
			purple();
			printf("\n%s\n\n", buffer);
			free(buffer);
		}
		//list of all files
		else if(strcmp(str,"/files\n")==0)
		{
			char *buffer;
			buffer = (char *)malloc(sizeof(char) * size);
			bzero(buffer, size);

			valread = receive_all(sock, buffer, size);
			yellow();
			printf("\nHere is The Details Of All Files that Server holds\n");
			purple();
			if(strlen(buffer)==1)
			{
				printf("\nThere is no File at the Server\n\n");
			}
			else
				printf("\n%s\n\n", buffer);
			free(buffer);
		}
		//Upload the file
		else if(1 == sscanf(str, "/upload %[^ \n]%[^\n]", s, s1))
		{
			char *buffer;
			buffer = (char *)malloc(sizeof(char) * size);
			bzero(buffer, size);
			char msg5[256];
            bzero(msg5,256);

			int p = 1;
			if (1 == get_message(s, buffer, p))
			{
				strcat(buffer, "#");
				valread = send(sock, buffer, strlen(buffer), 0);
				read(sock, msg5, 256);
				printf("\n%s\n",msg5);
			}
			else
			{	yellow();
				hello = "^Check your file Name: Either It is empty or worng name#";
				printf("\n%s\n\n", hello);
				valread = send(sock, hello, strlen(hello), 0);
			}
			free(buffer);
		}
		//Download the file
		else if(1 == sscanf(str, "/download %[^ \n]%[^\n]", s, s1))
		{
			char *buffer;
			buffer = (char *)malloc(sizeof(char) * size);
			bzero(buffer, size);

			valread = receive_all(sock, buffer, size);
			if (buffer[0] == '^')
			{
				printf("\n%s\n\n", buffer);
			}
			else
			{	
				printf("Successfully downloaded\n");
				put_message(s, buffer);
			}
			free(buffer);
		}
		//Invite
		else if(3 == sscanf(str, "/invite %[^ ] %d %[^ \n]%[^\n]", s, &k1,s2,s3) && (strcmp(s2,"V")==0 ||strcmp(s2,"E")==0))
		{
			//Nothing is there to execute in this part.
			char *buffer;
			buffer = (char *)malloc(sizeof(char) * size);
			bzero(buffer, size);

			valread = receive_all(sock, buffer, size);
			printf("\n%s\n\n", buffer);
			free(buffer);
		}
		//Line Content
		else if(3 == sscanf(str, "/read %[^ ] %d %d%[^\n]", s, &k1,&k2,s3)||2 == sscanf(str, "/read %[^ ] %d %[^\n]", s,&k1,s3)||1 == sscanf(str, "/read %[^ \n] %[^\n]", s,s3))
		{
			char *buffer;
			buffer = (char *)malloc(sizeof(char) * size);
			bzero(buffer, size);

			valread = receive_all(sock, buffer, size);
			if (buffer[0] == '^')
			{
				printf("\n%s\n\n", buffer);
			}
			else
			{
				printf("\nSuccessfully fetched\nIntended Content:\n");
				printf("\n%s\n\n", buffer);
			}
			free(buffer);
		}
		//Insert Message
		else if(3 == sscanf(str, "/insert %[^ ] %d \"%[^\"]\" %[^\n]", s, &k1,s2,s3)||2 == sscanf(str, "/insert %[^ ] \"%[^\"]\" %[^\n]", s,s2,s3))
		{
			char *buffer;
			buffer = (char *)malloc(sizeof(char) * size);
			bzero(buffer, size);

			char *buffer5;
			buffer5 = (char *)malloc(sizeof(char) * size);
			bzero(buffer5, size);

			valread = receive_all(sock, buffer, size);
			if (buffer[0] == '^')
			{
				printf("\n%s\n\n", buffer);
			}
			else
			{
				printf("\n%s\n\n", buffer);
				
				valread = receive_all(sock, buffer5, size);
				printf("\n%s\n\n", buffer5);
			}
			free(buffer);
		}
		//Delete Lines
		else if(3 == sscanf(str, "/delete %[^ ] %d %d%[^\n]", s,&k1,&k2,s3) || 2 == sscanf(str, "/delete %[^ ] %d%[^\n]", s,&k1,s3)||1 == sscanf(str, "/delete %[^ \n] %[^\n]", s,s3))
		{
			char *buffer;
			buffer = (char *)malloc(sizeof(char) * size);
			bzero(buffer, size);

			char *buffer5;
			buffer5 = (char *)malloc(sizeof(char) * size);
			bzero(buffer5, size);

			valread = receive_all(sock, buffer, size);
			if (buffer[0] == '^')
			{
				printf("\n%s\n\n", buffer);
			}
			else
			{
				printf("\n%s\n\n", buffer);

				valread = receive_all(sock, buffer5, size);
				printf("\n%s\n\n", buffer5);
			}
			free(buffer);
		}
		//Exit
		else if(strcmp(str,"/exit\n")==0 || 1 == sscanf(str, "/exit%[ ]%[^\n]", s, s1))
		{
			printf("Client Program Exit:");
			break;
		} 
		//wrong Query
		else
		{
			bzero(message, 256);
			valread = read(sock, message, 256);
			printf("\n%s\n\n", message);
		}
	}
	return 0;
}
