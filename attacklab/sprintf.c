#include <stdio.h>

int main()
{
	unsigned int val = 0x59b997fa;
	char buf[20];
	sprintf(buf, "%.8x", val);
	printf("%s", buf);
	return 0;
}


