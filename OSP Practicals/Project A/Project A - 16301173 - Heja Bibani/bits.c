#include<stdio.h>
#include<stdint.h>
#include"bits.h"

void print_bits( uint8_t value )
{
	  //gets value from 1 and shifts from loop starting from seven
	  //the value will be anded with this shifted value
	  //if the result is greater than zero, then print 1 else print 0
  	  int i;
	  for( i = 7 ; i >=0 ; i-- )
	  {
		if( ( value  & ( 1<<i ) ) > 0)
		{
		  printf( "1" );
		}
		else
		{
		   printf( "0" );
		}
	  }
}

uint8_t toggle_bit( uint8_t *value , uint8_t bit )
{
	//use XOR operator to toggle bit [ 0 , 0 ] and [ 1 ,0 ] is not applied
	 *value = *value ^ ( 1 << bit );
}

uint8_t reverse_bits( uint8_t value )
{
   uint8_t test = 128;
   uint8_t test2 = 0;
   int x;

	//we sttart at 128 and and with value, if it is greater than 0, we toggle it's corresponding reverse bit
	for( x = 0 ; x <=7 ; x++ )
	{
	     if( ( test & value ) > 0 )
	     {
	     		toggle_bit( &test2 , x );
	     }

		test = test>>1;
	}

	return test2;
}

 uint8_t check_bit( uint8_t value , uint8_t bit )
{
	//we shift 1 by the bit we are intersted in
	//we "and" the resultant to the value
	//if the value is greater than zero it's on
	//if not, then it is off
	
   uint8_t test  = ( value & ( 1<<bit ) );

   if( ( test >> bit ) > 0 )
   {
    	return 1;
   }
   else
   {
   		return 0;
   }
}

uint8_t get_subfield( uint8_t value , uint8_t start , uint8_t stop )
{			
	//The value will be shifted from the stop position
	//once shifted to the left, it will shift to the right to the point that the start is at position 0
	value =  value<<( 7 - stop );
	value =  value>>( 7 - stop + start );
	return value;
}


