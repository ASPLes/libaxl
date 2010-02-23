#include <stdio.h>

int main (int argc, char ** argv)
{
	FILE * file;
	FILE * file2;
	char   buffer[2];
	char   buffer2[4];
	int    iterator;

	if (argc != 3) {
		printf ("Please provide two files ORIGINAL and UTF-8-TRANSLATE..\n");
		return -1;
	}

	file = fopen (argv[1], "r");
	if (file == NULL) {
		printf ("failed to open: %s..\n", argv[1]);
		return -1;
	}

	file2 = fopen (argv[2], "r");
	if (file2 == NULL) {
		printf ("failed to open: %s..\n", argv[2]);
		return -1;
	}
	
	/* foreach item at the original array do */
	iterator = 0;
	while (fread (buffer, 1, 1, file) == 1) {

		/* read the terminator */
		if (fread (&(buffer[1]), 1, 1, file) != 1) {
			break;
		}
		 
		if (buffer[1] != '\n') {
			printf ("Expected to find new line character at=%d..\n", iterator);
			return -1;
		}   


		if (fread (buffer2, 1, 1, file2) != 1) {
			printf ("failed to read utf-8 file (while handling buffer[0]='%c' (%u)..\n", iterator, buffer[0], (unsigned char) buffer[0]);
			return -1;
		} /* end if */

		if (fread (&(buffer2[1]), 1, 1, file2) != 1) {
			printf ("failed to read utf-8 file (while handling buffer[0]='%c'..\n", iterator);
			return -1;
		} /* end if */

		if (buffer2[1] == '\n') {
			printf ("/* store item associated to code %d */\n", iterator);
			printf ("table[%d].size      = 1;\n", iterator);
			printf ("table[%d].buffer[0] = (unsigned char) %d; /* 0x%x */\n\n", iterator, (unsigned char) iterator, (unsigned char) iterator);
			iterator++;
			continue;
		}

		if (fread (&(buffer2[2]), 1, 1, file2) != 1) {
			printf ("failed to read utf-8 file (2 type unit)..\n");
			return -1;
		} /* end if */
		
		if (buffer2[2] == '\n') {
			printf ("/* store item associated to code %d */\n", iterator);
			printf ("table[%d].size      = 2;\n", iterator);
			printf ("table[%d].buffer[0] = (unsigned char) %u; /* 0x%x */\n", iterator, (unsigned char) buffer2[0], (unsigned char) buffer2[0]);
			printf ("table[%d].buffer[1] = (unsigned char) %u; /* 0x%x */\n\n", iterator, (unsigned char) buffer2[1], (unsigned char) buffer2[1]);
			iterator++;
			continue;
		}

		if (fread (&(buffer2[3]), 1, 1, file2) != 1) {
			printf ("failed to read utf-8 file (3 type unit)..\n");
			return -1;
		} /* end if */

		printf ("/* store item associated to code %d */\n", iterator);
		printf ("table[%d].size      = 3;\n", iterator);
		printf ("table[%d].buffer[0] = (unsigned char) %u; /* 0x%x */\n",   iterator, (unsigned char) buffer2[0], (unsigned char) buffer2[0]);
		printf ("table[%d].buffer[1] = (unsigned char) %u; /* 0x%x */\n",   iterator, (unsigned char) buffer2[1], (unsigned char) buffer2[1]);
		printf ("table[%d].buffer[2] = (unsigned char) %u; /* 0x%x */\n\n", iterator, (unsigned char) buffer2[2], (unsigned char) buffer2[2]);
		iterator++;
		continue;
		
	}
	fclose (file);
	return 0;
}
