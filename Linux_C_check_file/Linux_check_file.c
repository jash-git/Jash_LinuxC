#include <sys/stat.h> /* struct stat, fchmod (), stat (), S_ISREG, S_ISDIR */
#include <stdio.h>

int main (int argc, char *argv[])
{
	struct stat sts;
	if ((stat (argv [1], &sts)) == -1)
	{
		printf ("The file %s doesn't exist...\n", argv [1]);
	}
	else
	{
		printf ("The file %s does exist...\n", argv [1]);
	}
	return 0;
}