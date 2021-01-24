#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
	int testdev;
	unsigned char buf[4];

	testdev = open("/dev/wordcount", O_RDWR);
	if (testdev == -1)
	{
		printf("Cann't open file \n");
		return 0;
	}
	if (argc > 1)
	{

		write(testdev, argv[1], strlen(argv[1]));
		printf("string:%s\n", argv[1]);
	}

	read(testdev, buf, 4);

	int n = 0;
	//  将4个字节还原成int类型的值
	n = ((int) buf[0]) << 24 | ((int) buf[1]) << 16 | ((int) buf[2]) << 8
	        | ((int) buf[3]);
	printf("word byte display:%d,%d,%d,%d\n", buf[0], buf[1], buf[2], buf[3]);
	printf("word count:%d\n", n);
	close(testdev);
	return 0;
}


