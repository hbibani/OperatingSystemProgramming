#include        <stdio.h>
#include        <stdio.h>
#include        <unistd.h>
#include        <fcntl.h>
#include        <stdbool.h>
#include        <sys/stat.h>
#include        <string.h>
#include        <stdlib.h>
#include        <sys/types.h>
#include        <errno.h>
/* stdio.h will be found in the system path */
#include "fs.h"
/* fs.h will be found in the local path */

int main(int ac, char**av)
{
	char string[9];
	int dec = 1;
	int x = -1;
	int y = -2;
	while(1)
	{
		printf("\n\n");
		if(x != -1)
		{
		   getFilename(x);
		}
		else
		{
		   printf("No file currently opened.");

		}
		printf( "\nPick decision\n" );
		printf( "(0) Exit\n" );
		printf( "(1) Initialize Disk\n" );
		printf( "(2) List Files\n" );
		printf( "(3) Display bitmap\n" );
		printf( "(4) Open files\n" );
		printf( "(5) Write To File\n" );
		printf( "(6) Read File\n" );
		printf( "(7) Delete File\n" );

		printf("Enter an option: ");
	 	scanf( "%d", &dec );
		printf( "\n" );

		while( dec < 0 || dec > 7 )
		{
			printf( "That was an invalid choice, please pick a number between 0 and 7.\n" );
			scanf( "%d", &dec );
		}


		
		
		
		switch(dec)
		{
			case 0:
			exit(0);
			break;

			case 1:
			if(y == -2)
			{
				initializeFile();
				y = 0;
			}
			else
			{
			  printf("Already initialized. \n");
			}
			break;

			case 2:
			if(y==0)
			{
				listFiles();
			}
			else
			{
			   printf("Needs initialization. \n");
			}
			break;

			case 3:
			if(y==0)
			{
				blockStatusPrint();
			}
			else
			{
			   printf("Needs initialization. \n");
			}
			break;

			case 4:
			if(y == 0)
			{
				printf("What is the name of the file?\n");
				scanf("%s", string);
				x = openFile(string);
			}
			else
			{
				printf("Needs initialization. \n");
			}
		
			break;

			case 5:
			if(y == 0)
			{
				writingFile(x);
			}
			else
			{
			   printf("Needs initialization. \n");
			}
			break;

			case 6:
			if(y == 0)
			{
				readFile(x);
			}
			else
			{
			   printf("Needs initialization. \n");
			}
			break;


			case 7:
			if(y == 0)
			{
				x = deletefile(x);
			}
			else
			{
			   printf("Needs initialization. \n");
			}
			break;

		}

	}

	return 0;

}
