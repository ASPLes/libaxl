#include <stdio.h>

int main (int argc, char ** argv)
{
	FILE * file;
	char   buffer[1];
	int    iterator;
	int    item_found = 1;

	if (argc != 2) {
		printf ("Please provide a file..\n");
		return -1;
	}

	file = fopen (argv[1], "r");
	if (file == NULL) {
		printf ("failed to open: %s..\n", argv[0]);
		return -1;
	}
	
	iterator   = 0;
	item_found = 0;
	while (fread (buffer, 1, 1, file) == 1) {		
		if (buffer[0] == '\n'){
			printf ("\n");
			if (item_found) {
				iterator++;
				item_found = 0;
			}
		} else {
			printf ("iterator=%d: value: %d, %c ", iterator, (unsigned char) buffer[0], buffer[0]);
			item_found = 1;
		}

	}
	fclose (file);
	return 0;
}
