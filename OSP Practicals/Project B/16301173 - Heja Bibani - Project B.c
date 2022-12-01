#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<errno.h>

/*argument length*/
#define MAX_LINE 4096

/*Maximum arguments, can only be half the amount of words because of the spaces for 1 letter words*/
#define MAX_WORDS MAX_LINE/2

/* break line into words separated by whitespace, placing them in the
   array words, and setting the count to nwords */
void tokenize( char *line , char **words , int *nwords );

/*does built-in functions, returns 1 on exit,
 and executes change directory if needed */
int do_built_in( char** words , int nwords );

/*internal built-in function to change directory*/
void changeDirectory( char *path );

/*gets the argument list created by tokenize, prepares for execution*/
void find_and_execute( char *arglist[ ] );

int main()
{
	char line[ MAX_LINE ]; 	//line
	char *words[ MAX_WORDS ]; 	//array of words
	int stop=0,nwords=0;
	int lineMax;
	while(1)
	{
		printf( "OSP CLI $  " );

		if( fgets( line , MAX_LINE, stdin ) != NULL ) //if NULL EOF entered
		{
			tokenize( line , words , &nwords ); //prepares the array of arguments

			stop = do_built_in( words ,  nwords ); //checks to see if command is built-in

			if( stop == 0 )
			{
				find_and_execute( words );
			}
			else if( stop == 1 )
			{
				break;
			}
		}
		else
		{
			printf("\nEOF exit. \n"); //eof exit
			break;
		}

		//reset variables
		words[ nwords ] = NULL;
		nwords = 0;
	}
	printf("\n");
	return 0;
}

/*does built-in functions, returns 1 on exit,
 and executes change directory if needed */
int do_built_in( char** words, int nwords )
{
	//checks to see if user has written "exit"
	if( ( strncmp( *words , "exit" , 4 )  == 0 ) && ( strlen( *words ) == 4 ) &&  ( nwords == 1 ) )
	{
		return 1;
	}

	//checks to see if first command is "cd", then changes the directory
  	if ( ( strncmp( *words , "cd" , 2 ) == 0 )  && ( strlen( *words ) == 2 ) )
 	{
  		changeDirectory( words[ 1 ] );
  		return 2;
  	}

  	return 0;
}

/*internal built-in function to change directory*/
void changeDirectory( char *path )
{
	//change directory system call invoked by path
	int cdStatus = chdir( path );

	if( cdStatus == -1 )
	{
		perror( "cd error [internal]" );
	}
	
	//print current working directory
	char cwd[ 100 ];
	getcwd( cwd ,sizeof( cwd ) );
	printf( "Current Directory: %s \n" , cwd );
}

/*gets the argument list created by tokenize 
and forks and executes the program*/
void find_and_execute( char *arglist[ ] )
{
	int	pid,exitstatus;
	pid = fork( );			// make new process

	switch( pid )
	{
		case -1:
			perror( "fork failed" );
			exit( 1 );
		case 0:
			execvp( arglist[ 0 ] , arglist );
			perror( "execvp failed" );
			exit( 1 ); //will return 1 to parent if an error occured
		default:   	//if value greater than zero; then it is the parent
			while( wait( &exitstatus ) != pid );
			printf( "child exited with status %d\n",  //we get the values of exit status [use shift] to determine status
					exitstatus>>8 );
	}
}

/*Gets the argument, uses a token to break those arguments
 into successive words, and place them into an array*/
void tokenize( char *line , char **words , int *nwords )
{
    *nwords=1;

    for( words[ 0 ] = strtok( line ," \t\n" );			 //first line gets tokenized and sent to array 1
        ( *nwords < MAX_WORDS ) && (words[ *nwords ]=strtok( NULL , " \t\n" ) ); // after each successive call to strtok, you use NULL
        *nwords=*nwords+1
       );

    return;
}
