/* logservice.c -- implementation of the log service */
#include "logservice.h"

int logServiceInit( )
{
	int id;
	
	/* getID ,connect to message queue here, no need for other attributes */
	id = msgget( KEY , 0660 ); //connect to service
	return id;				   //return id
}

int logMessage( int serviceId , char *message )
{
	/*Validate message length here */
	if( strlen( message ) > MSGCHARS )
	{
		return -1;
	}
	
	int processId = getpid( );              //get processID for message
	struct message test; 					//struct to store message
	strcpy( test.message , message ); 	  	//copy variables
	test.type = processId;				 	//store processId of child
	printf( "Message sent, processID: %ld String: %s\n" , test.type , test.message );

	/*Send the message */
	int rv;
	rv = msgsnd( serviceId , &test , MSGCHARS , 0 );

	if( rv == -1 )
	{
		perror( "msgsnd" );
	}
	else
	{
		printf( "Message sent successfully!\n" );
	}	

	return rv; // returns 0 or -1 on sucess
}
