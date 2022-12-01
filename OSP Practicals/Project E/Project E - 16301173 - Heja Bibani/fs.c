#ifndef FS_H
#define FS_H
/* Prevent multiple inclusion */

#include        <stdint.h>
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
struct entry
{
   	char  user;
	char  name[9];
	char  extension[4];
	short blockcount;
	short block[8];
};

uint8_t bitmap[20];
struct entry directory[32];

int toggle_bit(int block)
{

	int elem=block/8;
	int pos=block%8;
	int mask=1<<pos;

	bitmap[elem]^=mask;

	return bitmap[elem]&mask;
}

int block_status(int block)
{
	int elem=block/8;
	int pos=block%8;
	int mask=1<<pos;

	return bitmap[elem]&mask;
}

void initializeBitMap( )
{
	int i;
	toggle_bit( 0 );
	for( i = 1 ; i < 160 ; i++ )
	{
		toggle_bit( i );
		toggle_bit( i );
	}
}

void initializeFile()
{
	int i;
	for(i = 0 ; i < 32; i++)
	{
		directory[i].user = -1;
	}

	initializeBitMap( );
}

void blockStatusPrint( )
{
	int i, z;

	for(i = 0; i < 160; i++)
	{
		if(block_status(i) > 0)
		{
			z = 1;
		}
		else
		{
			z = 0;
		}

		printf("%d", z);
	}
	printf("\n");
}

void listFiles( )
{
	int i;
	int x;
	int y=0;
	for( i = 0 ; i < 32 ; i++ )
	{
		x = ( int )directory[ i ].user;
		if( x == 1 )
		{
			char *s;
			s = (char *)malloc( (strlen(directory[i].name) + strlen(directory[i].extension) + 2)*sizeof(char) );
			s[0] = '\0';
			strcat(s, directory[i].name);
			strcat(s,".");
			strcat(s, directory[i].extension);
			printf( "File[%d] Name: %s File Extension: %s File Block Count: %d\n" , i , s, directory[i].extension, directory[i].blockcount );
			s ='\0';
			free(s);
			y++;
		}
	}

	if(y==0)
	{
		printf("No files to print.\n");
	}
}

int createFile( char *name )
{
	int i;
	int x;
	int firstopen = -1;
	for(i = 0 ; i < 32; i++)
	{
		if(	(x = ( int )directory[ i ].user ) == -1)
		{
			firstopen = i;
			break;
		}
	}

	char *extension;
	char* string = (char*)malloc((strlen(name) + 1)*sizeof(char));
	strcpy(string,name);
	name = strtok(string, "." );
	extension = strtok(NULL, "");
	
	if(strlen( name ) > 8)
	{
		fprintf( stderr , "File name too big.\n" );
		string[0] = '\0';
		free(string);
		return -1;
	}

	if(firstopen != -1)
	{
		directory[ firstopen ].user = 1;
		strcpy( directory[ firstopen ].name , name );
		strcpy(directory[ firstopen ].extension, extension);
		directory[ firstopen ].blockcount = 0;
		string[0] = '\0';
		free(string);
		return firstopen;
	}

}

int openFile( char* name )
{
	int i = 0;
	int  number = -1;
	int x;
	int test;
	char*s;
	s = (char *)malloc( (strlen(directory[i].name) + strlen(directory[i].extension) + 2)*sizeof(char) );
	
	for( i = 0 ; i < 32 ; i++ )
	{	
		s[0] = '\0';
		strcat(s, directory[i].name);
		strcat(s,".");
		strcat(s, directory[i].extension);

		if( strcmp( s , name ) == 0 )
		{
			number = i;
		}
	}

	s[0] = '\0';
	free(s);
	if( number == -1 )
	{
		test = createFile( name );
		return test;
	}

	return number;
}


int readFile( int block )
{
	if(block == -1)
	{
	  printf("Have not chosen a valid file.\n");
	  return -1;
	}
		
	int i;

	char*s;
	s = (char *)malloc( (strlen(directory[block].name) + strlen(directory[block].extension) + 2)*sizeof(char) );
	s[0] = '\0';
	strcat(s, directory[block].name);
	strcat(s,".");
	strcat(s, directory[block].extension);

	printf("File name: %s  Blocks: \n", s);

	for(i = 0 ; i < directory[block].blockcount; i++)
	{
		printf("Block number[%d]: %d \n", i, directory[block].block[i]);
	}
	
		//needs to go to end
	if(directory[block].blockcount == 0)
	{
		printf("No blocks have been allocated.");
	}
	
	s[0] = '\0';
	free(s);
	
	return 0;
}

int writingFile( int block )
{
	if(block == -1)
	{
	  printf("Have not chosen a valid file.\n");
	  return -1;
	}

	int i;
	if(directory[block].blockcount == 8)
	{
		printf("File has reached maximum capcity. \n");
		return -1;
	}

	for( i = 1; i < 160; i++)
	{
		if(block_status(i) == 0 && directory[block].blockcount < 8)
		{
			directory[block].block[directory[block].blockcount] = i;
			directory[block].blockcount++;
			toggle_bit(i);
			break;
		}

		if( i == 159 )
		{
			printf("Disk full.");
			return -1;
		}
	}
	
	return 0;
}

int deletefile( int block )
{
	
	if(block == -1)
	{
	  printf("Have not chosen a valid file.\n");
	  return -1;
	}
	
	int i;
	int size = directory[block].blockcount;

	for(i = 0 ; i < size; i++)
	{
		if(directory[block].block[i] > 0)
		{
			toggle_bit(directory[block].block[i]);
			directory[block].blockcount--;
			directory[block].block[i] = -1;
		}
	}



	directory[block].user = -1;
	directory[block].extension[0] = '\0';
	directory[block].name[0] = '\0';
	return -1;

}

void getFilename(int block)
{
 	char*s;
	s = (char *)malloc( (strlen(directory[block].name) + strlen(directory[block].extension) + 2)*sizeof(char) );
	s[0] = '\0';
	strcat(s, directory[block].name);
	strcat(s,".");
	strcat(s, directory[block].extension);

	printf("Opened: %s\n", s);
	s[0] = '\0';
	free(s);
}

#endif
