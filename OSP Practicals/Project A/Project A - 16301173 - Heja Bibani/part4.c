#include<stdio.h>
#include<stdint.h>
#include"bits.h"


int main(int ac, char **av)
{

	uint8_t val = 0;
	uint8_t bit;

	for(bit=0;bit<8;bit++)
	{
		printf("val:%u, bit:%u\n",val,bit);
		print_bits(val);
		putchar('\n');
		toggle_bit(&val,bit);
		print_bits(val);
		putchar('\n');
	}

	for(bit=0;bit<8;bit++)
	{
		printf("val:%u, bit:%u\n",val,bit);
		print_bits(val);
		putchar('\n');
		toggle_bit(&val,bit);
		print_bits(val);
		putchar('\n');
	}

	return 0;
}
