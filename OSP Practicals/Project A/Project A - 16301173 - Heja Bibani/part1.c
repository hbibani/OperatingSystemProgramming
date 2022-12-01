#include<stdio.h>
#include<stdint.h>
#include"bits.h"


int main(int ac, char **av)
{
	uint8_t val=0b10101010;

	print_bits(val);
	printf("\n");
	return 0;
}
