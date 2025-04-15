Online File Editor With Collaboration:

Port No: 8888
IP Address: 127.0.0.1 

The program is properly working as expected. Here is description of each Commands:
/users: Up to Mark
/files: Up to Mark
/upload: Up to Mark
/download: Up to Mark
/invite: Up to Mark
	In response to /invite command we only accept(YES/NO) from target client side....any other that that it wil throw the error(Command Invalid).
/read: Up to Mark:
/insert: Up to Mark:
    If you want to put message in next line use the symbol '^'. This character will behave like the '\n' new line character.
/delete: Up to Mark
/exit: Up to Mark


1. Maxi No of clients allowd to connect with server at the same time are 5 Only.
2. if any clients want to upload file with the same name Server will throw the error. Here Extention of file is included in file name. 
3. Internal data structure. 
    I have created a array of structure.It contains following datatypes(char array, int array,int variable)
    struct file_details
    {
        char f_name[256];
        int owner;
        int collaboration[10];
        char permission[10];
        int valid;
    }

    In Invite command, I am storing permission in array of structure.
    struct invitation
    {
        char f_name[256];
        int clt;
        char permission;
    }
    here variable valid is used to mark file details active or deleted.For sending the Active file details to client First,server creates a Temp File, stores all details in it and then send it to the respective client.
4. if a client exits then all the files it owned(and their respect data) will be marked deleted and if it has any permission to any other file those permission will also be deleted. 
5. File name convention is same as earlier assignment.we dont accept any space in file name.Except space any other special characters are allowed.
6. At a time,only one client can send the invite request.


Compile the Program by using:
cd server/
gcc server.c -o server

gcc client.c -o client

Run the program as 
./server
./client
