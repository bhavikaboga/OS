#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 1024   

int writeRequest(int fd, char* string)
{
   printf("Writing to device: \"%s\"\n", string);
   int ret = write(fd, string, strlen(string));
   if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }
   return 0;
}

int readRequest(int fd, int n)
{
   char* buffer = malloc(sizeof(char) * BUFFER_LENGTH);
   int ret = read(fd, buffer, n);

   printf("Reading from device: \"%s\"\n", buffer);

   if (ret < 0){
      perror("Failed to read the message from the device.\n");
      return errno;
   }
   
   free(buffer);
   buffer = NULL;
   return 0;
}

int main(void)
{
	int ret, fd;
	char stringToSend[BUFFER_LENGTH];
	printf("Starting device test code example...\n");
	fd = open("/dev/devChar", O_RDWR);
	
	if (fd < 0)
	{
		perror("Failed to open the device...");
		return errno;
	}

	writeRequest(fd, "COP4600:"); 
	readRequest(fd, 4); // Should print "COP4"
	writeRequest(fd, "OperatingSystems");
	readRequest(fd, 7); // Should print "600:Ope"
	readRequest(fd, 15); // Should print "ratingSystems"

	printf("End of the program\n");

	return 0;
}
