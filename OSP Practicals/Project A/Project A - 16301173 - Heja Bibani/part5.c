#include<stdio.h>
#include<stdint.h>
#include"bits.h"


int main(int ac, char **av)
{
	uint8_t val=0b10101010;
	uint8_t val2=0b11101010;

	print_bits(val);
	putchar('\n');
	print_bits(val2);
	putchar('\n');

	print_bits(get_subfield(val,0,2));
	putchar('\n');

	print_bits(get_subfield(val,1,3));
	putchar('\n');

	print_bits(get_subfield(val2,1,7));
	putchar('\n');

	return 0;
}
