/* logserver.c -- implementation of the log server */
#include <signal.h>
#include "logservice.h"

int queue_id; /* stores queue_id for use in signal handler */

void handler( int sig );
void errorExit( );

int main( )
{
	printf( "Creating service!\n" );

	queue_id  = msgget( KEY , IPC_CREAT | IPC_EXCL | 0660 ); // create server, and get id
	
	if( queue_id == -1 )
	{
		perror( "Message set" );
		exit( 1 );
	}
	
	//need to store information being recieved into struct
	struct message test; 
	
	/*signal handler installed on function called handler [deletes the queue on ctrl-c]*/
	signal( SIGINT , handler );
	
	while ( 1 )
	{
		/*receive a message*/
		if ( msgrcv( queue_id , &test , MSGCHARS , 0 , 0 ) == - 1 )
		{
		   perror( "msgrcv" );
		   errorExit( );
		}
		else
		{
			/*display the message*/
			printf( "Reciever recieved, Process ID: %ld  Message: %s\n" , test.type , test.message );	
		}	
	}
	
	return 0;
}

void errorExit( )
{
	/* clean up the queue here */
	if ( msgctl( queue_id , IPC_RMID , NULL ) == -1 )
	{
  		perror( "msgctl failed" );
  		exit( 4 );
   	}
   	
   	printf( "\nReciever error,  message queue destroyed, exited...\n" );
   	exit( 2 );
}

void handler( int sig )
{
	/* clean up the queue here */
	if ( msgctl( queue_id , IPC_RMID , NULL ) == -1 )
	{
  		perror( "msgctl failed" );
  		exit( 4 );
   	}
    
	printf( "\nSignal captured,  message queue destroyed, exited...\n" );
	exit( 1 );
}
