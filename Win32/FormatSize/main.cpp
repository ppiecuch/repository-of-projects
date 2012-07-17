#include <stdio.h>
#include <memory.h>

char* readable_fs(double size/*in bytes*/, char *buf) {
	int i = 0;
	const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
	while (size > 1024) {
		size /= 1024;
		i++;
	}
	sprintf(buf, "%.*f %s", i, size, units[i]);
	return buf;
}

inline char* yonFormatSize(double size) {
	static char buf[20];
	memset(buf,0x0,20);

	int i = 0;
	const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
	while (size > 1024) {
		size /= 1024;
		i++;
	}
	sprintf(buf, "%.*f%s", i, size, units[i]);
	return buf;
}

int main(int argc, char* argv[])
{
	char buf[10];
	//printf("File size: %s\n", readable_fs(819200, buf));
	printf("File size: %s\n", yonFormatSize(819200));
	printf("File size: %s\n", yonFormatSize(819200));
	//printf("File size: %s\n", readable_fs(0, buf));
	getchar();
}