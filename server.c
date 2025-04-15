// This file contains the functionality of the Server side C Program for Socket programming which handle multiple clients.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <stdbool.h>
#define size 1000000
#define TRUE 1
#define FALSE 0
#define PORT 8888

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

typedef struct file_details file_details;
typedef struct invitation invitation;
struct file_details
{
    char f_name[256];
    int owner;
    int collaboration[10];
    char permission[10];
    int valid;
};
int track1=0;
file_details arr[256];
struct invitation
{
    char f_name[256];
    int clt;
    char permission;
};
int track2=0;
invitation arr2[256];

int total_lines(char name[])
{
    // append the new line character at the end of the file if it is not there
    FILE *fileptr;
    fileptr = fopen(name, "a+");
    fseek(fileptr, -1, SEEK_END);
    if (fgetc(fileptr) != '\n')
        fprintf(fileptr, "\n");
    fclose(fileptr);
    // Find the total no of lines in the file
    int count = 0;
    char chr;
    FILE *file2;
    file2 = fopen(name, "r");
    chr = getc(file2);
    while (chr != EOF)
    {
        if (chr == '\n')
        {
            count = count + 1;
        }
        chr = getc(file2);
    }
    fclose(file2);
    return count;
}

int get_message(char name[], char *buffer, int p)
{
	FILE *file = fopen(name, "r");
	if (file != NULL)
	{
		// append the new line character at the end of the file if it is not there
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
void get_message_without(char name[], char *buffer)
{
	FILE *file = fopen(name, "r");
	if (file != NULL)
	{
		// append the new line character at the end of the file if it is not there
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

void lines(char buffer[],char s[],int k1,int k2,int new_socket)
{
    int count;
    count=total_lines(s);
    printf("Count=%d\n",count);
    if( k1<count && k1>=-count && k2<count && k2>=-count)
    {	FILE* file = fopen(s, "r"); 
        FILE* file1=fopen("temp1.txt","w");
        char line[1024]; 
        int j=0;
        if(k1>=-count && k1<0)
        {	k1=count-(-k1);
        }
        if(k2>=-count && k2<0)
        {	k2=count-(-k2);
        }
        while (fgets(line, sizeof(line), file)) 
        { 	
            if(j == k1 ) 
            { 
                fprintf(file1,"%s",line);
                j=j+1;
                while (j<=k2)
                {
                    fgets(line, sizeof(line), file);
                    fprintf(file1,"%s",line);
                    j++;
                }
                break;     
            } 
            j++; 
        } 
        fclose(file);
        fclose(file1);
        bzero(buffer,size);
        get_message_without("temp1.txt",buffer);
        remove("temp1.txt");
    }
    else
    {	strcat(buffer,"Wrong Query Received-Index OUT OF RANGE-Check your K value(line no)");
    }
}
void insert(char buffer[],char s[],int k, char msg[],int new_socket)
{   
    int count;
    count=total_lines(s);
    printf("count==%d",count);
    if (k<count&& k>=-count)
    {	
        FILE* file1 = fopen(s, "r"); 
        FILE* file2 = fopen("temp2.txt", "w"); 
        char line[1024]; 
        if(k>=-count && k<0)
        {	k=count-(-k);
        }
        int j=0;
        while (j<k) 
        { 	
            fgets(line, sizeof(line), file1);
            fputs(line,file2);
            j++; 
        }
        strcat(msg,"\n");
        fputs(msg,file2);
        
        while (j<count) 
        { 	
            fgets(line, sizeof(line), file1);
            fputs(line,file2);
            j++; 
        } 
        fclose(file1);
        fclose(file2);
        remove(s);
        rename("temp2.txt",s);
        strcat(buffer,"Successfully Edited");
    }
    else
    {	
        strcat(buffer,"Wrong Query Received-Index OUT OF RANGE-Check your K value(line no).");
    }
    
}

void insert_end(char buffer[],char s[],int k, char msg[],int new_socket)
{   
    int count;
    count=total_lines(s);
    k=count;
    FILE* file1 = fopen(s, "r"); 
    FILE* file2 = fopen("temp2.txt", "w"); 
    char line[1024]; 
    int j=0;
    while (j<k) 
    { 	
        fgets(line, sizeof(line), file1);
        fputs(line,file2);
        j++; 
    }
    strcat(msg,"\n");
    fputs(msg,file2); 
    fclose(file1);
    fclose(file2);
    remove(s);
    rename("temp2.txt",s);
    strcat(buffer,"Successfully Edited");
}

void delete(char buffer[],char s[],int k1,int k2,int new_socket)
{
    int count;
    count=total_lines(s);
    printf("Count=%d\n",count);
    if( k1<count && k1>=-count && k2<count && k2>=-count)
    {	FILE* file = fopen(s, "r"); 
        FILE* file1=fopen("temp1.txt","w");
        char line[1024]; 
        if(k1>=-count && k1<0)
        {	k1=count-(-k1);
        }
        if(k2>=-count && k2<0)
        {	k2=count-(-k2);
        }
        int j=0;
        while (j<k1) 
        { 	
            fgets(line, sizeof(line), file);
            fputs(line,file1);
            j++; 
        }
        while (j<=k2) 
        { 	
            fgets(line, sizeof(line), file);
            j++; 
        }
        while (j<count) 
        { 	
            fgets(line, sizeof(line), file);
            fputs(line,file1);
            j++; 
        } 
        fclose(file);
        fclose(file1);
        remove(s);
        rename("temp1.txt",s);
        strcat(buffer,"Successfully Deleted");
    }
    else
    {	strcat(buffer,"Wrong Query Received-Index OUT OF RANGE-Check your K value(line no)");
    }
}

void users(char buffer[], int client_socket[])
{   int ip;
    FILE* file1=fopen("temp1.txt","w");
    yellow();
    fprintf(file1,"S.No\tSocket_No\tClient_Id\n");
    fprintf(file1,"----------------------------------\n");

    for (ip = 0; ip < 5; ip++)
    {
        // if position is empty
        if (client_socket[ip] != 0)
        {
            fprintf(file1,"%d\t%d\t\t%d\n",ip+1,client_socket[ip],client_socket[ip]+10000);
        }
    }
    fclose(file1);
    bzero(buffer,size);
    get_message_without("temp1.txt",buffer);
    remove("temp1.txt");
}

void files_info(char buffer[])
{   int i,j,k;
    int count=0;
    int count1=0;
    FILE* file1=fopen("temp1.txt","w");

    fprintf(file1,"S.No\tFile_Name\tOwner_ID\tTotal_Lines\tCollaborator_ID and Permission\n");
    fprintf(file1,"--------------------------------------------------------------------------------------\n");
    for(i=0; i<track1;i++)
    {   
        if(arr[i].valid==1)
        {
            count1 +=1;
            count=total_lines(arr[i].f_name);
            fprintf(file1,"%d\t%s\t\t%d\t\t%d\t\t",count1,arr[i].f_name,arr[i].owner+10000,count);
            j=0;
            k=0;
            while(j<10)
            {   
                if(arr[i].collaboration[j]!=0)
                {
                    k=1;
                    fprintf(file1,"%d--%c\n",arr[i].collaboration[j]+10000,arr[i].permission[j]);
                    fprintf(file1,"\t\t\t\t\t\t\t");
                }
                j=j+1;
            }
            if(k==0)
            {
                fputs("No Colloborator for this File",file1);
            }
            fputs("\n",file1);
        }
    }
    fclose(file1);
    bzero(buffer,size);
    get_message_without("temp1.txt",buffer);
    remove("temp1.txt");
}
void fill_details(char name[],int socket)
{   int i,j;
    strcpy(arr[track1].f_name,name);
    arr[track1].owner= socket;
    arr[track1].valid=1;
    for(i=0;i<10;i++)
    {
        arr[track1].collaboration[i]=0;
        arr[track1].permission[i]='V';
    }
    track1=track1+1;
}

void add_permission(char name[],int sd,char s)
{   int i,j;
    for (i=0;i<track1;i++)
    {   
        if(arr[i].valid==1)
        {
            if(strcmp(name,arr[i].f_name)==0)
            {
                j=0;
                while(arr[i].collaboration[j]!=0 && arr[i].collaboration[j] !=sd )
                {  
                    j=j+1;
                }
                arr[i].collaboration[j]=sd;
                arr[i].permission[j]=s;
                break;
            }
        }
    }
}

int check_dup(char name[])
{
    int i=0;
    for (i=0; i<track1;i++)
    {
        if(arr[i].valid==1)
        {
            if(strcmp(name,arr[i].f_name)==0)
            {
                return 0;
            }
        }
    }
    return 1;
}

int is_owner(int socket,char name[])
{
    int i=0;
    for (i=0; i<track1;i++)
    {
        if(arr[i].valid==1)
        {
            if(strcmp(name,arr[i].f_name)==0)
            {
                if(socket==arr[i].owner)
                {
                    return 1;
                }
                break;
            }
        }
    }
    return 0;
}

int has_E_permission(int socket, char name[])
{
    int i=0;
    int j=0;
    for (i=0; i<track1;i++)
    {
        if(arr[i].valid==1)
        {
            if(strcmp(name,arr[i].f_name)==0)
            {
                j=0;
                while(j<10)
                {
                    if(arr[i].collaboration[j]==socket && arr[i].permission[j]=='E')
                        return 1;
                    j=j+1;
                }
                break;
                
            }
        }
    }
    return 0;
}
int has_V_permission(int socket, char name[])
{
    int i=0;
    int j=0;
    for (i=0; i<track1;i++)
    {
        if(arr[i].valid==1)
        {
            if(strcmp(name,arr[i].f_name)==0)
            {
                j=0;
                while(j<10)
                {
                    if(arr[i].collaboration[j]==socket && arr[i].permission[j]=='V')
                        return 1;
                    j=j+1;
                }
                break;  
            }
        }
    }
    return 0;

}
int is_file(char name[])
{
    int i=0;
    for (i=0; i<track1;i++)
    {
        if(arr[i].valid==1)
        {
            if(strcmp(name,arr[i].f_name)==0)
            {
                return 1;
            }
        }
    }
    return 0;
}
void erase_data(int socket)
{
    int i=0;
    int j=0;
    for (i=0; i<track1;i++)
    {
        if(arr[i].owner==socket)
        {
            arr[i].valid=0;
        }
        j=0;
        while(j<10)
        {   
            if(arr[i].collaboration[j]==socket)
            {   arr[i].collaboration[j]=0;
                arr[i].permission[j]='X';
            }
            j=j+1;
        }
    }  
}

void new_line(char str[])
{
    int i=0;
    for(i=0;str[i]!='\0';i++)
    {
        if(str[i]=='^')
        {
            str[i] = '\n';
        }
    }
}

int main(int argc, char *argv[])
{
    int opt = TRUE;
    int master_socket, addrlen, new_socket, client_socket[5],
        max_clients = 5, activity, ip, valread, sd;
    int max_sd;
    struct sockaddr_in address;
    // set of socket descriptors
    fd_set readfds;
    char buff[1024];
    bzero(buff, 1024);
    // initialise all client_socket[] to 0 so not checked
    for (ip = 0; ip < max_clients; ip++)
    {
        client_socket[ip] = 0;
    }

    // create a master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    white();
    printf("\nListener on port no : %d \n", PORT);

    // try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // accept the incoming connection
    addrlen = sizeof(address);
    puts("It is Waiting for connections .........");

    while (TRUE)
    {
        // clear the socket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        // add child sockets to set
        for (ip = 0; ip < max_clients; ip++)
        {
            // socket descriptor
            sd = client_socket[ip];

            // if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            // highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }

        // wait for an activity on one of the sockets , timeout is NULL ,
        // so wait indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }

        // If something happened on the master socket ,
        // then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            // add new socket to array of sockets
            for (ip = 0; ip < max_clients; ip++)
            {
                // if position is empty
                if (client_socket[ip] == 0)
                {
                    client_socket[ip] = new_socket;
                    break;
                }
            }
            if (ip == max_clients) // if we could not handle more clients
            {
                purple();
                strcpy(buff, "^Server cannont handle any more connections.\n");
                send(new_socket, buff, 1024, 0);
                yellow();
                printf("\n%s\n", buff);
                close(new_socket);
            }
            else
            {
                bzero(buff, 1024);
                purple();
                strcpy(buff, "This client is connected to Connected\n");
                send(new_socket, buff, 1024, 0);
                bzero(buff, 1024);
                // inform user of socket number - used in send and receive commands
                yellow();
                printf("\nNew connection is Receiced\n\tSocket fd is: %d\n\tIP address is: %s\n\tPort No is: %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                printf("\nAdding to list of sockets as %d\n", ip);
            }
        }
        // else its some IO operation on some other socket
        for (ip = 0; ip < max_clients; ip++)
        {
            sd = client_socket[ip];
            if (FD_ISSET(sd, &readfds))
            {
                // Check if it was for closing , and also read the
                // incoming message
                char str[256],msg2[256];
                bzero(str, 256);
                bzero(msg2, 256);
                if ((valread = read(sd, str, 256)) == 0)
                {
                    // Somebody disconnected , get his details and print
                    getpeername(sd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen);
                    yellow();
                    printf("\nHost disconnected , ip %s , port %d \n",
                           inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    // Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[ip] = 0;
                }
                // Echo back the message that came in
                else
                {   
                    white();
                    printf("\n The Recevied Query/Response by the server:\n %s",str);

                    if(strcmp(str,"YES\n")!=0 && strcmp(str,"NO\n")!=0)
                    { 
                        strcpy(msg2,"Just a Test Message\n");
                        send(sd, msg2, strlen(msg2), 0);
                        bzero(msg2,256);
                    }
                    char *hello;
                    char s[256], s1[256], s2[256], s3[256],msg4[256];
                    int k1=0,k2=0,k3=0;
                    int p;
                    yellow();
                    if(strcmp(str,"YES\n")==0)
                    {
                        add_permission(arr2[track2-1].f_name,arr2[track2-1].clt,arr2[track2-1].permission);
                        printf("\nSuccesfully Updated the permission\n");
                        continue;
                    }
                    if(strcmp(str,"NO\n")==0)
                    {
                        printf("\nClient denied the Invite permission update\n");
                        continue;
                    }

                    if(strcmp(str,"/users\n")==0)
                    {  
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);
                        users(buffer,client_socket);
                        strcat(buffer, "#");
                        valread = send(sd, buffer, strlen(buffer), 0);
                        printf("\n Successfully Fetchted the all Active Clients\n");
                        free(buffer);
                    }
                    else if(strcmp(str,"/files\n")==0)
                    {
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);
                        files_info(buffer);
                        strcat(buffer, "#");
                        valread = send(sd, buffer, strlen(buffer), 0);
                        printf("\n Successfully Fetchted the all Active File Details\n");
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

                        valread = receive_all(sd, buffer, size);
                        if (buffer[0] == '^')
                        {
                            printf("\n%s\n\n", buffer);
                        }
                        else
                        {
                            if(check_dup(s))
                            {   
                                strcpy(msg5,"Successfully Uploaded the File");
                                printf("\n%s\n",msg5);
                                put_message(s, buffer);
                                fill_details(s,sd);
                                valread = send(sd, msg5, strlen(msg5), 0);

                            }
                            else
                            {
                                strcpy(msg5,"Same File Name already exist");
                                printf("\n%s\n",msg5);
                                valread = send(sd, msg5, strlen(msg5), 0);

                            }
                            
                        }
                        free(buffer);
                    }
                    //Download the file
                    else if(1 == sscanf(str, "/download %[^ \n]%[^\n]", s, s1))
                    {
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);

                        if(is_file(s) && (has_E_permission(sd,s) || is_owner(sd,s)))
                        {
                            int p = 1;
                            if (1 == get_message(s, buffer, p))
                            {
                                strcat(buffer, "#");
                                valread = send(sd, buffer, strlen(buffer), 0);
                            }
                            else
                            {
                                hello = "^Check your file Name: Either It is empty or worng name#";
                                printf("\n%s\n\n", hello);
                                valread = send(sd, hello, strlen(hello), 0);
                            }
                        }
                        else
                        {
                            hello = "^Client dont have permission to download this file or there is no file at the server #";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);

                        } 
                        free(buffer);
                    }
                    //Invite
                    else if(3 == sscanf(str, "/invite %[^ ] %d %[^ \n]%[^\n]", s, &k1,s2,s3) && (strcmp(s2,"V")==0 ||strcmp(s2,"E")==0))
                    {  
                        char *buffer;
                        char msg3[256];
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);

                        if( is_file(s) && is_owner(sd,s) && sd !=k1-10000 )
                        {   
                            hello = "^Client have right to give  permission to this file#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);

                            bzero(msg3,256);
                            strcpy(arr2[track2].f_name,s);
                            arr2[track2].clt=k1-10000;
                            arr2[track2].permission=s2[0];
                            track2=track2+1;
                            strcpy(msg3,"~Do you to take permission give answer yes or no");
                            valread = send(k1-10000, msg3, strlen(msg3), 0);
                        }
                        else
                        {
                            hello = "^Client dont have right to give  permission to this file(Not Owner or Same client id or File doesnot exist)#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);
                        }
                        free(buffer);
                    }
                    else if(3 == sscanf(str, "/read %[^ ] %d %d%[^\n]", s, &k1,&k2,s3))
                    {
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);
                        if( is_file(s) && (is_owner(sd,s) || has_E_permission(sd,s)|| has_V_permission(sd,s)))
                        {
                            lines(buffer,s,k1,k2,sd);
                            strcat(buffer, "#");
                            valread = send(sd, buffer, strlen(buffer), 0);    
                        }
                        else
                        {
                            hello = "^Client dont  have rights to access this file or File doesnot exist#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);

                        }
                        free(buffer);
                    }
                    else if(2 == sscanf(str, "/read %[^ ] %d %[^\n]", s, &k1,s3))
                    {
                        k2=k1;
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);
                        if( is_file(s) && (is_owner(sd,s) || has_E_permission(sd,s)|| has_V_permission(sd,s)))
                        {
                            lines(buffer,s,k1,k2,sd);
                            strcat(buffer, "#");
                            valread = send(sd, buffer, strlen(buffer), 0);    
                        }
                        else
                        {
                            hello = "^Client dont  have rights to access this file or File doesnot exist#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);

                        }
                        free(buffer);
                    }
                    else if(1 == sscanf(str, "/read %[^ \n] %[^\n]", s,s3))
                    {
                        k1=0;
                        k2= -1;
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);
                        if( is_file(s) && (is_owner(sd,s) || has_E_permission(sd,s)|| has_V_permission(sd,s)))
                        {
                            lines(buffer,s,k1,k2,sd);
                            strcat(buffer, "#");
                            valread = send(sd, buffer, strlen(buffer), 0);    
                        }
                        else
                        {
                            hello = "^Client dont  have rights to access this file or File doesnot exist#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);
                        }
                        free(buffer);
                    }
                    else if(3 ==(p= sscanf(str, "/insert %[^ ] %d \"%[^\"]\" %[^\n]", s, &k1,s2,s3)))
                    {
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);

                        char *buffer5;
                        buffer5 = (char *)malloc(sizeof(char) * size);
                        bzero(buffer5, size);

                        new_line(s2);
                        if( is_file(s) && (is_owner(sd,s) || has_E_permission(sd,s)))
                        {
                            insert(buffer,s,k1,s2,sd);
                            strcat(buffer, "#");
                            valread = send(sd, buffer, strlen(buffer), 0);

                            get_message_without(s,buffer5);
                            strcat(buffer5, "#");
                            valread = send(sd, buffer5, strlen(buffer5), 0);

                        }
                        else
                        {
                            hello = "^Client dont  have rights to access this file to Insert or File doesnot exist#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);
                        }
                        free(buffer);
                    }
                    else if(2 ==(p =sscanf(str, "/insert %[^ ] \"%[^\"]\" %[^\n]", s,s2,s3)))
                    {
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);
                        char *buffer5;
                        buffer5 = (char *)malloc(sizeof(char) * size);
                        bzero(buffer5, size);
                        new_line(s2);
                        if( is_file(s) && (is_owner(sd,s) || has_E_permission(sd,s)))
                        {
                            insert_end(buffer,s,k1,s2,sd);
                            strcat(buffer, "#");
                            valread = send(sd, buffer, strlen(buffer), 0);

                             get_message_without(s,buffer5);
                            strcat(buffer5, "#");
                            valread = send(sd, buffer5, strlen(buffer5), 0);
                        }
                        else
                        {
                            hello = "^Client dont  have rights to access this file to Insert or or File doesnot exist#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);
                        }
                        free(buffer);
                    }
                    else if(3 == sscanf(str, "/delete %[^ ] %d %d%[^\n]", s,&k1,&k2,s3))
                    {
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);
                        char *buffer5;
                        buffer5 = (char *)malloc(sizeof(char) * size);
                        bzero(buffer5, size);
                        if( is_file(s) && (is_owner(sd,s) || has_E_permission(sd,s)))
                        {
                            delete(buffer,s,k1,k2,sd);
                            strcat(buffer, "#");
                            valread = send(sd, buffer, strlen(buffer), 0);

                             get_message_without(s,buffer5);
                            strcat(buffer5, "#");
                            valread = send(sd, buffer5, strlen(buffer5), 0);
                        }
                        else
                        {
                            hello = "^Client dont  have rights to access this file to Delete or File doesnot exist#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);
                        }
                        
                        free(buffer);

                    }
                    else if(2 == sscanf(str, "/delete %[^ ] %d%[^\n]", s,&k1,s3))
                    {
                        k2=k1;
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);
                        char *buffer5;
                        buffer5 = (char *)malloc(sizeof(char) * size);
                        bzero(buffer5, size);

                        if( is_file(s) && (is_owner(sd,s) || has_E_permission(sd,s)))
                        {
                            delete(buffer,s,k1,k2,sd);
                            strcat(buffer, "#");
                            valread = send(sd, buffer, strlen(buffer), 0);

                             get_message_without(s,buffer5);
                            strcat(buffer5, "#");
                            valread = send(sd, buffer5, strlen(buffer5), 0);
                        }
                        else
                        {
                            hello = "^Client dont  have rights to access this file to Delete or File doesnot exist#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);
                        }
                        free(buffer);

                    }
                    else if(1 == sscanf(str, "/delete %[^ \n] %[^\n]", s,s3))
                    {
                        k1=0;
                        k2= -1;
                        char *buffer;
                        buffer = (char *)malloc(sizeof(char) * size);
                        bzero(buffer, size);


                        char *buffer5;
                        buffer5 = (char *)malloc(sizeof(char) * size);
                        bzero(buffer5, size);

                        if( is_file(s) && (is_owner(sd,s) || has_E_permission(sd,s)))
                        {
                            delete(buffer,s,k1,k2,sd);
                            strcat(buffer, "#");
                            valread = send(sd, buffer, strlen(buffer), 0);

                             get_message_without(s,buffer5);
                            strcat(buffer5, "#");
                            valread = send(sd, buffer5, strlen(buffer5), 0);
                        }
                        else
                        {
                            hello = "^Client dont  have rights to access this file to Delete or File doesnot exist#";
                            printf("\n%s\n\n", hello);
                            valread = send(sd, hello, strlen(hello), 0);
                        }
                        free(buffer);
                    }
                    else if(strcmp(str,"/exit\n")==0 ||1 == sscanf(str, "/exit%[ ]%[^\n]", s, s1))
                    {
                        printf("server Program Exit As you put Exit Query:");
                        erase_data(sd);
                        break;
                    } 
                    else
                    {
                        printf("Wrong Query Format sent by client: check it once \n");
                        hello = "\nWrong query format: Check it once";
                        send(sd, hello, strlen(hello), 0);
                    }
                }
            }
        }
    }
    return 0;
}
