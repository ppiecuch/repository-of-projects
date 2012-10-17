#include <stdio.h>
#include <stdlib.h>
int hex2int(const char* str)
{
	return (int)strtol( str, NULL, 16);
}
int main(int argc, char* argv[])
{
	//const char* string = "3.1415926This stopped it";
	//double  x = strtod( string, NULL);
	//printf("strtod = %f\n", x );
	const char* string = "0xDEFA66";
	long  x = strtol( string, NULL, 16);
	printf("string = %s\n", string );
	printf("strtod = %d\n", x );

	getchar();
	return 0;
}