
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
	int ret;
	char stringToSend[BUFFER_LENGTH];
	printf("Starting device test code example...\n");
	int fi = open("/dev/devInput", O_RDWR);
	int fo = open("/dev/devOutput", O_RDWR);
	
	if (fi < 0)
	{
		perror("Failed to open the input device...");
		return errno;
	}

	if (fo < 0)
	{
		perror("Failed to open the output device...");
		return errno;
	}

	writeRequest(fi, "COP0UCF: Op"); 
	readRequest(fo, 4); // Should print "COP0"
	writeRequest(fi, "eratingSystems");
	readRequest(fo, 7); // Should print "Undefea"
	readRequest(fo, 50); // Should print "ted 2018 National Champions UCF0:OperatingSystems"

	printf("End of the program\n");

	return 0;
}
