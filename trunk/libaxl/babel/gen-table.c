#include <stdio.h>
#include <axl.h>

int main (int argc, char ** argv)
{
	FILE * file;
	FILE * file2;
	FILE * output;
	char   buffer[2];
	char   buffer2[4];
	int    iterator;
	char * tolower_encoding;
	char * toupper_encoding;
	char * encoding;
	char * string_aux;
	int    max_value;

	if (argc != 4) {
		printf ("Please ejecute: ./gen-table ORIGINAL_FILE UTF8-TRANSLATE-FILE CODING-NAME\n");
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
	

	encoding          = (char *) argv[3];
	tolower_encoding  = axl_stream_to_lower_copy (encoding);
	axl_stream_remove (tolower_encoding, "-", false);
	axl_stream_remove (tolower_encoding, ".", false);

	toupper_encoding  = axl_stream_to_upper_copy (encoding);
	axl_stream_remove (toupper_encoding, "-", false);
	axl_stream_remove (toupper_encoding, ".", false);

	/* open output */
	string_aux = axl_strdup_printf ("axl_babel_%s.c", tolower_encoding);
	output = fopen (string_aux, "w");
	if (output == NULL) {
		printf ("failed to open output: %s..\n", string_aux);
		axl_free (string_aux);
		return -1;
	}
	

	fprintf (output, "/**\n");
	fprintf (output, " * Axl Babel: encoding support for axl: %s coding\n", encoding);
	fprintf (output, " * Copyright (C) 2008 Advanced Software Production Line, S.L.\n");
	fprintf (output, " */\n\n");
	fprintf (output, "/* include base header */\n");
	fprintf (output, "#include <axl_babel_%s.h>\n\n", tolower_encoding);

	fprintf (output, "/**\n");
	fprintf (output, " * @brief Creates the translation table for %s representation\n",
		encoding);
	fprintf (output, " *  to move information from %s to utf-8 and viceversa.\n", encoding);
	fprintf (output, " */\n");
	fprintf (output, "axlBabelTable * axl_babel_build_%s_table (void) {\n\n", tolower_encoding);

	fprintf (output, "\taxlBabelTable * table;\n\n");

	/* foreach item at the original array do */
	iterator = 0;
	while (fread (buffer, 1, 1, file) == 1) {

		/* read the terminator */
		if (fread (&(buffer[1]), 1, 1, file) != 1) {
			break;
		}
		iterator++;
	} /* end file */
	fclose (file);

	/* record max value */
	max_value = iterator;

	file = fopen (argv[1], "r");
	if (file == NULL) {
		printf ("failed to open: %s..\n", argv[1]);
		return -1;
	}

	fprintf (output, "\t/* create the table to hold information translate %s encoding */\n", encoding);
	fprintf (output, "\ttable = axl_new (axlBabelTable, %d);\n\n", iterator);
	
	/* foreach item at the original array do */
	iterator = 0;
	while (fread (buffer, 1, 1, file) == 1) {

		/* read character from base file */
		if (fread (&(buffer[1]), 1, 1, file) != 1) {
			break;
		}

		if (buffer[1] != '\n') {
			printf ("ERROR (1): Expected to find new line character at=%d..\n", iterator);
			return -1;
		}   

		/* check max value here */
		if (iterator >= max_value) {
			printf ("ERROR (2): Reached max value expected at the translated file. Translation table must have a 1:1 relation.\n");
			return -1;
		}

		/* read content from translated file */
		if (fread (buffer2, 1, 1, file2) != 1) {
			printf ("ERROR (3): failed to read utf-8 file at iterator=%d (while handling buffer[0]='%c' (%u)..\n", 
				iterator, buffer[0], (unsigned char) buffer[0]);
			return -1;
		} /* end if */

		if (buffer2[0] == '\n' && buffer[0] != '\n') {
			printf ("WARN: found no definition for buffer[0]='%c' (%u), iterator=%d..\n",
				buffer[0], (unsigned char) buffer[0], iterator);
			fprintf (output, "\t/* store item associated to code %d */\n", iterator);
			fprintf (output, "\ttable[%d].size      = 1;\n", iterator);
			fprintf (output, "\ttable[%d].buffer[0] = 0; /* unsupported translation */\n\n", iterator);
			iterator++;
			continue;
		}

		if (fread (&(buffer2[1]), 1, 1, file2) != 1) {
			printf ("ERROR (4): failed to read utf-8 file (while handling buffer[0]='%c'..\n", iterator);
			return -1;
		} /* end if */

		if (buffer2[1] == '\n') {
			fprintf (output, "\t/* store item associated to code %d */\n", iterator);
			fprintf (output, "\ttable[%d].size      = 1;\n", iterator);
			fprintf (output, "\ttable[%d].buffer[0] = (unsigned char) %d; /* 0x%x */\n\n", iterator, (unsigned char) iterator, (unsigned char) iterator);
			iterator++;
			continue;
		}

		if (fread (&(buffer2[2]), 1, 1, file2) != 1) {
			printf ("ERROR (5): failed to read utf-8 file (2 type unit)..\n");
			return -1;
		} /* end if */
		
		if (buffer2[2] == '\n') {
			fprintf (output, "\t/* store item associated to code %d */\n", iterator);
			fprintf (output, "\ttable[%d].size      = 2;\n", iterator);
			fprintf (output, "\ttable[%d].buffer[0] = (unsigned char) %u; /* 0x%x */\n", iterator, (unsigned char) buffer2[0], (unsigned char) buffer2[0]);
			fprintf (output, "\ttable[%d].buffer[1] = (unsigned char) %u; /* 0x%x */\n\n", iterator, (unsigned char) buffer2[1], (unsigned char) buffer2[1]);
			iterator++;
			continue;
		}

		if (fread (&(buffer2[3]), 1, 1, file2) != 1) {
			printf ("ERROR (6): failed to read utf-8 file (3 type unit)..\n");
			return -1;
		} /* end if */

		fprintf (output, "\t/* store item associated to code %d */\n", iterator);
		fprintf (output, "\ttable[%d].size      = 3;\n", iterator);
		fprintf (output, "\ttable[%d].buffer[0] = (unsigned char) %u; /* 0x%x */\n",   iterator, (unsigned char) buffer2[0], (unsigned char) buffer2[0]);
		fprintf (output, "\ttable[%d].buffer[1] = (unsigned char) %u; /* 0x%x */\n",   iterator, (unsigned char) buffer2[1], (unsigned char) buffer2[1]);
		fprintf (output, "\ttable[%d].buffer[2] = (unsigned char) %u; /* 0x%x */\n\n", iterator, (unsigned char) buffer2[2], (unsigned char) buffer2[2]);
		iterator++;
		continue;
		
	}
	fclose (file);

	fprintf (output, "\t/* return table created */\n");
	fprintf (output, "\treturn table;\n\n");

	fprintf (output, "} /* end axl_babel_build_%s_table */\n\n", tolower_encoding);
	
	/* close file */
	fclose (output);
	printf ("ok, result at: %s..\n", string_aux);
	axl_free (string_aux);

	/* now write header */
	string_aux = axl_strdup_printf ("axl_babel_%s.h", tolower_encoding);
	output = fopen (string_aux, "w");
	if (output == NULL) {
		printf ("failed to open output: %s..\n", string_aux);
		axl_free (string_aux);
		return -1;
	}

	fprintf (output, "/**\n");
	fprintf (output, " * Axl Babel: encoding support for axl: %s coding\n", encoding);
	fprintf (output, " * Copyright (C) 2008 Advanced Software Production Line, S.L.\n");
	fprintf (output, " */\n\n");
	fprintf (output, "/* include base header */\n");
	fprintf (output, "#include <axl_babel.h>\n\n");

	fprintf (output, "#ifndef __AXL_BABEL_%s_H__\n", toupper_encoding);
	fprintf (output, "#define __AXL_BABEL_%s_H__\n\n", toupper_encoding);

	fprintf (output, "/**\n");
	fprintf (output, " * @brief Creates the translation table for %s representation to move information\n",
		encoding);
	fprintf (output, " * from %s to utf-8 and viceversa.\n", encoding);
	fprintf (output, " */\n");
	fprintf (output, "axlBabelTable * axl_babel_build_%s_table (void);\n", tolower_encoding);

	fprintf (output, "\n");
	fprintf (output, "#endif /* end __AXL_BABEL_%s_H__ */\n\n", toupper_encoding);

	/* close file */
	fclose (output);
	printf ("ok, result at: %s..\n", string_aux);
	axl_free (string_aux);
	
	/* free values */
	axl_free (toupper_encoding);
	axl_free (tolower_encoding);

	return 0;
}
