#include <stdio.h>
#include <stdint.h>
void g(int8_t n, int32_t x, int8_t y);
void f(int8_t x, int8_t y);

int main ()
{
	f(4 ,5);
}

void f(int8_t x, int8_t y)
{
	g(2,x,y);
	return;
}

void g(int8_t n, int32_t x, int8_t y)
{
	int8_t z;
	if (n == 0)
	{
		printf("Values: [%d,%d]\n",x,y);
		return;
	}
	else
	{
		z = x+y;
		g(n- 1 , z, z+1);
	}
	return;
}
