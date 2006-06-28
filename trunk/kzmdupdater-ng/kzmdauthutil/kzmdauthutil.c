#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	char path[1024];
	char buffer[1024];
	FILE *fd;
	FILE *fifo;

	if (argc < 2) {
		printf("Please specify a path\n");
		return -1;
	}
	if (getuid() != 0) {
		printf("Must run as root\n");
		return -1;
	}

	strncpy(path, argv[1], 1023);
	strncat(path, "/deviceid", (1023-sizeof(path)));
	
	if ( (fd = fopen(path, "r")) == NULL) {
		printf("Cannot open etc files\n");
		return -1;
	}

	mkfifo("/var/tmp/kzmd-auth", 0666);

	if ( (fifo = fopen("/var/tmp/kzmd-auth", "w")) == NULL) {
		printf("Cannot open the fifo\n");
		return -1;
	}

	fgets(buffer, 1023, fd);
	fputs(buffer, fifo);
	putc('\n', fifo);
	fclose(fd);
	memset(buffer, '\0', 1024);
	memset(path, '\0', 1024);

	strncpy(path, argv[1], 1023);
	strncat(path, "/secret", (1023-sizeof(path)));
	
	if ( (fd = fopen(path, "r")) == NULL) {
		printf("Cannot open secret file\n");
		return -1;
	}

	fgets(buffer, 1023, fd);
	fputs(buffer, fifo);
	putc('\n', fifo);
	fclose(fd);
	fclose(fifo);
	return 0;
}
